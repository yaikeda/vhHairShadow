#version 400
			  
in vec4 mvpPos;//ライトからみた、自己投影影生成用のベクトル

in vec2 texCoord;

//計算済みshadow texture
uniform sampler2D calculatedShadowTexture;

//フラグ関連
uniform float shadow_noshadow;
uniform float pcfFlag;
flat in float sendHairFlag;
flat in float sendOsmLastPosFlag;

//DOSM関連
uniform sampler2DArray depthTexture;
uniform sampler2DArray opacityTexture;
uniform float layerInterval;
uniform float textureInterval;
uniform float textureNum;

//共通
in vec3 normal;

//kajiya-kay関連
in vec3 mvPos;
in vec3 globalPosition;
in vec3 binormal;
in vec3 tangent;
uniform vec3 lightPosition;
uniform float shininessOne;
uniform float shininessTwo;
uniform float specularWeightOne;
uniform float specularWeightTwo;
uniform vec3 diffuseColor;
uniform float diffuseWeight;
uniform vec3 specularColorOne;
uniform vec3 specularColorTwo;

//phongシェーディング
in vec3 localPosition;
in mat4 mMatrix;
flat in vec3 sendCameraPosition;
uniform vec3 lightDiffuseColor;
uniform vec3 materialAmbient;
uniform vec3 lightAmbient;
uniform vec3 materialShininess;
uniform vec3 lightShininess;
uniform float shininess;


//color texture
uniform sampler2D colorTexture;

//テクスチャ問い合わせよう
uniform float textureWidth;
uniform float textureHeight;

int texWidth = int(textureWidth);
int texHeight = int(textureHeight);

out vec4 fragColor;

float KajiyaKayDiffuse( vec3 hairTangent, vec3 lightVector)
{
	float df = dot( hairTangent,normalize( lightVector ) );
	df = 1.0 - (df * df);
	if ( df < 0 ) df = 0;
	if ( df > 0 ) df = sqrt( df );
	return df;
}

float KajiyaKaySpecular( vec3 hairTangent, vec3 lightVector, vec3 viewVector)
{
	float kspec;
	float sintl;
	float sinte;
	float  vt;

	sintl = KajiyaKayDiffuse( hairTangent, lightVector);

	vt =  dot(viewVector, hairTangent);
	sinte = 1.0 - (vt * vt);
	if ( sinte < 0 ) sinte = 0;
	if ( sinte > 0 ) sinte = sqrt( sinte );

	// equl on paper : (t.l)(t.e) + sin(t,l)sin(t.e)
	kspec = sintl * sinte - dot( normalize(lightVector), hairTangent ) * vt;

	if ( kspec < 0 ) kspec = 0;
	return kspec;
}
vec3 lightingKajiyaKay()
{
	//カメラ座標で計算を行っている
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightDir = normalize(lightPosition - globalPosition);
  vec3 viewDir =  normalize(sendCameraPosition - globalPosition);

  vec3 n = normalize(normal);
  vec3 t = normalize(tangent);

  float s1 = max(0.01, shininessOne);
  float s2 = max(0.01, shininessTwo);
  float g1 = specularWeightOne;
  float g2 = specularWeightTwo;
	
  vec3 DiffuseColor   = diffuseColor;
  vec3 specular1 = specularColorOne;
  vec3 SecondarySpecularColor = specularColorTwo;

  float DiffuseWeight = diffuseWeight;  	  	

  float HalfLambert = max(dot(lightDir, n), 0)*0.5+0.5;

  float d = max(0.0,dot(n, normalize(lightDir)));
  float h = min(1.0,max(0.0,HalfLambert));
  	
  float dd = d*(1.0-h) + h;
  float ds = d;  	
  	
  vec3 diff  = DiffuseWeight*DiffuseColor*dd*KajiyaKayDiffuse(t, -lightDir);
  float  ss  = KajiyaKaySpecular(t, -lightDir, -viewDir);
  vec3 spec1 = specular1*ds*pow(ss,s1)*g1;
  vec3 spec2 = SecondarySpecularColor*ds*pow (ss,s2)*g2;

  vec3 c = ((diff) + (spec1 + spec2))*lightColor;	
  return c;
}






vec3 getDiffuseSpecular()
{
	vec4 worldPosition = mMatrix * vec4(localPosition, 1.0);
	vec4 worldNormal = vec4(normal, 1.0);
	worldNormal.xyz = normalize(worldNormal.xyz);
	

	vec3 V = normalize(sendCameraPosition - worldPosition.xyz);
	vec3 L = normalize(lightPosition - worldPosition.xyz);

	//diffuse
	float diffuseLight = max(dot(L, worldNormal.xyz), 0.0);
	vec3 diffuse = diffuseColor * lightDiffuseColor * diffuseLight;

	//specular
	vec3 specular = vec3(0.0);
	if (diffuseLight > 0.0) {
		vec3 R = reflect(-L, worldNormal.xyz);//Lは、頂点に入ってくる方向のベクトルにする
		float specularLight = pow(max(dot(R, V), 0.0), shininess);
		
		specular = materialShininess*lightShininess*specularLight;
	}

	return diffuse + specular;
}





vec4 getGaussedValue(vec3 textureCoord) 
{
	//取得する点のサイズ 8近傍で, いくつまで外側を取ってくるか。
	int radius = 1;
	float nor = 0.0;

	//近傍テクセルの比較合計
	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);


	vec2 nowTexCoord;
	nowTexCoord.x = textureCoord.x * texWidth;
	nowTexCoord.y = textureCoord.y * texHeight;

	
	if (nowTexCoord.x >= (texWidth - radius) || nowTexCoord.y >= (texHeight - radius) 
		|| nowTexCoord.x <= radius || nowTexCoord.y <= radius) {
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
	

	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			if (i == 0 && j == 0) continue;

			nowTexCoord.x = nowTexCoord.x + i;
			nowTexCoord.y = nowTexCoord.y + j;

			textureCoord.x = nowTexCoord.x / texWidth;
			textureCoord.y = nowTexCoord.y / texHeight;

			sum += texture(opacityTexture, textureCoord);
			nor += 1;
		}
	}

	nor = 1.0 / nor;
	
	
	return sum *= nor;
}

float deepOpacityMapShadow()
{

	const int mapNum = 4;

	vec4 nowPos = mvpPos / mvpPos.w;//現在の注目点の位置-1.0~1.0
	vec2 texPosXY = (nowPos.xy + 1) * 0.5;//0.0~1.0

	vec4 depthTexColor = texture(depthTexture, vec3(texPosXY, 0));//get near
		
	float c_near = depthTexColor.x;//-1.0 ~ 1.0

	int texturePos = 0;
	float allLayer = 2 * textureNum * (1 + 4 * textureNum);
	float c_base = (1.0 - c_near) / allLayer;//レイヤ同士の最小距離
	float textureFar = 0.0;
	for (int i = 0; i < textureNum; i++) {
		textureFar = c_near + c_base * 2 * (i + 1) * (1 + 4 * (i + 1));
		if (nowPos.z < textureFar) {
			texturePos = i;
			break;
		}
	}

	

	//get now opacity colors.
	//色の取得
	float opacityValue[4] = float[4](0.0, 0.0, 0.0, 0.0);
	float beforeLastOpacityValue = 0.0;//一つ前のテクスチャの最後の値をとる		

	vec4 opacityTexColor;
	if (pcfFlag == 0) {
		//single sample
		opacityTexColor = texture(opacityTexture, vec3(texPosXY, texturePos));//4つのopacity map値	
		opacityValue[0] = opacityTexColor.r;
		opacityValue[1] = opacityTexColor.g;
		opacityValue[2] = opacityTexColor.b;
		opacityValue[3] = opacityTexColor.a;

		//before opacity color 
		opacityTexColor = texture(opacityTexture, vec3(texPosXY, texturePos-1));
	} else {
		//gauss
		opacityTexColor = getGaussedValue(vec3(texPosXY, texturePos));//4つのopacity map値
		opacityValue[0] = opacityTexColor.r;
		opacityValue[1] = opacityTexColor.g;
		opacityValue[2] = opacityTexColor.b;
		opacityValue[3] = opacityTexColor.a;

		//before opaciyt color 
		opacityTexColor = getGaussedValue(vec3(texPosXY, texturePos-1));		
	}			

	if (texturePos != 0) {
		beforeLastOpacityValue = opacityTexColor.a;
	} 

	
	
	float layerDistance[mapNum];//注目テクスチャ内の四つの中で、どこの値を参照するかを決める
	float textureNear = 0;
	if (texturePos <= 0) {
		textureNear = c_near;
	} else {
		textureNear = c_near + 
			c_base * 2 * (texturePos) * (1 + 4 * (texturePos));
	}
	for (int i = 0; i < mapNum; i++) {
		if (i == 0) {
			layerDistance[i] = textureNear;
		} else {
			layerDistance[i] = layerDistance[i-1];
		}
		layerDistance[i] += texturePos * 4 * c_base + c_base * (i + 1);
	}

	//calc opacity value
	float resultValue = 0.0;
	for (int count = 0; count < mapNum; count++) {
		if (nowPos.z < layerDistance[count]) {
			/*if (count == 0) {
				resultValue = -1;
				break;
			}*/

			float calcDepth = 0.0;
			if (count == 0) {
				float nowDepth = nowPos.z - textureNear;
				if (nowDepth < 0) { nowDepth = 0; }
				calcDepth = nowDepth / (layerDistance[count] - textureNear);
			} else {
				float nowDepth = nowPos.z - layerDistance[count - 1];
				if (nowDepth < 0) { nowDepth = 0; }
				calcDepth = nowDepth / (layerDistance[count] - layerDistance[count-1]);
			}

			float currentOpacity = 1.0;
			if (count == 0) {
				currentOpacity = (1.0 - calcDepth) * beforeLastOpacityValue + calcDepth * opacityValue[count];
			} else {
				currentOpacity = (1.0 - calcDepth) * opacityValue[count-1] + calcDepth * opacityValue[count];
			}

			if (currentOpacity > 1.0) {
				resultValue = 0.0;
				break;
			} else {
				resultValue = 1.0 - currentOpacity;
				break;
			}
		}
	}
	
	return resultValue;
}

float getLastOpacityValue(void)
{
	vec4 nowPos = mvpPos / mvpPos.w;
	vec2 texPosXY = (nowPos.xy + 1) * 0.5;
	vec3 textureCoord = vec3(texPosXY, int(textureNum - 1));//最後のテクスチャ
	
	vec4 texColor;
	if (pcfFlag == 0) {
		texColor = texture(opacityTexture, textureCoord);
	} else if (pcfFlag == 1) {
		texColor = getGaussedValue(textureCoord);
	}

	if (texColor.a > 1) {
		return 0.0;
	} else {
		return (1.0 - texColor.a); //最後のレイヤ
	} 
}





in vec3 checkVec;
in vec3 sendtangent;
in vec3 sendnormal;
in float sendexpand;
in vec3 sendVec;

void main(void)
{
	


	const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
	const vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
	const vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
	const vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
	const vec4 magenta = vec4(1.0, 0.0, 1.0, 1.0);
	const vec4 cyan = vec4(0.0, 1.0, 1.0, 1.0);
	const vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
	
	if (sendHairFlag == 1) {
		float brightness = deepOpacityMapShadow();

		vec3 ambient = materialAmbient * lightAmbient;

		vec3 kajiyaKayColor = vec3(0.0, 0.0, 0.0);
		
		if (shadow_noshadow == 1.0) {
			kajiyaKayColor = lightingKajiyaKay();	
			fragColor = vec4(ambient + kajiyaKayColor * brightness, 1.0);
			return;
		} else if (shadow_noshadow == 0.0){
			kajiyaKayColor = lightingKajiyaKay();	
			fragColor = vec4(ambient + kajiyaKayColor, 1.0);
			return;
		}
	} else if (sendHairFlag == 0.0) {
		//ambient
			
		vec3 ambient = materialAmbient * lightAmbient;
		//vec3 diffSpec = getDiffuseSpecular();

		if (shadow_noshadow == 1.0) {
			float brightness = getLastOpacityValue();

			fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0) * brightness, 1.0);
		
			//fragColor = vec4(ambient + diffSpec*brightness, 1.0);
		} else if (shadow_noshadow == 0.0){
			//fragColor = vec4(ambient + diffSpec, 1.0);
			fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0), 1.0);
		}
			
		
		return;
	}
}
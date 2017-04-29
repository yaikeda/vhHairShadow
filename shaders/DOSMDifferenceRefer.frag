#version 400
			  
in vec4 mvpPos;//ライトからみた、自己投影影生成用のベクトル

in vec2 texCoord;

//計算済みshadow texture
uniform sampler2D calculatedShadowTexture;
in vec4 cameraMVP;

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


vec4 referCalcedTexture(vec2 textureCoord) 
{
	//取得する点のサイズ 8近傍で, いくつまで外側を取ってくるか。
	int radius = 2;
	float nor = 0.0;

	//近傍テクセルの比較合計
	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);	

	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			if (i == 0 && j == 0) continue;

			sum += textureOffset(calculatedShadowTexture, textureCoord, ivec2(i, j));
			nor += 1;
		}
	}

	nor = 1.0 / nor;
	
	
	return sum *= nor;
}


void main(void)
{
	vec4 nowCameraMVP = cameraMVP / cameraMVP.w;//クリッピング完了
	vec2 calculatedShadowTexturePos = (nowCameraMVP.xy + 1) * 0.5;//テクスチャ座標

	vec4 referedBrightness = texture(calculatedShadowTexture, calculatedShadowTexturePos);
	//vec4 referedBrightness = referCalcedTexture(calculatedShadowTexturePos);
	vec3 ambient = materialAmbient * lightAmbient;

	if (sendHairFlag == 1) {
		vec3 kajiyaKayColor= lightingKajiyaKay();	
		fragColor = vec4(ambient + kajiyaKayColor * referedBrightness[0], 1.0);
   
	} else {
		fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0) * referedBrightness[0], 1.0);
	}
}
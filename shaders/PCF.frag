#version 400

//フラグ関連
flat in float sendHairFlag;
uniform float shadow_noshadow;
uniform float pcfFlag;

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


//shadow map用
in vec4 shadowCoord;
uniform sampler2D shadowTexture;
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





float shadeWithShadow(void)
{
	float shadow = 0;

	vec2 texCoord = shadowCoord.xy / shadowCoord.w;
	texCoord = (texCoord.xy + 1) * 0.5;
	float nowPos = shadowCoord.z / shadowCoord.w;
	float textureValue;

	if (pcfFlag == 0) {
		//pcfでpcfをきっているので, 普通のシャドウマップ
		textureValue = (texture(shadowTexture, texCoord)).x;
		if (textureValue <= nowPos) 
			shadow = 0.0;
		else if (textureValue > nowPos)
			shadow = 1.0; 
	} else if (pcfFlag == 1) {
		int radius = 2;
		//float nor = 1.0 / ((radius*2+1) * (radius*2+1) - 1);//1だとintのままで計算される。
		float nor = 0;

		//近傍テクセルの比較合計
		float sum = 0;

		vec2 nowTexCoord;
		nowTexCoord.x = texCoord.x * texWidth;
		nowTexCoord.y = texCoord.y * texHeight;

		if (nowTexCoord.x >= (texWidth - radius) || nowTexCoord.y >= (texHeight - radius) 
			|| nowTexCoord.x <= 0 || nowTexCoord.y <= 0) {
			return 1.0;
		}

		for (int i = -radius; i <= radius; i++) {
			for (int j = -radius; j <= radius; j++) {
				if (i == 0 && j == 0) continue;

				nowTexCoord.x = nowTexCoord.x + i;
				nowTexCoord.y = nowTexCoord.y + j;

				texCoord.x = nowTexCoord.x / texWidth;
				texCoord.y = nowTexCoord.y / texHeight;

				textureValue = (texture(shadowTexture, texCoord)).x;

				if (textureValue < nowPos) 
					shadow = 0.0;
				else if (textureValue > nowPos)
					shadow = 1.0; 

				sum += shadow;
				nor += 1;
			}
		}

		nor = 1.0 / nor;
		shadow = sum * nor;
	}

	return shadow;
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

void main(void)
{
	float brightness = shadeWithShadow();
	
	if (sendHairFlag == 1.0) {
		vec3 ambient = materialAmbient * lightAmbient;

		vec3 kajiyaKayColor = vec3(0.0, 0.0, 0.0);
		
		if (shadow_noshadow == 1.0) {
			kajiyaKayColor = lightingKajiyaKay();	
			fragColor = vec4(ambient + kajiyaKayColor * brightness, 1.0);
		} else if (shadow_noshadow == 0.0){
			kajiyaKayColor = lightingKajiyaKay();	
			fragColor = vec4(ambient + kajiyaKayColor, 1.0);
		}
		return;
	} else if (sendHairFlag == 0.0) {
		/*
		//ambient
		vec3 ambient = materialAmbient * lightAmbient;
		vec3 diffSpec = getDiffuseSpecular();

		if (shadow_noshadow == 1.0) {
			fragColor = vec4(ambient + diffSpec*brightness, 1.0);
		} else if (shadow_noshadow == 0.0){
			fragColor = vec4(ambient + diffSpec, 1.0);
		}
		return;
		*/
		
		vec3 ambient = materialAmbient * lightAmbient;
		
		if (shadow_noshadow == 1.0) {
			fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0) * brightness, 1.0);
		} else if (shadow_noshadow == 0.0){
			fragColor = vec4(ambient + vec3(1.0, 1.0, 1.0), 1.0);
		}
		return;
	}
}
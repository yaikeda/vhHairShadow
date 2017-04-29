#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in float vertexExpandDirection;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

//light
uniform mat4 lightViewMatrix[10];
uniform mat4 lightProjectionMatrix[10];
out vec4 mvpPos[10];//とりあえず10個まで対応
uniform float lightCount;
flat out float sendLightCount;

//camera
flat out vec3 sendCameraPosition;

//共通
uniform mat4 normalMatrix;

//kajiya-kay関係
out vec3 globalPosition;
out vec3 mvPos;
out vec3 binormal;
out vec3 normal;
out vec3 tangent;

//phongシェーディング
uniform vec3 cameraPosition;
out vec3 localPosition;
out vec3 localNormal;
out mat4 mMatrix;

//テクスチャ取得用
out vec2 texCoord;


//ヘア管理用
uniform float thickness;
uniform float hairFlag;
uniform float cylinderFlag;
flat out float sendHairFlag;



//確認用変数
out vec3 sendtangent;
out vec3 sendnormal;
out float sendexpand;
out vec3 sendVec;

void main()
{
	
	sendHairFlag = hairFlag;
	sendCameraPosition = cameraPosition;
	sendLightCount = lightCount;


	//複数光源は、ひとまず、地面に投射した影のみ描いてみる。
	//自己投射の影も計算する場合は、ベクトル再配置がもう一回必要になる。
	if (hairFlag == 1) {
		if (cylinderFlag == 1) {

			normal = (normalMatrix * vec4(vertexNormal, 1.0)).xyz;
	
			//sendColor = vertexColor;
		
		
		
			//テクスチャ取得用
			//texCoord = vertexTexCoord;
			sendtangent = vertexTangent;
			sendnormal = vertexNormal;
	

			//kajiya-kay
			globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
			mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
			tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;
			binormal = cross(normal, tangent);
		
			//phong
			localPosition = vertexPosition;
			mMatrix = modelMatrix;
		

			vec4 movedPosition = vec4(normal * thickness, 0.0) + modelMatrix * vec4(vertexPosition, 1.0);

			gl_Position = projectionMatrix * viewMatrix * movedPosition;

		}
		else {
			tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;//global

			vec4 nowGlobalPos = modelMatrix * vec4(vertexPosition, 1.0);
			vec3 viewVec = cameraPosition - nowGlobalPos.xyz;
			viewVec = normalize(viewVec);

			
			binormal = cross(viewVec, tangent);
			normal = cross(tangent, binormal);
			
			vec3 offset = binormal * vertexExpandDirection * thickness;

			vec4 movedPosition = vec4(offset, 0.0) + nowGlobalPos;
			
			//kajiya-kay
			globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
			mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
				
			//phong
			localPosition = (inverse(modelMatrix) * movedPosition).xyz;
			mMatrix = modelMatrix;
	
			gl_Position = projectionMatrix * viewMatrix * movedPosition;
		
		}
	} else {
		//kajiya-kay
		globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
		mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
		normal = (normalMatrix * vec4(vertexNormal, 1.0)).xyz;
		tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;
		binormal = cross(normal, tangent);
	
		//phong
		localPosition = vertexPosition;
		mMatrix = modelMatrix;
		

		//テクスチャ取得用
		texCoord = vertexTexCoord;

		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);	
	}


	//共通の変数
	for (int i = 0; i < lightCount; i++) {
		mvpPos[i] = lightProjectionMatrix[i] * lightViewMatrix[i] * modelMatrix * vec4(vertexPosition, 1.0);
	}
	

	
}
#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in float vertexExpandDirection;

//must need matrix
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

//flag
uniform float hairFlag;
uniform float cylinderFlag;
uniform float osmLastPosFlag;
flat out float sendHairFlag;
flat out float sendOsmLastPosFlag;


//light
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

flat out vec3 sendCameraPosition;
out vec4 mvpPos;

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

//DOSM関係
uniform float dosmLastTextureDepth;
uniform float dosmLastPlaneSize;

//テクスチャ取得用
out vec2 texCoord;

//ヘア管理用
uniform float thickness;

//確認用変数
out vec3 sendtangent;
out vec3 sendnormal;
out float sendexpand;
out vec3 sendVec;

void main()
{
	
	sendHairFlag = hairFlag;
	sendCameraPosition = cameraPosition;

	
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

			mvpPos = lightProjectionMatrix * lightViewMatrix * movedPosition;
			gl_Position = projectionMatrix * viewMatrix * movedPosition;

		}
		else {
			tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;//global

			vec4 nowGlobalPos = modelMatrix * vec4(vertexPosition, 1.0);
			vec3 viewVec = cameraPosition - nowGlobalPos.xyz;
			viewVec = normalize(viewVec);

			
			binormal = cross(viewVec, tangent);
			normal = cross(tangent, binormal);

			sendexpand = vertexExpandDirection;
			


			
			vec3 offset = binormal * vertexExpandDirection * thickness;
				
			vec4 movedPosition = vec4(offset, 0.0) + nowGlobalPos;
			
			//kajiya-kay
			globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
			mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
				
			//phong
			localPosition = (inverse(modelMatrix) * movedPosition).xyz;
			mMatrix = modelMatrix;
	

			mvpPos = lightProjectionMatrix * lightViewMatrix * movedPosition;
			gl_Position = projectionMatrix * viewMatrix * movedPosition;
		
		}
	} else {
		sendOsmLastPosFlag = osmLastPosFlag;
		if (osmLastPosFlag == 1.0) {
			


			//make plane on light view
			vec4 planePos;
			vec2 planeMoveVec = vec2(0.0, 0.0);

			if (vertexExpandDirection == 0.0) {
				planeMoveVec = vec2(-0.5, 0.5);
			} else if (vertexExpandDirection == 1.0) {
				planeMoveVec = vec2( 0.5, 0.5);
			} else if (vertexExpandDirection == 2.0) {
				planeMoveVec = vec2( 0.5, -0.5);
			} else if (vertexExpandDirection == 3.0) {
				planeMoveVec = vec2(-0.5, -0.5);
			} 
			 


			
			planePos.x = planeMoveVec.x * dosmLastPlaneSize;
			planePos.y = planeMoveVec.y * dosmLastPlaneSize;
			planePos.z = -dosmLastTextureDepth;//uniform value was positived by preprocess.
			planePos.w = 1.0;

			//move plane to global position
			mat4 invLightViewMat = inverse(lightViewMatrix);		 
			planePos = invLightViewMat * planePos;

			//generaly coordinate translate
			gl_Position = projectionMatrix * viewMatrix * planePos;
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

			mvpPos = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);	
		}
	}
}
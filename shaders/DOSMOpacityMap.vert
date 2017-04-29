#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in float vertexExpandDirection;

uniform float cylinderFlag;
uniform vec3 lightPosition;
uniform mat4 normalMatrix;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float thickness;
uniform float hairFlag;

out vec3 sendNormalVec;
out vec3 sendViewVec;
out vec4 mvpPos;



uniform mat4 vMatrix;
uniform mat4 pMatrix;




void main()
{
	if (hairFlag == 1) {
		if (cylinderFlag == 1) {
			vec3 movedPosition = vertexPosition + normalize(vertexNormal) * thickness;
			mvpPos = projectionMatrix * viewMatrix * modelMatrix * vec4(movedPosition, 1.0);
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(movedPosition, 1.0);
		} else {
			vec3 tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;//global

			vec4 nowGlobalPos = modelMatrix * vec4(vertexPosition, 1.0);
			vec3 viewVec = lightPosition - nowGlobalPos.xyz;
			sendViewVec = viewVec;
			viewVec = normalize(viewVec);

			vec3 binormal = cross(viewVec, tangent);
			sendNormalVec = cross(tangent, binormal);
			
			vec3 offset = binormal * vertexExpandDirection * thickness;

			vec4 movedPosition = vec4(offset, 0.0) + nowGlobalPos;
			

			mvpPos = projectionMatrix * viewMatrix * movedPosition;
			gl_Position = projectionMatrix * viewMatrix * movedPosition;
		}
	} else {
		//距離計算用のベクトル
		mvpPos = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	
		//座標変換
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
		
	}
}
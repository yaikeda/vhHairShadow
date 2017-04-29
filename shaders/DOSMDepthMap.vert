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
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform float thickness;
uniform float hairFlag;

out vec4 mvpPos;

uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{

	if (hairFlag == 1) {
		if (cylinderFlag == 1) {
			vec3 movedPosition = vertexPosition + normalize(vertexNormal) * thickness;
			mvpPos = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(movedPosition, 1.0);
			gl_Position = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(movedPosition, 1.0);
		} else {
			vec3 tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;//global

			vec4 nowGlobalPos = modelMatrix * vec4(vertexPosition, 1.0);
			vec3 viewVec = lightPosition - nowGlobalPos.xyz;
			viewVec = normalize(viewVec);
		
			vec3 binormal = cross(viewVec, tangent);
			
			vec3 offset = binormal * vertexExpandDirection * thickness;

			vec4 movedPosition = vec4(offset, 0.0) + nowGlobalPos;
			
			mvpPos = lightProjectionMatrix * lightViewMatrix * movedPosition;
			gl_Position = lightProjectionMatrix * lightViewMatrix * movedPosition;
		}
	} else {
		mvpPos = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

		gl_Position = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);	
	}

}
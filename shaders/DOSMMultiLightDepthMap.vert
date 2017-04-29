#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;

uniform float cylinderFlag;
uniform float lightPosition;


uniform mat4 modelMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform float thickness;
uniform float hairFlag;

out vec4 mvpPos;

void main()
{
	if (hairFlag == 1) {
			vec3 movedPosition = vertexPosition + normalize(vertexNormal) * thickness;
			mvpPos = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(movedPosition, 1.0);
			gl_Position = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(movedPosition, 1.0);
		} else {
		mvpPos = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
		gl_Position = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	}
}
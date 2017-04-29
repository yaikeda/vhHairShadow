#version 400

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inTangent;
layout (location = 2) in vec3 inMoveDirection;

uniform mat4 uMVP;

out vec4 ioLocalPosition;

void main()
{
	ioLocalPosition = vec4(inPos, 1.0);
	gl_Position = uMVP * vec4(inPos, 1.0);
}
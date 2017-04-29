#version 400

in vec3 inPos;

uniform mat4 uMVP;


out vec4 ioLocalPosition;

void main()
{
	ioLocalPosition = vec4(inPos, 1.0);
	gl_Position = uMVP * vec4(inPos, 1.0);
}
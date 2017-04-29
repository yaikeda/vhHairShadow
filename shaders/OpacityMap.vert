#version 400

layout (location = 0) in vec3 inPos;

uniform mat4 uMVP;

out vec4 ioCenterPos;
out vec4 ioPos;


void main()
{
  ioCenterPos = uMVP * vec4(0.0, 0.0, 0.0, 1.0);
	ioPos = uMVP * vec4(inPos, 1.0);
	gl_Position = ioPos;
}
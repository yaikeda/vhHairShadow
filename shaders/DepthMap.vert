#version 330

layout (location = 0) in vec3 inVertexPos;

uniform mat4 uMVPMtx;

out vec4 oiMVPPos;

void main()
{
	oiMVPPos = uMVPMtx * vec4(inVertexPos, 1.0);
	gl_Position = oiMVPPos;
}
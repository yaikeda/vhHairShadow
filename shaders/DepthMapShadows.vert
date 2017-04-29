#version 330

layout (location = 0) in vec3 inVertexPos;

uniform mat4 uMVPMtx;
uniform mat4 uLightMVPMtx;

out vec4 oiLightViewPos;

void main()
{
  oiLightViewPos = uLightMVPMtx * vec4(inVertexPos, 1.0);
  gl_Position = uMVPMtx * vec4(inVertexPos, 1.0);
} 
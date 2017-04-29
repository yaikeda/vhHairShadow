#version 330

in vec4 oiMVPPos;

out vec4 fragColor;

void main()
{
	vec4 normPos = oiMVPPos / oiMVPPos.w;
	float z = normPos.z;
	fragColor = vec4(z, z, z, 1.0);
}
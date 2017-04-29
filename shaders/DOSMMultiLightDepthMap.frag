#version 400

in vec4 mvpPos;

out vec4 fragColor;
void main(void)
{
	float z = mvpPos.z / mvpPos.w;
	fragColor = vec4(z, z, z, 1.0);
}
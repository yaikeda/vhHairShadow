#version 400

in vec4 mvpPos;

out vec4 fragColor;

void main()
{
	//[“x’l‚ğŒvZ
	float z = mvpPos.z /  mvpPos.w;
	fragColor = vec4(z, z, z, 1.0);
}
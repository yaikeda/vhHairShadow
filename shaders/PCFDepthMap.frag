#version 400

in vec4 mvpPos;

out vec4 fragColor;

void main()
{
	float z = mvpPos.z / mvpPos.w;

	fragColor = vec4(z, z, z, z);
}


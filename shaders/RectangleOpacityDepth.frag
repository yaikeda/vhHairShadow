#version 400

in vec4 ioPos;

out vec4 fragColor;

void main()
{
	float z = ioPos.z / ioPos.w;
    fragColor = vec4(z, z, z, 1.0);
}
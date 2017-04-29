#version 400

in vec4 position;

out vec4 fragColor;

vec4 calcDepthColor(vec4 vFL)
{
	vec4 result;
	float z = vFL.z / vFL.w;
	result = vec4(z, z, z, 1.0);

	return result;
}

void main()
{
	fragColor = calcDepthColor(position);
}


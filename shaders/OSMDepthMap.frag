#version 400

uniform float opacityValue;
out vec4 fragColor;

void main()
{
	fragColor = vec4(opacityValue, opacityValue, opacityValue, opacityValue);
}


#version 400

in vec4 ioPos;
in vec2 ioUV;

uniform sampler2DArray uDepthTex;//深度値マップ
uniform float uOpacityValue; 

out vec4 fragColor;

void main()
{

	

	//受け取った位置を正規化
	vec4 nowPos = ioPos / ioPos.w;
	float z = nowPos.z;


	vec2 texPos = (nowPos.xy + 1.0)*0.5;

	float weight = (1.0 - distance(ioUV, vec2(1.0, 1.0)));
	weight = clamp(weight, 0.0, 1.0);
	float opacityValue = uOpacityValue * weight;

  
	//深度値取得
	vec4 texColor = texture(uDepthTex, vec3(texPos, 0));
	float near = texColor.x;


	//fragColor = texColor * vec4(0.0, 1.0, 1.0, 0.0);
	//return;


	//レイヤを置く位置を決定
	float base = (1.0 - near) / 4.0;
	float layerDistance[4] = float[4](0.0, 0.0, 0.0, 0.0);//透明度記録位置数は4に固定
    layerDistance[0] = near + base;
    layerDistance[1] = layerDistance[0] + base;
    layerDistance[2] = layerDistance[1] + base;
    layerDistance[3] = layerDistance[2] + base;

	float opacityMap[4] = float[4](0.0, 0.0, 0.0, 0.0);
	if (z < layerDistance[0]) {opacityMap[0] = opacityValue;}
	if (z < layerDistance[1]) {opacityMap[1] = opacityValue;}
	if (z < layerDistance[2]) {opacityMap[2] = opacityValue;}
	if (z < layerDistance[3]) {opacityMap[3] = opacityValue;}

 
	fragColor = vec4(opacityMap[0], opacityMap[1], opacityMap[2], opacityMap[3]);
}
#version 400

in vec4 ioCenterPos;
in vec4 ioPos;


uniform sampler2DArray uDepthTex;//深度値マップ
uniform float uOpacityValue; 

out vec4 fragColor;

void main()
{
	//受け取った位置を正規化
	vec4 nowPos = ioPos / ioPos.w;
	float z = nowPos.z;

  //現在位置から中心点との距離を計算, 不透明度を調整
  
  
  
  vec4 nowCenterPos = ioCenterPos / ioCenterPos.w;
  float centerlength = length(nowPos.xyz - nowCenterPos.xyz) * 8;

  float opacityValue = uOpacityValue * centerlength;
  

	//深度値取得
	vec4 texColor = texture(uDepthTex, vec3((nowPos.xy + 1.0) * 0.5, 0));
	float near = texColor.x;

	//レイヤを置く位置を決定
	float base = (1.0 - near) / 4;
	float layerDistance[4] = float[4](0.0, 0.0, 0.0, 0.0);//透明度記録位置数は4に固定
    layerDistance[0] = near + base;
    layerDistance[1] = layerDistance[0] + base;
    layerDistance[2] = layerDistance[1] + base;
    layerDistance[3] = layerDistance[2] + base;

	//値の記録

	float opacityMap[4] = float[4](0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 4; i++) {
		if (z < layerDistance[i]) {
			opacityMap[i] = uOpacityValue;
     // opacityMap[i] = opacityValue;
		}
	}



	fragColor = vec4(opacityMap[0], opacityMap[1], opacityMap[2], opacityMap[3]);
	

}
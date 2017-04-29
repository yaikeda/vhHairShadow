#version 400

in vec4 mvpPos;

uniform float revisionFlag;

uniform sampler2DArray depthTexture;
uniform float nowLightNumber;

uniform float opacityValue;
uniform float layerInterval;
//uniform float textureNear;//現在注目しているテクスチャの位置 0~1
//uniform float textureFar;//現在注目中の次のテクスチャの位置 0~1
uniform float dosmLastPos;//一枚のテクスチャの割り当てられた距離
uniform float nowTextureNum;//現在のテクスチャの番号０～
uniform float allTextureNum;//すべてのテクスチャ枚数

in vec3 sendViewVec;
in vec3 sendNormalVec;
out vec4 fragColor;

void main()
{	
	vec3 viewVec = normalize(sendViewVec);
	vec3 normalVec = normalize(sendNormalVec);
	float opacityRevision = abs(dot(viewVec, normalVec));


	//c_  means "const" dont change the value.
	const int mapNum = 4;//一枚のテクスチャに何枚のレイヤが含まれるか。
	
	vec4 nowPos = mvpPos / mvpPos.w;//-1.0~1.0
	
	//ライトから最も近い点の距離
	vec4 texColor = texture(depthTexture, vec3((nowPos.xy + 1) * 0.5, nowLightNumber));
	float c_near = texColor.x;//-1.0~1.0 画面に見えたときの視覚的には黒から白なのでわかりにくいが, 実際は-1.0 ~ 1.0の間で推移する。

	//prepare layer distance
	float layerDistance[mapNum];
	float allLayer = 2 * allTextureNum * (1 + 4 * allTextureNum);
	float c_base = (1.0 - c_near) / allLayer;//farからnearまでの距離
	float nowTextureStart = 0;
	if (nowTextureNum <= 0) {
		nowTextureStart = c_near;
	} else {
		nowTextureStart = c_near + c_base * (2 * (nowTextureNum) * (1 + 4 * (nowTextureNum)));
	}

	for (int i = 0; i < mapNum; i++) {
		if (i == 0) {
			layerDistance[i] = nowTextureStart;
		} else {
			layerDistance[i] = layerDistance[i-1];
		}
		layerDistance[i] += nowTextureNum * 4 * c_base + c_base * (i + 1);
	}



	float opacityMap[mapNum] = float[mapNum](0.0, 0.0, 0.0, 0.0);

	if (revisionFlag == 0) {
		for (int i = 0; i < mapNum; i++) {
			if (nowPos.z < layerDistance[i]) {
				opacityMap[i] = opacityValue;
			}
		}
	} else {
		for (int i = 0; i < mapNum; i++) {
			if (nowPos.z < layerDistance[i]) {
				opacityMap[i] = opacityValue * opacityRevision;
			}
		}
	}
	
	fragColor = vec4(opacityMap[0], opacityMap[1], opacityMap[2], opacityMap[3]);
}


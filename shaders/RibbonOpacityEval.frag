#version 400

in vec4 ioLocalPosition;

uniform mat4 uLightMVP[20];
uniform float uOffset[20];
uniform vec3 uShadowColor[20];
uniform int uMatrixNum;
uniform sampler2DArray uDepthTex;
uniform sampler2DArray uOpacityTex;
uniform int uMapResolution;

out vec4 fragColor;



vec3 GetOpacity(int index, vec3 texPos, float depth)
{
	//深度値取得
	vec4 depthTexColor = texture(uDepthTex, texPos);
	float near = depthTexColor.x;
	
	//レイヤを置く位置を決定
	float base = (1.0 - near) / 4;
	float layerDistance[4];//透明度記録位置数は4に固定
    layerDistance[0] = near + base;
    layerDistance[1] = layerDistance[0] + base;
    layerDistance[2] = layerDistance[1] + base;
    layerDistance[3] = layerDistance[2] + base;


	//オパシティ値取得
	vec4 opacityTexColor = texture(uOpacityTex, texPos);
  
  
	float opacityValue[4];
  opacityValue[0] = opacityTexColor.r;
	opacityValue[1] = opacityTexColor.g;
	opacityValue[2] = opacityTexColor.b;
	opacityValue[3] = opacityTexColor.a;
  
    if (depth > 1.0)
    {
        return vec3(opacityValue[3]);
    }
    if(depth<layerDistance[0])
    {
        float alpha = max(0.0,(1.0-(layerDistance[0]-depth)/(layerDistance[0]-near)));
        alpha = min(alpha, 1.0);
        alpha = max(alpha, 0.0);
        float opacity = alpha * opacityValue[0];
        return vec3(opacity);
    }
    {
	float opacity = 0.0;
        for(int i=1;i<=3;i++) {
            if(depth<layerDistance[i])
            {
                float alpha = (1.0-(layerDistance[i] - depth)/(layerDistance[i]-layerDistance[i-1]));
                alpha = min(alpha, 1.0);
                alpha = max(alpha, 0.0);

                opacity = (1-alpha) * opacityValue[i-1] + alpha * opacityValue[i];
            }
        }
	return vec3(opacity);
    }

}

vec4 GetMVPPos(mat4 matrix)
{
    vec4 pos;

    pos = matrix * ioLocalPosition;
	
    return pos;
}


vec3 ComputeOpacity(int index, float sampleSize)
{
	

    vec4 nowPos = GetMVPPos(uLightMVP[index]);
    nowPos = nowPos / nowPos.w;

//return nowPos.xyz;

	float depth = nowPos.z + uOffset[index];
	if (nowPos.z < 0.0) {//クリッピング空間よりも手前にあったら
		return vec3(0.0);
	}

    vec3 texPos = vec3((nowPos.xy + 1.0) * 0.5, index);
	float d = length((texPos.xy - vec2(0.5)));
	if (d > 0.5) {//ライトが当たっていない
		return vec3(0.0);
	}
	


    //for文でまわす
    vec2 delta = vec2(1.0/float(uMapResolution));//texture coordinate変化量




	vec3 opacityValue = vec3(0.0);
	float normValue = 0.0;

   
	for (float i = -sampleSize; i <= sampleSize; i+=1.0) {
		for (float j = -sampleSize; j <= sampleSize; j+=1.0) {
			vec3 samplePos = texPos+vec3(vec2(i,j)*delta,0);
            vec3 color = GetOpacity(index, samplePos, depth); 
            
			opacityValue += color;
			normValue += 1.0;
		}
	}




	normValue = 1.0 / normValue;

	opacityValue = opacityValue * vec3(normValue);

    opacityValue = min(vec3(1.0), opacityValue);
    opacityValue = max(vec3(0.0), opacityValue);

	return opacityValue;
}


void main()
{

    vec3 L = vec3(1.0);

	for (int index = 0; index < uMatrixNum; index++) {
        vec3 buffLightColor = vec3(1.0) - (uShadowColor[index] * ComputeOpacity(index, 0.0));
        L *= buffLightColor;
	}
	
	
		
    L = max(L, vec3(0.0));
    L = min(L, vec3(1.0));
    


	fragColor = vec4(L, 1.0);
    
}


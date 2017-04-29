#version 330

in vec4 oiLightViewPos;
uniform sampler2DArray texDepthMap;
uniform float uOffset;

out vec4 fragColor;

void main() {

  vec4 nowPos = oiLightViewPos / oiLightViewPos.w;
  vec2 texPos = (nowPos.xy + 1.0) * 0.5;
	
  
	if (nowPos.z < 0.0) {
		fragColor = vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}
  

  vec4 texColor = texture(texDepthMap, vec3(texPos, 0));
	
  
 
  if ( texColor.r - uOffset <= nowPos.z) {
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
 
}
#ifndef __AKGRENDERLINEOPACITYEVALUATE_H__
#define __AKGRENDERLINEOPACITYEVALUATE_H__

#include "AkgTypeDef.h"
#include "GL/glew.h"
#include <vector>

class AkgRenderLineOpacityEvaluate {
public:
  AkgRenderLineOpacityEvaluate(void);
  ~AkgRenderLineOpacityEvaluate(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture, float lightMVPMtx[], t_CameraInfo cameraInfo);

private:
};
#endif
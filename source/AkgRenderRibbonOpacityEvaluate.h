#ifndef __AKGRENDERRIBBONOPACITYEVALUATE_H__
#define __AKGRENDERRIBBONOPACITYEVALUATE_H__

#include "AkgTypeDef.h"
#include "GL/glew.h"
#include <vector>

class AkgRenderRibbonOpacityEvaluate {
public:
  AkgRenderRibbonOpacityEvaluate(void);
  ~AkgRenderRibbonOpacityEvaluate(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture, float lightVPMtx[], t_CameraInfo cameraInfo);

private:
};
#endif
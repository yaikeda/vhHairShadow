#ifndef __AKGRENDERRECTANGLEOPACITYEVALUATE_H__
#define __AKGRENDERRECTANGLEOPACITYEVALUATE_H__

#include "AkgTypeDef.h"
#include "GL/glew.h"
#include <vector>

class AkgRenderRectangleOpacityEvaluate {
public:
  AkgRenderRectangleOpacityEvaluate(void);
  ~AkgRenderRectangleOpacityEvaluate(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture, float lightVPMtx[], t_CameraInfo cameraInfo);

private:
};
#endif
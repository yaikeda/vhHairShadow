#ifndef __AKGRENDERTRIANGLEOPACITYEVALUATE_H__
#define __AKGRENDERTRIANGLEOPACITYEVALUATE_H__

#include "AkgTypeDef.h"
#include "GL/glew.h"
#include <vector>

class AkgRenderTriangleOpacityEvaluate {
public:
  AkgRenderTriangleOpacityEvaluate(void);
  ~AkgRenderTriangleOpacityEvaluate(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture, float lightVPMtx[], t_CameraInfo cameraInfo);

private:
};
#endif
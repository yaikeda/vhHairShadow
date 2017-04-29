#ifndef __AKGRENDEROPACITYEVALUATE_H__
#define __AKGRENDEROPACITYEVALUATE_H__

#include "AkgTypeDef.h"
#include "GL/glew.h"
#include <vector>

class AkgRenderOpacityEvaluate {
 public:
  AkgRenderOpacityEvaluate(void);
  ~AkgRenderOpacityEvaluate(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo, 
    std::vector<t_TextureInfo> texture, float lightMVPMtx[], t_CameraInfo cameraInfo);
 
 private:
};
#endif
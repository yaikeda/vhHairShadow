#ifndef __AKGRENDERDEPTHMAPSHADOWS_H__
#define __AKGRENDERDEPTHMAPSHADOWS_H__

#include "AkgTypeDef.h"
#include "GL/glew.h"
#include <vector>

class AkgRenderDepthMapShadows {
 public:
  AkgRenderDepthMapShadows(void);
  ~AkgRenderDepthMapShadows(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo, 
    std::vector<t_TextureInfo> texture, float lightMVPMtx[], t_CameraInfo cameraInfo);
 private:
};
#endif
#ifndef __AKGRENDERRECTANGLEOPACITYMAP_H__
#define __AKGRENDERRECTANGLEOPACITYMAP_H__

#include "AkgTypeDef.h"
#include <vector>

class AkgRenderRectangleOpacityMap {
public:
  AkgRenderRectangleOpacityMap(void);
  ~AkgRenderRectangleOpacityMap(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture,
    std::vector<t_LightInfo>* lightInfo,
    float lightVPMtx[],
    t_CameraInfo cameraInfo);

protected:

private:
};
#endif
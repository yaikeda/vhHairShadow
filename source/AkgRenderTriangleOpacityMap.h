#ifndef __AKGRENDERTRIANGLEOPACITYMAP_H__
#define __AKGRENDERTRIANGLEOPACITYMAP_H__

#include "AkgTypeDef.h"
#include <vector>

class AkgRenderTriangleOpacityMap {
public:
  AkgRenderTriangleOpacityMap(void);
  ~AkgRenderTriangleOpacityMap(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture,
    std::vector<t_LightInfo>* lightInfo,
    float lightVPMtx[]);

protected:

private:
};
#endif
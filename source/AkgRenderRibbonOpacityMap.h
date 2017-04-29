#ifndef __AKGRENDERRIBBONOPACITYMAP_H__
#define __AKGRENDERRIBBONOPACITYMAP_H__

#include "AkgTypeDef.h"
#include <vector>

class AkgRenderRibbonOpacityMap {
public:
  AkgRenderRibbonOpacityMap(void);
  ~AkgRenderRibbonOpacityMap(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture,
    std::vector<t_LightInfo>* lightInfo,
    float lightVPMtx[]);

protected:

private:
};
#endif
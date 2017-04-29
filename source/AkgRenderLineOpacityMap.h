#ifndef __AKGRENDERLINEOPACITYMAP_H__
#define __AKGRENDERLINEOPACITYMAP_H__

#include "AkgTypeDef.h"
#include <vector>

class AkgRenderLineOpacityMap {
public:
  AkgRenderLineOpacityMap(void);
  ~AkgRenderLineOpacityMap(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture, float lightMVPMtx[]);

protected:

private:
};
#endif
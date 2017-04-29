#ifndef __AKGRENDEROPACITYMAP_H__
#define __AKGRENDEROPACITYMAP_H__

#include "AkgTypeDef.h"
#include <vector>

class AkgRenderOpacityMap {
 public:
  AkgRenderOpacityMap(void);
  ~AkgRenderOpacityMap(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo, 
    std::vector<t_TextureInfo> texture, float lightMVPMtx[]);
 
 protected:

 private:
};
#endif
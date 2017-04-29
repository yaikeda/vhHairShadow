#ifndef __AKGRENDERLINEOPACITYDEPTH_H__
#define __AKGRENDERLINEOPACITYDEPTH_H__

#include "AkgTypeDef.h"
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class AkgRenderLineOpacityDepth {
public:
  AkgRenderLineOpacityDepth(void);
  ~AkgRenderLineOpacityDepth(void);
  void Render(GLuint program, std::vector<t_ObjectInfo> objectInfo,
    std::vector<t_TextureInfo> texture, glm::mat4* lightMVPMtx, std::vector<t_LightInfo> lightInfo);

protected:
  void GetClippingFromLight(float nearFar[], glm::mat4 modelMtx, glm::mat4 viewMtx, std::vector<t_ObjectInfo> objectInfo);

private:
};
#endif
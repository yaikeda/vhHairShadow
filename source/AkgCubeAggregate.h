#ifndef __AKGCUBEAGGREGATE_H__
#define __AKGCUBEAGGREGATE_H__

#include <vector>

#include "AkgObjects.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class AkgCubeAggregate : public AkgObjects {
 public:
  void Create(t_ObjectInfo* objectInfo);
 protected:
  void GetBoundingBox(float vert[][3]);
 private:
  std::vector<unsigned int> m_Index;
  float m_BBox[8][3];
};
#endif
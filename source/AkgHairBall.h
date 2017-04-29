#ifndef __AKGHAIRBALL_H__
#define __AKGHAIRBALL_H__

#include <vector>

#include "AkgObjects.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class AkgHairBall : public AkgObjects {
 public:
  void Create(t_ObjectInfo* objectInfo);
  void GetClipping(float nearFar[], glm::mat4 modelMtx, glm::mat4 viewMtx);
protected:
  void GetBoundingBox(std::vector<float>* vert);
 private:
  std::vector<unsigned int> m_Index;
  float m_BBox[8][3];
};

#endif
#ifndef __AKGPLANE_H__
#define __AKGPLANE_H__

#include <vector>

#include "AkgTypeDef.h"
#include "AkgObjects.h"

class AkgPlane : public AkgObjects {
 public:
  void Create(t_ObjectInfo* objectInfo);
 private:
  std::vector<unsigned int> m_Index;
};

#endif
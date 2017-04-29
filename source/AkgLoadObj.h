#ifndef __AKGLOADOBJ_H__
#define __AKGLOADOBJ_H__

#include <vector>

class AkgLoadObj {
 public:
  void Load(const char* fileName, 
    std::vector<float>& vert, std::vector<unsigned int>& index);
 private:
};
#endif
#ifndef __AKGMOUSE_H__
#define __AKGMOUSE_H__

#include "AkgGeneralManager.h"

class AkgMouse {
 public:
  void Exec(AkgGeneralManager* obj, int button, int state, int x, int y);
 private:
};
#endif
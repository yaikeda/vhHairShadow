#include "AkgMouse.h"

void AkgMouse::Exec(AkgGeneralManager* obj, int button, int state, int x, int y)
{
  switch (state) {
  case GLUT_DOWN:
    {      
      int motionStart[] = {x, y};
      obj->SetMotionStart(motionStart);
    }
    break;
  case GLUT_UP:
    {

    }
    break;
  }
}
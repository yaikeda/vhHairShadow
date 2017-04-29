#include "AkgReshape.h"

void AkgReshape::Exec(AkgGeneralManager* obj, int w, int h)
{
  obj->SetWindowSize(w, h);
  
  float angleAspectNearFar[4];

  obj->GetCamera(NULL, NULL, NULL, angleAspectNearFar);

  angleAspectNearFar[1] = (float)w / (float)h;

  obj->SetCamera(NULL, NULL, NULL, angleAspectNearFar);

}
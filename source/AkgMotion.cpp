#include "AkgMotion.h"

#include "AkgMouse.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void AkgMotion::Exec(AkgGeneralManager* obj, int x, int y)
{
/*設計方針
  設計結果として，マウスで，注目したところにカメラを向けたい
  変更する物
    カメラの見る場所
  必要な計算材料
    クリックした位置
    クリックした位置からの変化量
    離した位置
*/
  float look[3];
  obj->GetCamera(NULL, look, NULL, NULL); 

  int* motionStart = obj->GetMotionStart();

  look[0] =  (float)-x + motionStart[0];
  look[1] = (float)-y + motionStart[1]; 

  //std::cout << look[0] << " " << look[1] << std::endl;

  obj->SetCamera(NULL, look, NULL, NULL);

  glutPostRedisplay();
}
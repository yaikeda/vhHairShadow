#include "AkgKeyboard.h"

#include "AkgIdle.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

void AkgKeyboard::exec(AkgGeneralManager* obj, unsigned char key, int x, int y)
{
  switch (key) {
  case 'a':
    {
            //init camera pos.
            //カメラ位置を初期状態に戻す

            float look[3] = { 0.0f, 0.0f, 0.0f };
            float angleAspectNearFar[4];

            obj->GetCamera(NULL, look, NULL, angleAspectNearFar);

            look[0] = 272;
            look[1] = 119;
            angleAspectNearFar[0] = 29.0f;

            obj->SetCamera(NULL, look, NULL, angleAspectNearFar);

            std::cout << "カメラ位置 初期化" << std::endl;

    }
    break;
  case 'i':
  case 'I':
    {
    //init camera pos.
    //カメラ位置を初期状態に戻す
    
    float pos[3] = { 282.0f, 150.0f, -4.93f };
    //float look[3] = { 275.0f, 143.0f, 0.0f };
    float look[3] = { 0.0f, 0.0f, 0.0f };
    float up[3] = { 0.0f, 1.0f, 0.0f };
    float angleAspectNearFar[4] = { 110.0f, 1.0f, 1.0f, 5000.0f };

    obj->SetCamera(pos, look, up, angleAspectNearFar); 
  
    std::cout << "カメラ位置 初期化" << std::endl;
    
    }
    break;
/*
  case 'j':
    dataSet.setHairThickness_Rendering(dataSet.getHairThickness_Rendering() + 0.03);
    std::cout << "thickness(Rendering): " << dataSet.getHairThickness_Rendering() << std::endl;
    break;
  case 'J':
    dataSet.setHairThickness_Rendering(dataSet.getHairThickness_Rendering() - 0.03);
    printf("thickness(Rendering): %f\n", dataSet.getHairThickness_Rendering());
    break;
  case 'k':
    dataSet.setHairThickness_Opacity(dataSet.getHairThickness_Opacity() + 0.03);
    printf("thickness(Opacity): %f\n", dataSet.getHairThickness_Opacity());
    break;
  case 'K':
    dataSet.setHairThickness_Opacity(dataSet.getHairThickness_Opacity() - 0.03);
    printf("thickness(Opacity): %f\n", dataSet.getHairThickness_Opacity());
    break;
 */
  case 'm':
  case 'M':
    if (!obj->GetAnimationFlag()) {
      //アニメーションon
      obj->SetAnimationFlag(true);
      glutIdleFunc(AkgIdle::Exec);
      fprintf(stderr, "アニメーションオン\n");
    } else {
      //アニメーションoff
      obj->SetAnimationFlag(false);
      glutIdleFunc(0);
      fprintf(stderr, "アニメーションオフ\n");
    }
    break;

  
  case 'o':
  {
    char* name[5] = {
      "HairBall",
      "LineHairBall",
      "RibbonHairBall",
      "TriangleHairBall",
      "RectangleHairBall"};
    for (int i = 0; i < 5; i++) {
      float value = obj->GetObjectOpacityvalue(name[i]) + 0.001f;
      obj->SetObjectOpacityValue(name[i], value);

      std::cout << name[i] << "'s opacity valure change: " << value << std::endl;
    }
  }
    break;
  case 'O':
    {
    char* name[5] = {
      "HairBall",
      "LineHairBall",
      "RibbonHairBall",
      "TriangleHairBall",
      "RectangleHairBall"};
    for (int i = 0; i < 5; i++) {
      float value = obj->GetObjectOpacityvalue(name[i]) - 0.001f;
      obj->SetObjectOpacityValue(name[i], value);

      std::cout << name[i] << "'s opacity valure change: " << value << std::endl;
    }
    
    }
    break;
    /*
  case 'p':
  case 'P':
    if (dataSet.getPcfFlag() == 0) {
      dataSet.setPcfFlag(1);
      fprintf(stderr, "pcf(ぼかし)オン\n");
    }
    else if (dataSet.getPcfFlag() == 1) {
      dataSet.setPcfFlag(0);
      fprintf(stderr, "pcf(ぼかし)オフ\n");
    }

    break;
  */

  
  case 'r':
    {
    //カメラ位置の回転
    //cameraPosの変更
    float pos[3];

    obj->GetCamera(pos, NULL, NULL, NULL);

    glm::vec3 vPos = glm::make_vec3(pos);
    glm::vec4 newPos = glm::rotate<float>(
      glm::mat4(1.0f), 15, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(vPos, 1.0);

    memcpy(pos, &newPos[0], sizeof(float) * 3);
    obj->SetCamera(pos, NULL, NULL, NULL);

    }
    break;
    
  case 'R':
    {
    //カメラ位置の回転
    //cameraPosの変更
    float pos[3];

    obj->GetCamera(pos, NULL, NULL, NULL);

    glm::vec3 vPos = glm::make_vec3(pos);
    glm::vec4 newPos = glm::rotate<float>(
      glm::mat4(1.0f), -15, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(vPos, 1.0);

    memcpy(pos, &newPos[0], sizeof(float) * 3);

    obj->SetCamera(pos, NULL, NULL, NULL);
    }
    break;
    

  case 's':
    {
    //回転速度加速
    float speed = obj->GetRotationSpeed() * 1.5f;
    obj->SetRotationSpeed(speed);
    std::cout << "rotation speed up " << speed << std::endl;
    }
    break;
  case 'S':
    {
    //回転速度減速
    float speed = obj->GetRotationSpeed() * 0.5f;
    obj->SetRotationSpeed(speed);
    std::cout << "rotation speed down " << speed << std::endl;
    }
    break;
  case 't':
  {
    char* name[5] = {
      "HairBall",
      "LineHairBall",
      "RibbonHairBall",
      "TriangleHairBall",
      "RectangleHairBall"};
    for (int i = 0; i < 5; i++) {
      float value = obj->GetObjectThickness(name[i]) + 0.01f;
      obj->SetObjectThickness(name[i], value);

      std::cout << name[i] << "'s thickness change: " << value << std::endl;
    }
  }
  break;
  case 'T':
  {
    char* name[5] = {
      "HairBall",
      "LineHairBall",
      "RibbonHairBall",
      "TriangleHairBall",
      "RectangleHairBall"};
    for (int i = 0; i < 5; i++) {
      float value = obj->GetObjectThickness(name[i]) - 0.01f;
      obj->SetObjectThickness(name[i], value);

      std::cout << name[i] << "'s thickness change: " << value << std::endl;
    }

  }
  break;

/*
  case 'y':
  case 'Y':
    if (dataSet.getReferModeFlag() == 1) { 
      dataSet.setReferModeFlag(0); 
      fprintf(stderr, "normal calc mode\n");
    }
    else { 
      dataSet.setReferModeFlag(1); 
      fprintf(stderr, "refer mode\n");
    }
    break;

    */
  case 'z':
    {
    //zoom
    float pos[3];

    obj->GetCamera(pos, NULL, NULL, NULL);

    glm::mat4 scale = glm::scale<float>(glm::mat4(1.0f), glm::vec3(0.99f, 0.99f, 0.99f));

    glm::vec4 vpos = glm::vec4(pos[0], pos[1], pos[2], 1.0);

    vpos = scale * vpos;
    memcpy(pos, &vpos[0], sizeof(float)*3);

    obj->SetCamera(pos, NULL, NULL, NULL);
    }
    break;
  case 'Z':
    {
    //zoom
    float pos[3];

    obj->GetCamera(pos, NULL, NULL, NULL);

    glm::mat4 scale = glm::scale<float>(glm::mat4(1.0f), glm::vec3(1.01f, 1.01f, 1.01f));
    glm::vec4 vpos = glm::vec4(pos[0], pos[1], pos[2], 1.0);

    vpos = scale * vpos;
    memcpy(pos, &vpos[0], sizeof(float) * 3);

    obj->SetCamera(pos, NULL, NULL, NULL);
    }
    break;
  default:
    break;
  }



  glutPostRedisplay();
}
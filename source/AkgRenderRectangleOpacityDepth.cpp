#include "AkgRenderRectangleOpacityDepth.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include "AkgHairBall.h"

#include <iostream>
#include <algorithm>


AkgRenderRectangleOpacityDepth::AkgRenderRectangleOpacityDepth(void)
{}

AkgRenderRectangleOpacityDepth::~AkgRenderRectangleOpacityDepth(void)
{}

void AkgRenderRectangleOpacityDepth::Render(
  GLuint program,
  std::vector<t_ObjectInfo> objectInfo,
  std::vector<t_TextureInfo> texture,
  glm::mat4* lightMVPMtx,
  std::vector<t_LightInfo>* lightInfo
  )
{
  if (objectInfo.size() >= 2) {
    std::cout << "RectangleHairBallのデータを使用します" << std::endl;
  }

  glUseProgram(program);

  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  //make matrix////////////////////////////////////////////////////////////////
  t_ObjectInfo oi, normalHairBall;
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    if (strcmp(objectInfo[i].name, "RectangleHairBall") == 0) {
      oi = objectInfo[i];
    }
    //if (strcmp(objectInfo[i].name, "LineHairBall") == 0) {
    //  oi = objectInfo[i];
    //}
    //if (strcmp(objectInfo[i].name, "HairBall") == 0) {
    //  oi = objectInfo[i];
    //}
  }
  glm::mat4 modelMtx = glm::make_mat4(oi.modelMatrix);
  glm::mat4 viewMtx = glm::lookAt(
    glm::make_vec3(lightInfo->at(0).pos),
    glm::make_vec3(lightInfo->at(0).look),
    glm::make_vec3(lightInfo->at(0).up));
  

  float nearFar[2] = { FLT_MAX, -FLT_MAX };

  //hairBall.GetClipping(nearFar, modelMtx, viewMtx);
  GetClippingFromLight(nearFar, modelMtx, viewMtx, objectInfo);
  //glm::mat4 projMtx = glm::perspective<float>(80, 1.0f, nearFar[0], nearFar[1]);
  //glm::mat4 projMtx = glm::perspective<float>(80, 1.0f, nearFar[0], nearFar[1]);
  glm::mat4 projMtx = glm::perspective<float>(80, 1.0f, 30.0, nearFar[1]);


  glm::mat4 vpMtx = projMtx * viewMtx;
  glm::mat4 mvMtx = viewMtx * modelMtx;

  //glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMtx));

  
  int location = glGetUniformLocation(program, "uMV");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &mvMtx[0][0]);
  } else {
    std::cout << "uMV error" << std::endl;
  }

  location = glGetUniformLocation(program, "uP");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &projMtx[0][0]);
  } else {
    std::cout << "uP error" << std::endl;
  }

  /*
  location = glGetUniformLocation(program, "uNormalMatrix");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &normalMatrix[0][0]);
  } else {
    std::cout << "uNormalMatrix error" << std::endl;
  }
  */

  
  location = glGetUniformLocation(program, "uThickness");
  if (location >= 0) {
    glUniform1fv(location, 1, &oi.thickness);
  } else {
    std::cout << "uThickness" << std::endl;
  }
  

  memcpy(lightInfo->at(0).viewMatrix, &viewMtx[0][0], sizeof(float)* 16);
  memcpy(lightInfo->at(0).projMatrix, &projMtx[0][0], sizeof(float)* 16);

  /*
  location = glGetUniformLocation(program, "uLightPos");
  if (location >= 0) {
    glUniform3fv(location, 1, lightInfo->at(0).pos);
  } else {
    std::cout << "uLightPos error" << std::endl;
  }
  */
  
  t_TextureInfo texInfo;
  for (unsigned int i = 0; i < texture.size(); i++) {
    if (strcmp(texture[i].name, "DepthTexture") == 0) {
      texInfo = texture[i];
      break;
    }
  }
  
  
  glBindFramebuffer(GL_FRAMEBUFFER, texInfo.fbo);
  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texInfo.texture, 0, 0);

  glClearDepth(1.0);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    glBindVertexArray(objectInfo[i].handle);

    if (strcmp(objectInfo[i].name, "LineHairBall") == 0) {
      glDrawElements(GL_LINE_LOOP, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
    }
    else {
      glDrawElements(GL_QUADS, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
    }
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  
  
  glUseProgram(0);

  *lightMVPMtx = projMtx * viewMtx;
}

//protected
void AkgRenderRectangleOpacityDepth::GetClippingFromLight(
  float nearFar[],
  glm::mat4 modelMtx,
  glm::mat4 viewMtx,
  std::vector<t_ObjectInfo> objectInfo)
{
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    for (unsigned int j = 0; j < 8; j++) {
      float base[3];
      memcpy(base, objectInfo[i].bbox[j], sizeof(float)* 3);
      glm::vec4 point = glm::vec4(base[0], base[1], base[2], 1.0);
      point = viewMtx * modelMtx * point;
      float z = -point.z;

      nearFar[0] = std::min<float>(z, nearFar[0]);
      nearFar[1] = std::max<float>(z, nearFar[1]);
    }
  }
}

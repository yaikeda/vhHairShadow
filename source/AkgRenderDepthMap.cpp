#include "AkgRenderDepthMap.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include <iostream>
#include <algorithm>


AkgRenderDepthMap::AkgRenderDepthMap(void)
{}

AkgRenderDepthMap::~AkgRenderDepthMap(void)
{}

void AkgRenderDepthMap::Render(
  GLuint program, 
  std::vector<t_ObjectInfo> objectInfo,
  std::vector<t_TextureInfo> texture, 
  glm::mat4* lightMVPMtx,
  std::vector<t_LightInfo> lightInfo
  )
{
  if (objectInfo.size() >= 2) {
    std::cout << "HairBallのデータを使用します" << std::endl;
  }

  glUseProgram(program);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  //make matrix////////////////////////////////////////////////////////////////
  t_ObjectInfo oi;
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    if (strcmp(objectInfo[i].name, "LineHairBall") == 0) {
      oi = objectInfo[i];
      break;
    }
  }
  glm::mat4 modelMtx = glm::make_mat4(oi.modelMatrix); 
  glm::mat4 viewMtx = glm::lookAt(
    glm::make_vec3(lightInfo[0].pos), 
    glm::make_vec3(lightInfo[0].look), 
    glm::make_vec3(lightInfo[0].up));

  float nearFar[2] = {FLT_MAX, -FLT_MAX};
  GetClippingFromLight(nearFar, modelMtx, viewMtx, objectInfo);

  glm::mat4 projMtx = glm::perspective<float>(80, 1.0f, 30, 10000);
  
  glm::mat4 MVPMtx = projMtx * viewMtx * modelMtx;

  int location = glGetUniformLocation(program, "uMVPMtx");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &MVPMtx[0][0]);
  }
  

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
    glDrawElements(GL_LINE_LOOP, 
      objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
  }

  glBindFramebuffer(GL_FRAMEBUFFER , 0);  
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glUseProgram(0);

  *lightMVPMtx = projMtx * viewMtx;
  
}

//protected
void AkgRenderDepthMap::GetClippingFromLight(
  float nearFar[],
  glm::mat4 modelMtx, 
  glm::mat4 viewMtx, 
  std::vector<t_ObjectInfo> objectInfo)
{  
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    for (unsigned int j = 0; j < 8; j++) {
      float base[3];
      memcpy(base, objectInfo[i].bbox[j], sizeof(float) * 3);
      glm::vec4 point = glm::vec4(base[0], base[1], base[2], 1.0);
      point = viewMtx * modelMtx * point;
      float z = -point.z;

      nearFar[0] = std::min<float>(z, nearFar[0]);
      nearFar[1] = std::max<float>(z, nearFar[1]);
    }
  }
}
 
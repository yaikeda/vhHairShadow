#include "AkgRenderRibbonOpacityMap.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

AkgRenderRibbonOpacityMap::AkgRenderRibbonOpacityMap(void)
{}
AkgRenderRibbonOpacityMap::~AkgRenderRibbonOpacityMap(void)
{}
void AkgRenderRibbonOpacityMap::Render(
  GLuint program, 
  std::vector<t_ObjectInfo> objectInfo, 
  std::vector<t_TextureInfo> texture,
  std::vector<t_LightInfo>* lightInfo,
  float lightVPMtx[])
{
  glUseProgram(program);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE, GL_ONE);
  glEnable(GL_BLEND);

  /////////////////////////////////////////////////////////////////////////////
  //send uniform values



  //fragment shader
  t_ObjectInfo oi;
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    if (strcmp(objectInfo[i].name, "RibbonHairBall") == 0) {
      oi = objectInfo[i];
      break;
    }
  }

  glm::mat4 modelMtx = glm::make_mat4(oi.modelMatrix);
  glm::mat4 viewMtx = glm::make_mat4(lightInfo->at(0).viewMatrix);
  glm::mat4 projMtx = glm::make_mat4(lightInfo->at(0).projMatrix);


  glm::mat4 mvMtx = viewMtx * modelMtx;
  glm::mat4 vpMtx = projMtx * viewMtx;
  glm::mat4 mvpMtx = projMtx * mvMtx;
  glm::mat4 normalMtx = glm::transpose(glm::inverse(modelMtx));

  //vertex shader
  int location = glGetUniformLocation(program, "uM");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &modelMtx[0][0]);
  } else {
    std::cout << "uM error" << std::endl;
  }

  location = glGetUniformLocation(program, "uVP");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &vpMtx[0][0]);
  } else {
    std::cout << "uP error" << std::endl;
  }

  location = glGetUniformLocation(program, "uNormalMatrix");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &normalMtx[0][0]);
  } else {
    std::cout << "uNormalMatrix error" << std::endl;
  }
  
  location = glGetUniformLocation(program, "uLightPos");
  if (location >= 0) {
    glUniform3fv(location, 1, lightInfo->at(0).pos);
  }
  else {
    std::cout << "uLightPos error" << std::endl;
  }
  
  location = glGetUniformLocation(program, "uThickness");
  if (location >= 0) {
    glUniform1fv(location, 1, &oi.thickness);
  } else {
    std::cout << "uThickness error" << std::endl;
  }  

  memcpy(lightVPMtx, &vpMtx[0][0], sizeof(float)* 16);

    

  location = glGetUniformLocation(program, "uOpacityValue");
  if (location >= 0) {
    glUniform1fv(location, 1, &(oi.opacityValue));
  }
  else {
    std::cout << "ユニフォーム変数" << "uOpacityValue" << "が見つからない" << std::endl;
  }

  t_TextureInfo texInfo;
  for (unsigned int i = 0; i < texture.size(); i++) {
    if (strcmp(texture[i].name, "DepthTexture") == 0) {
      texInfo = texture[i];
      break;
    }
  }

  int texUnitNum = 10;
  glActiveTexture(GL_TEXTURE0 + texUnitNum);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texInfo.texture);

  location = glGetUniformLocation(program, "uDepthTex");
  if (location >= 0) {
    glUniform1i(location, texUnitNum);
  }
  else {
    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uDepthTex");
  }

  //
  /////////////////////////////////////////////////////////////////////////////

  for (unsigned int i = 0; i < texture.size(); i++) {
    if (strcmp(texture[i].name, "OpcityTexture") == 0) {
      texInfo = texture[i];
      break;
    }
  }


  glBindFramebuffer(GL_FRAMEBUFFER, texInfo.fbo);
  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texInfo.texture, 0, 0);

  glClearDepth(0.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    glBindVertexArray(objectInfo[i].handle);
    glDrawElements(GL_TRIANGLES, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  glUseProgram(0);
}
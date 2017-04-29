#include "AkgRenderLineOpacityMap.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

AkgRenderLineOpacityMap::AkgRenderLineOpacityMap(void)
{}
AkgRenderLineOpacityMap::~AkgRenderLineOpacityMap(void)
{}
void AkgRenderLineOpacityMap::Render(GLuint program, std::vector<t_ObjectInfo> objectInfo, std::vector<t_TextureInfo> texture, float lightMVPMtx[])
{
  glUseProgram(program);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE, GL_ONE);
  glEnable(GL_BLEND);

  /////////////////////////////////////////////////////////////////////////////
  //send uniform values



  //fragment shader
  t_ObjectInfo hairBallObjectInfo;
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    if (strcmp(objectInfo[i].name, "LineHairBall") == 0) {
      hairBallObjectInfo = objectInfo[i];
      break;
    }
  }

  glm::mat4 newlightMVPMtx = glm::make_mat4(lightMVPMtx) * glm::make_mat4(hairBallObjectInfo.modelMatrix);
  //vertex shader
  int location = glGetUniformLocation(program, "uMVP");
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &newlightMVPMtx[0][0]);
  }


  location = glGetUniformLocation(program, "uOpacityValue");
  if (location >= 0) {
    glUniform1fv(location, 1, &(hairBallObjectInfo.opacityValue));
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
    glDrawElements(GL_LINE_LOOP, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  glUseProgram(0);
}
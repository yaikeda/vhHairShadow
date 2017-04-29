#include "AkgRenderRectangleOpacityEvaluate.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

AkgRenderRectangleOpacityEvaluate::AkgRenderRectangleOpacityEvaluate(void)
{}

AkgRenderRectangleOpacityEvaluate::~AkgRenderRectangleOpacityEvaluate(void)
{}

void AkgRenderRectangleOpacityEvaluate::Render(
  GLuint program,
  std::vector<t_ObjectInfo> objectInfo,
  std::vector<t_TextureInfo> texture,
  float lightVPMtx[],
  t_CameraInfo cameraInfo)
{
  glUseProgram(program);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glClearDepth(1.0);
  glClearColor((GLclampf)1.0, (GLclampf)1.0, (GLclampf)1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





  /*

  //fragment shader
  t_ObjectInfo oi;
  for (unsigned int i = 0; i < objectInfo.size(); i++) {
    if (strcmp(objectInfo[i].name, "RectangleHairBall") == 0) {
      oi = objectInfo[i];
      break;
    }
  }

  glm::mat4 modelx = glm::make_mat4(oi.modelMatrix);
  glm::mat4 viewx = glm::make_mat4(lightInfo->at(0).viewMatrix);
  glm::mat4 projx = glm::make_mat4(lightInfo->at(0).projMatrix);


  glm::mat4 mvMtx = viewx * modelx;
  glm::mat4 vpMtx = projx * viewx;
  glm::mat4 mvpMtx = projx * mvMtx;
  glm::mat4 normalMtx = glm::transpose(glm::inverse(modelx));

  glm::mat4 invV = glm::inverse(viewx);

  int loc = glGetUniformLocation(program, "uP");
  if (loc >= 0) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, &projx[0][0]);
  }
  else {
    std::cout << "uP error" << std::endl;
  }

  loc = glGetUniformLocation(program, "uMV");
  if (loc >= 0) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mvMtx[0][0]);
  }
  else {
    std::cout << "uMV error" << std::endl;
  }
  loc = glGetUniformLocation(program, "uinvV");
  if (loc >= 0) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, &invV[0][0]);
  }
  else {
    std::cout << "uivV error" << std::endl;
  }
  loc = glGetUniformLocation(program, "uThickness");
  if (loc >= 0) {
    glUniform1fv(loc, 1, &oi.thickness);
  }
  else {
    std::cout << "uThickness error" << std::endl;
  }


  */


















  //fragment shader
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

  int location = glGetUniformLocation(program, "uDepthTex");
  if (location >= 0) {
    glUniform1i(location, texUnitNum);
  }
  else {
    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uDepthTex");
  }

  for (unsigned int i = 0; i < texture.size(); i++) {
    if (strcmp(texture[i].name, "OpcityTexture") == 0) {
      texInfo = texture[i];
      break;
    }
  }

  texUnitNum = 11;
  glActiveTexture(GL_TEXTURE0 + texUnitNum);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texInfo.texture);

  location = glGetUniformLocation(program, "uOpacityTex");
  if (location >= 0) {
    glUniform1i(location, texUnitNum);
  }
  else {
    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uOpacityTex");
  }

  location = glGetUniformLocation(program, "uMatrixNum");
  if (location >= 0) {
    glUniform1i(location, 1);
  }
  else {
    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uMatrixNum");
  }


  t_TextureInfo sendTexture;
  for (unsigned int i = 0; i < texture.size(); i++) {
    if (strcmp(texture[i].name, "OpcityTexture")) {
      sendTexture = texture[i];
      break;
    }
  }

  /*
  location = glGetUniformLocation(program, "uMapResolution");
  if (location >= 0) {
  glUniform1iv(location, 1, &(sendTexture.size));
  } else {
  fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uMapResolution");
  }
  */




  float offset = 0.0f;
  location = glGetUniformLocation(program, "uOffset");
  if (location >= 0) {
    glUniform1fv(location, 1, &offset);
  }
  else {
    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uOffset");
  }

  float shadowColor[3] = { 1.0, 1.0, 1.0 };
  location = glGetUniformLocation(program, "uShadowColor");
  if (location >= 0) {
    glUniform3fv(location, 1, shadowColor);
  }
  else {
    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uShadowColor");
  }
  //
  /////////////////////////////////////////////////////////////////////////////





  glm::mat4 viewMtx = glm::lookAt(
    glm::make_vec3(cameraInfo.pos),
    glm::make_vec3(cameraInfo.look),
    glm::make_vec3(cameraInfo.up));
  glm::mat4 projMtx =
    glm::perspective<float>(
    cameraInfo.angleAspectNearFar[0],
    cameraInfo.angleAspectNearFar[1],
    cameraInfo.angleAspectNearFar[2],
    cameraInfo.angleAspectNearFar[3]);



  for (unsigned int i = 0; i < objectInfo.size(); i++) {

    glm::mat4 modelMtx = glm::make_mat4(objectInfo[i].modelMatrix);
    glm::mat4 MVPMtx = projMtx * viewMtx * modelMtx;

    /*
    ////////////////////////////////////////////////////////////
    int flag = 0;
    if (strcmp(objectInfo[i].name, "RectangleHairBall") == 0) {
      flag = 1;
    }
    location = glGetUniformLocation(program, "uRectangleFlag");
    if (location >= 0) {
      glUniform1iv(location, 1, &flag);
    }
    else {
      fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uRectangleFlag");
    }
    ///////////////////////////////////////////////////////////////////
*/

    location = glGetUniformLocation(program, "uMVP");
    if (location >= 0) {
      glUniformMatrix4fv(location, 1, GL_FALSE, &MVPMtx[0][0]);
    }
    else {
      fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uMVP");
    }




    glm::mat4 newLightMVPMtx = glm::make_mat4(lightVPMtx) * glm::make_mat4(objectInfo[i].modelMatrix);
    location = glGetUniformLocation(program, "uLightMVP");
    if (location >= 0) {
      glUniformMatrix4fv(location, 1, GL_FALSE, &newLightMVPMtx[0][0]);
    }
    else {
      fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uLightMVP");
    }

    glBindVertexArray(objectInfo[i].handle);

    
    if (strcmp(objectInfo[i].name, "LineHairBall") == 0) {
      glDrawElements(GL_LINE_LOOP, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
    }
    else {
      glDrawElements(GL_TRIANGLES, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
    }
    
    //glDrawElements(GL_TRIANGLES, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);    
  }

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glUseProgram(0);
}
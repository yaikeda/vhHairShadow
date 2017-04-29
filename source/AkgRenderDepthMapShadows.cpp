#include "AkgRenderDepthMapshadows.h"



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

AkgRenderDepthMapShadows::AkgRenderDepthMapShadows(void)
{}

AkgRenderDepthMapShadows::~AkgRenderDepthMapShadows(void)
{}

void AkgRenderDepthMapShadows::Render(
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
  glClearColor((GLclampf)0.7, (GLclampf)0.7, (GLclampf)0.7, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 
  //fragment shader
  t_TextureInfo texInfo;
  for (unsigned int i = 0; i < texture.size(); i++) {
    if (strcmp(texture[i].name, "DepthTexture") == 0) {
      texInfo = texture[i];
      break;
    }
  }

  int texUnitNum = 10;
  glActiveTexture(GL_TEXTURE0+texUnitNum);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texInfo.texture);

  int location = glGetUniformLocation(program, "texDepthMap");
  if (location >= 0) {
	  glUniform1i(location, texUnitNum);
  } else {
	  fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "texDepthMap");
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
 

    int location = glGetUniformLocation(program, "uMVPMtx");
    if (location >= 0) {
      glUniformMatrix4fv(location, 1, GL_FALSE, &MVPMtx[0][0]);
    } else {
	    fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uMVPMtx");
    }

    glm::mat4 lightMVPMtx = glm::make_mat4(lightVPMtx) * glm::make_mat4(objectInfo[i].modelMatrix);
    location = glGetUniformLocation(program, "uLightMVPMtx");
    if (location >= 0) {
      glUniformMatrix4fv(location, 1, GL_FALSE, &lightMVPMtx[0][0]);
    } else {
      fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uLightMVPMtx");
    }

    location = glGetUniformLocation(program, "uOffset");
    if (location >= 0) {
      glUniform1fv(location, 1, &(objectInfo[i].opacityValue));
    } else {
      fprintf(stderr, "ユニフォーム変数%sが見つからない\n", "uOffset");
    }

    glBindVertexArray(objectInfo[i].handle);
    
    if (strcmp(objectInfo[i].name, "LineHairBall") == 0) {
      glDrawElements(GL_LINE_LOOP, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
    }
    else {
      glDrawElements(GL_TRIANGLES, objectInfo[i].indexSize, GL_UNSIGNED_INT, objectInfo[i].index);
    }
  }

 
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  
  glUseProgram(0);
}
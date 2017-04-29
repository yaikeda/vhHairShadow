#include "AkgGeneralManager.h"

#include "AkgShaderGenerator.h"
#include "AkgTextureGenerator.h"

#include "AkgRenderOpacityDepth.h"
#include "AkgRenderOpacityMap.h"
#include "AkgRenderOpacityEvaluate.h"
#include "AkgRenderLineOpacityDepth.h"
#include "AkgRenderLineOpacityMap.h"
#include "AkgRenderLineOpacityEvaluate.h"
#include "AkgRenderRibbonOpacityDepth.h"
#include "AkgRenderRibbonOpacityMap.h"
#include "AkgRenderRibbonOpacityEvaluate.h"
#include "AkgRenderTriangleOpacityDepth.h"
#include "AkgRenderTriangleOpacityMap.h"
#include "AkgRenderTriangleOpacityEvaluate.h"
#include "AkgRenderRectangleOpacityDepth.h"
#include "AkgRenderRectangleOpacityMap.h"
#include "AkgRenderRectangleOpacityEvaluate.h"
#include "AkgRenderDepthMap.h"
#include "AkgRenderDepthMapshadows.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string.h>




AkgGeneralManager::AkgGeneralManager()
{}
AkgGeneralManager::~AkgGeneralManager()
{}

void AkgGeneralManager::SetLight(void)
{
  glm::vec3 vPos = glm::vec3(0.0, 200.0, -200);
  glm::vec3 vLook = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 vUp = glm::vec3(0.0, 1.0, 0.0);


  t_LightInfo lightInfo;
  memcpy(lightInfo.pos, &vPos, sizeof(float) * 3);
  memcpy(lightInfo.look, &vLook, sizeof(float) * 3);
  memcpy(lightInfo.up, &vUp, sizeof(float) * 3);

  m_Light.push_back(lightInfo);
}



void AkgGeneralManager::SetObjects(void)
{

  {
    t_ObjectInfo objectInfo;
    m_Object["CubeAggregate"] = &m_CubeAggregate;
    objectInfo.name = "CubeAggregate";
    m_Object["CubeAggregate"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
  
  /*
  {
    t_ObjectInfo objectInfo;
    m_Object["HairBall"] = &m_HairBall;
    objectInfo.name = "HairBall";
    m_Object["HairBall"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
  */
 
  {
    t_ObjectInfo objectInfo;
    m_Object["LineHairBall"] = &m_LineHairBall;
    objectInfo.name = "LineHairBall";
    m_Object["LineHairBall"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
    /*
  {
    t_ObjectInfo objectInfo;
    m_Object["RibbonHairBall"] = &m_RibbonHairBall;
    objectInfo.name = "RibbonHairBall";
    m_Object["RibbonHairBall"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
  */
  /*
  {
    t_ObjectInfo objectInfo;
    m_Object["TriangleHairBall"] = &m_TriangleHairBall;
    objectInfo.name = "TriangleHairBall";
    m_Object["TriangleHairBall"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
  */
  
  {
    t_ObjectInfo objectInfo;
    m_Object["RectangleHairBall"] = &m_RectangleHairBall;
    objectInfo.name = "RectangleHairBall";
    m_Object["RectangleHairBall"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
  
  /*
  {
    t_ObjectInfo objectInfo;
    m_Object["Plane"] = &m_Plane;
    objectInfo.name = "Plane";
    m_Object["Plane"]->Create(&objectInfo);
    m_ObjectInfo.push_back(objectInfo);
  }
  */
}

void AkgGeneralManager::SetRenderPass(void)
{
  AkgShaderGenerator shaderGenerator;
  m_RenderPass["OpacityDepth"] = shaderGenerator.LoadShader("shaders/OpacityDepth");
  m_RenderPass["OpacityMap"] = shaderGenerator.LoadShader("shaders/OpacityMap");
  m_RenderPass["OpacityEvaluate"] = shaderGenerator.LoadShader("shaders/OpacityEval");
  
  m_RenderPass["LineOpacityDepth"] = shaderGenerator.LoadShader("shaders/OpacityDepth");
  m_RenderPass["LineOpacityMap"] = shaderGenerator.LoadShader("shaders/OpacityMap");
  m_RenderPass["LineOpacityEvaluate"] = shaderGenerator.LoadShader("shaders/OpacityEval");
  
  m_RenderPass["RibbonOpacityDepth"] = shaderGenerator.LoadShader("shaders/RibbonOpacityDepth");
  m_RenderPass["RibbonOpacityMap"] = shaderGenerator.LoadShader("shaders/RibbonOpacityMap");
  m_RenderPass["RibbonOpacityEvaluate"] = shaderGenerator.LoadShader("shaders/RibbonOpacityEval");

  m_RenderPass["TriangleOpacityDepth"] = shaderGenerator.LoadShader("shaders/TriangleOpacityDepth");
  m_RenderPass["TriangleOpacityMap"] = shaderGenerator.LoadShader("shaders/TriangleOpacityMap");
  m_RenderPass["TriangleOpacityEvaluate"] = shaderGenerator.LoadShader("shaders/TriangleOpacityEval");

  m_RenderPass["RectangleOpacityDepth"] = shaderGenerator.LoadShader("shaders/RectangleOpacityDepth");
  m_RenderPass["RectangleOpacityMap"] = shaderGenerator.LoadShader("shaders/RectangleOpacityMap");
  m_RenderPass["RectangleOpacityEvaluate"] = shaderGenerator.LoadShader("shaders/RectangleOpacityEval");


  m_RenderPass["DepthMap"] = shaderGenerator.LoadShader("shaders/DepthMap");
  m_RenderPass["DepthMapShadows"] = shaderGenerator.LoadShader("shaders/DepthMapShadows");
}

void AkgGeneralManager::SetTexture(void)
{
  AkgTextureGenerator textureGenerator;

  {
    t_TextureInfo texture;
    texture.name = "DepthTexture";
    texture.size = m_TextureSize;
    textureGenerator.GenerateTexture(&texture, 1);
    m_Texture.push_back(texture);  
  }

  {
    t_TextureInfo texture;
    texture.name = "OpcityTexture";
    texture.size = m_TextureSize;
    textureGenerator.GenerateTexture(&texture, 1);
    m_Texture.push_back(texture);  
  }
}



//Get
int* AkgGeneralManager::GetWindowSize(void)
{
  return m_WindowSize;
}

void AkgGeneralManager::GetCamera(
  float* pos, 
  float* look, 
  float* up, 
  float* angleAspectNearFar)
{
  if (pos != NULL) {
    memcpy(pos,           m_Camera.pos,           sizeof(float) * 3);
  }
  if (look != NULL) {
    memcpy(look,          m_Camera.look,          sizeof(float) * 3);
  }
  if (up != NULL) {
    memcpy(up,            m_Camera.up,            sizeof(float) * 3);
  }
  if (angleAspectNearFar != NULL) {
    memcpy(angleAspectNearFar,  m_Camera.angleAspectNearFar,  sizeof(float) * 4);
  }
}

float AkgGeneralManager::GetObjectOpacityvalue(const char* objectName)
{
  t_ObjectInfo* objectInfo = NULL;
  for (unsigned int i = 0; i < m_ObjectInfo.size(); i++) {
    if (strcmp(m_ObjectInfo[i].name, objectName) == 0) {
      objectInfo = &m_ObjectInfo[i];
      break;
    }
  }

  if (objectInfo == NULL) {
    return 0.0f;
  }
  return objectInfo->opacityValue;
} 

float AkgGeneralManager::GetObjectThickness(const char* objectName) {
  t_ObjectInfo* objectInfo = NULL;
  for (unsigned int i = 0; i < m_ObjectInfo.size(); i++) {
    if (strcmp(m_ObjectInfo[i].name, objectName) == 0) {
      objectInfo = &m_ObjectInfo[i];
      break;
    }
  }

  if (objectInfo == NULL) {
    return 0.0f;
  }
  return objectInfo->thickness;
}

bool AkgGeneralManager::GetAnimationFlag(void)
{
  return m_ModelMatrixInfo.m_AnimationFlag;
}

float AkgGeneralManager::GetRotationSpeed(void)
{
  return m_ModelMatrixInfo.m_RotationSpeed;
}

int* AkgGeneralManager::GetMotionStart(void)
{
  return m_MotionStart;
}


//Set
void AkgGeneralManager::SetWindowSize(int w, int h)
{
  m_WindowSize[0] = w;
  m_WindowSize[1] = h;
}

void AkgGeneralManager::SetTextureSize(int size)
{
  m_TextureSize = size;
}

void AkgGeneralManager::SetCamera(
  float pos[3], 
  float look[3], 
  float up[3], 
  float angleAspectNearFar[4])
{
  if (pos != NULL) {
    memcpy(m_Camera.pos, pos, sizeof(float) * 3);
  }
  if (look != NULL) {
    memcpy(m_Camera.look, look, sizeof(float) * 3);
  }
  if (up != NULL) {
    memcpy(m_Camera.up, up, sizeof(float) * 3);
  }
  if (angleAspectNearFar != NULL) {
    memcpy(m_Camera.angleAspectNearFar, angleAspectNearFar, sizeof(float) * 4);
  }
}
void AkgGeneralManager::SetObjectOpacityValue(const char* objectName, float value)
{
  for (unsigned int i = 0; i < m_ObjectInfo.size(); i++) {
    if (strcmp(m_ObjectInfo[i].name, objectName) == 0) {
      m_ObjectInfo[i].opacityValue = value;
      break;
    }
  }
} 

void AkgGeneralManager::SetObjectThickness(const char* objectName, float value)
{
  for (unsigned int i = 0; i < m_ObjectInfo.size(); i++) {
    if (strcmp(m_ObjectInfo[i].name, objectName) == 0) {
      m_ObjectInfo[i].thickness = value;
      break;
    }
  }
}

void AkgGeneralManager::SetAnimationFlag(bool flag)
{
  m_ModelMatrixInfo.m_AnimationFlag = flag;
}

void AkgGeneralManager::SetRotationSpeed(float value)
{
  m_ModelMatrixInfo.m_RotationSpeed = value;
}

void AkgGeneralManager::SetMotionStart(int* motion)
{
  memcpy(m_MotionStart, motion, sizeof(int) * 2);
}


bool AkgGeneralManager::Render()
{
  SetModelMatrix();//modelMatrix設定

  RenderLineDeepOpacityMaps();
  //RenderRectangleDeepOpacityMaps();
  //RenderDeepOpacityMaps();
  //RenderDepthMapShadows();
  //RenderTriangleDeepOpacityMaps();
  //RenderRibbonDeepOpacityMaps();
  //RenderVSMShadows();

  glutSwapBuffers(); 

  readpixel.saveImage(512, 512);
    
  return true;
}




///////////////////////////////////////////////////////////////////////////////
//protected////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::vector<t_ObjectInfo> AkgGeneralManager::GetIndexPointerList(std::vector<char*> itemList)
{
  std::vector<t_ObjectInfo> objectInfo;
  for (unsigned int i = 0; i < m_ObjectInfo.size(); i++) {
    for (unsigned int j = 0; j < itemList.size(); j++) {
      if (strcmp(itemList[j], m_ObjectInfo[i].name) == 0) {
        objectInfo.push_back(m_ObjectInfo[i]);  
      }
    }
  }

  return objectInfo;
}

void AkgGeneralManager::SetModelMatrix(void)
{
  for (unsigned int i = 0; i < m_ObjectInfo.size(); i++) {
    if (strcmp(m_ObjectInfo[i].name, "HairBall") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
      glm::mat4 rotate = glm::rotate<float>(glm::mat4(1.0f), m_ModelMatrixInfo.m_RotationAngle, glm::vec3(0.0, 1.0, 0.0));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      glm::mat4 modelMtx = translate * rotate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)*16);
    }
    else if (strcmp(m_ObjectInfo[i].name, "LineHairBall") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
      glm::mat4 rotate = glm::rotate<float>(glm::mat4(1.0f), m_ModelMatrixInfo.m_RotationAngle, glm::vec3(0.0, 1.0, 0.0));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      glm::mat4 modelMtx = translate * rotate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)* 16);
    }
    else if (strcmp(m_ObjectInfo[i].name, "RibbonHairBall") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
      glm::mat4 rotate = glm::rotate<float>(glm::mat4(1.0f), m_ModelMatrixInfo.m_RotationAngle, glm::vec3(0.0, 1.0, 0.0));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      glm::mat4 modelMtx = translate * rotate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)* 16);
    }
    else if (strcmp(m_ObjectInfo[i].name, "TriangleHairBall") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
      glm::mat4 rotate = glm::rotate<float>(glm::mat4(1.0f), m_ModelMatrixInfo.m_RotationAngle, glm::vec3(0.0, 1.0, 0.0));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      glm::mat4 modelMtx = translate * rotate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)* 16);
    }
    else if (strcmp(m_ObjectInfo[i].name, "RectangleHairBall") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
      glm::mat4 rotate = glm::rotate<float>(glm::mat4(1.0f), m_ModelMatrixInfo.m_RotationAngle, glm::vec3(0.0, 1.0, 0.0));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      glm::mat4 modelMtx = translate * rotate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)* 16);
    }
    else if (strcmp(m_ObjectInfo[i].name, "Plane") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 3000.0f));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f));
      glm::mat4 modelMtx = translate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)*16);
    }
    else if (strcmp(m_ObjectInfo[i].name, "CubeAggregate") == 0) {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(3000.0f, 1.0f, 3000.0f));
      glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f));
      glm::mat4 modelMtx = translate * scale;
      memcpy(m_ObjectInfo[i].modelMatrix, &modelMtx[0][0], sizeof(float)*16);
    } 
  }

  if (m_ModelMatrixInfo.m_AnimationFlag) {
    m_ModelMatrixInfo.m_RotationAngle  = 
    (m_ModelMatrixInfo.m_RotationAngle + m_ModelMatrixInfo.m_RotationSpeed);
  }  
  if (m_ModelMatrixInfo.m_RotationAngle > 360) {
    m_ModelMatrixInfo.m_RotationAngle = 0.0f;
  }

}

void AkgGeneralManager::RenderDeepOpacityMaps(void)
{
  //レンダリングするオブジェクトの登録
  std::vector<char*> itemList;
  itemList.push_back("HairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);


  glm::mat4 lightMVPMtx = glm::mat4(1.0f);

  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  AkgRenderOpacityDepth ROD;
  ROD.Render(m_RenderPass["OpacityDepth"], objectInfo, m_Texture, &lightMVPMtx, m_Light);

  
  //opacity map rendering
  AkgRenderOpacityMap ROM;
  ROM.Render(m_RenderPass["OpacityMap"], objectInfo, m_Texture, &lightMVPMtx[0][0]);

  
  //opacity  evaluate
  glViewport(0, 0, m_WindowSize[0], m_WindowSize[1]);
  itemList.push_back("CubeAggregate");
  objectInfo = GetIndexPointerList(itemList);

  AkgRenderOpacityEvaluate ROE;
  ROE.Render(m_RenderPass["OpacityEvaluate"], objectInfo, m_Texture,
    &lightMVPMtx[0][0], m_Camera);
    
}

void AkgGeneralManager::RenderLineDeepOpacityMaps(void)
{
  //レンダリングするオブジェクトの登録
  std::vector<char*> itemList;
  itemList.push_back("LineHairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);


  glm::mat4 lightMVPMtx = glm::mat4(1.0f);

  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  AkgRenderLineOpacityDepth RLOD;
  RLOD.Render(m_RenderPass["LineOpacityDepth"], objectInfo, m_Texture, &lightMVPMtx, m_Light);

  
  //opacity map rendering
  AkgRenderLineOpacityMap RLOM;
  RLOM.Render(m_RenderPass["LineOpacityMap"], objectInfo, m_Texture, &lightMVPMtx[0][0]);

  
  //opacity  evaluate
  glViewport(0, 0, m_WindowSize[0], m_WindowSize[1]);
  itemList.push_back("CubeAggregate");
  objectInfo = GetIndexPointerList(itemList);

  AkgRenderLineOpacityEvaluate RLineOE;
  RLineOE.Render(m_RenderPass["LineOpacityEvaluate"], objectInfo, m_Texture,
    &lightMVPMtx[0][0], m_Camera);
  
}

void AkgGeneralManager::RenderRibbonDeepOpacityMaps(void)
{
  //レンダリングするオブジェクトの登録
  std::vector<char*> itemList;
  itemList.push_back("RibbonHairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);


  glm::mat4 lightVPMtx = glm::mat4(1.0f);

  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  AkgRenderRibbonOpacityDepth RROD;
  RROD.Render(m_RenderPass["RibbonOpacityDepth"], objectInfo, m_Texture, &lightVPMtx, &m_Light);

  
  //opacity map rendering
  AkgRenderRibbonOpacityMap RROM;
  RROM.Render(m_RenderPass["RibbonOpacityMap"], objectInfo, m_Texture, &m_Light, &lightVPMtx[0][0]);

  
  //opacity  evaluate
  itemList.clear();
  glViewport(0, 0, m_WindowSize[0], m_WindowSize[1]);
  itemList.push_back("HairBall");
  itemList.push_back("CubeAggregate");
  objectInfo = GetIndexPointerList(itemList);

  AkgRenderRibbonOpacityEvaluate RROE;
  RROE.Render(m_RenderPass["RibbonOpacityEvaluate"], objectInfo, m_Texture,
    &lightVPMtx[0][0], m_Camera);
}


void AkgGeneralManager::RenderTriangleDeepOpacityMaps(void)
{
  //レンダリングするオブジェクトの登録
  std::vector<char*> itemList;
  itemList.push_back("TriangleHairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);


  glm::mat4 lightVPMtx = glm::mat4(1.0f);

  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  AkgRenderTriangleOpacityDepth ROD;
  ROD.Render(m_RenderPass["TriangleOpacityDepth"], objectInfo, m_Texture, &lightVPMtx, &m_Light);

  
  //opacity map rendering
  AkgRenderTriangleOpacityMap ROM;
  ROM.Render(m_RenderPass["TriangleOpacityMap"], objectInfo, m_Texture, &m_Light, &lightVPMtx[0][0]);

  
  //opacity  evaluate
  itemList.clear();
  glViewport(0, 0, m_WindowSize[0], m_WindowSize[1]);
  itemList.push_back("HairBall");
  itemList.push_back("CubeAggregate");
  objectInfo = GetIndexPointerList(itemList);
  
  AkgRenderTriangleOpacityEvaluate ROE;
  ROE.Render(m_RenderPass["TriangleOpacityEvaluate"], objectInfo, m_Texture,
    &lightVPMtx[0][0], m_Camera);
  
}

void AkgGeneralManager::RenderRectangleDeepOpacityMaps(void)
{
/*
  //レンダリングするオブジェクトの登録
  std::vector<char*> itemList;
  itemList.push_back("LineHairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);


  glm::mat4 lightMVPMtx = glm::mat4(1.0f);

  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  AkgRenderDepthMap RDM;
  RDM.Render(m_RenderPass["DepthMap"], objectInfo, m_Texture, &lightMVPMtx, m_Light);
  */



  std::vector<char*> itemList;
  itemList.push_back("RectangleHairBall");
  //itemList.push_back("HairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);

  glm::mat4 lightVPMtx = glm::mat4(1.0f);

  
  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  

  AkgRenderRectangleOpacityDepth RaOD;
  RaOD.Render(m_RenderPass["RectangleOpacityDepth"], objectInfo, m_Texture, &lightVPMtx, &m_Light);


  //itemList.clear();
  //itemList.push_back("RectangleHairBall");
  //objectInfo.clear();
  //objectInfo = GetIndexPointerList(itemList);
  //opacity map rendering
  AkgRenderRectangleOpacityMap ROM;
  ROM.Render(m_RenderPass["RectangleOpacityMap"], objectInfo, m_Texture, &m_Light, &lightVPMtx[0][0], m_Camera);

  
  //opacity  evaluate
  itemList.clear();
  glViewport(0, 0, m_WindowSize[0], m_WindowSize[1]);
  itemList.push_back("LineHairBall");
  itemList.push_back("CubeAggregate");
  objectInfo = GetIndexPointerList(itemList);

  AkgRenderRectangleOpacityEvaluate ROE;
  ROE.Render(m_RenderPass["RectangleOpacityEvaluate"], objectInfo, m_Texture,
    &lightVPMtx[0][0], m_Camera);
  
    
}

void AkgGeneralManager::RenderDepthMapShadows(void)
{
  //レンダリングするオブジェクトの登録
  std::vector<char*> itemList;
  itemList.push_back("LineHairBall");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);


  glm::mat4 lightMVPMtx = glm::mat4(1.0f);

  //opacity depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize);
  AkgRenderDepthMap RDM;
  RDM.Render(m_RenderPass["DepthMap"], objectInfo, m_Texture, &lightMVPMtx, m_Light);

  
  //opacity  evaluate
  glViewport(0, 0, m_WindowSize[0], m_WindowSize[1]);
  itemList.push_back("CubeAggregate");
  objectInfo = GetIndexPointerList(itemList);
  
  AkgRenderDepthMapShadows RDS;
  RDS.Render(m_RenderPass["DepthMapShadows"], objectInfo, m_Texture,
    &lightMVPMtx[0][0], m_Camera);
    
}

void AkgGeneralManager::RenderVSMShadows(void)
{
  std::vector<char*> itemList;
  itemList.push_back("CubeAggregate");
  std::vector<t_ObjectInfo> objectInfo = GetIndexPointerList(itemList);

  glm::mat4 lightMVPMtx = glm::mat4(1.0f);
  
  //depth rendering
  glViewport(0, 0, m_TextureSize, m_TextureSize );
  AkgRenderDepthMap RDM;
  RDM.Render(m_RenderPass["DepthMap"], objectInfo, m_Texture, &lightMVPMtx, m_Light);



}
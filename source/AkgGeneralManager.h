#ifndef __AKGGENERALMANAGER_H__
#define __AKGGENERALMANAGER_H__

#include "AkgTypeDef.h"

#include "AkgObjects.h"
#include "AkgCubeAggregate.h"
#include "AkgHairBall.h"
#include "AkgLineHairBall.h"
#include "AkgRibbonHairBall.h"
#include "AkgTriangleHairBall.h"
#include "AkgRectangleHairBall.h"
#include "AkgPlane.h"

#include "GL/glew.h"
#include "GL/wglew.h"

#include "readPixel.h"

#include <vector>
#include <map>
#include <string>

class AkgGeneralManager {
public:
  AkgGeneralManager();
  ~AkgGeneralManager();

  //init
  void SetLight(void);
  void SetObjects(void);
  void SetRenderPass(void);
  void SetTexture(void);
  
  //Render
  bool Render(void);

  //Get
  int* GetWindowSize();
  void GetCamera(float* pos, float* look, float* up, float* angleAspectNearFar);
  float GetObjectOpacityvalue(const char* objectName);
  float GetObjectThickness(const char* objectName);
  bool GetAnimationFlag(void);
  float GetRotationSpeed(void);
  int* GetMotionStart(void);

  //Set
  void SetWindowSize(int w, int h);
  void SetTextureSize(int size);
  void SetCamera(float pos[3], float look[3], float up[3], float angleAspectNearFar[4]);
  void SetObjectOpacityValue(const char* objectName, float value);
  void SetObjectThickness(const char* objectName, float value);
  void SetAnimationFlag(bool flag);
  void SetRotationSpeed(float value);
  void SetMotionStart(int* start);

protected:
  std::vector<t_ObjectInfo> GetIndexPointerList(std::vector<char*> itemList);
  void SetModelMatrix(void);

  //Render pattern
  void RenderDeepOpacityMaps(void);
  void RenderLineDeepOpacityMaps(void);
  void RenderRibbonDeepOpacityMaps(void);
  void RenderTriangleDeepOpacityMaps(void);
  void RenderRectangleDeepOpacityMaps(void);
  void RenderDepthMapShadows(void);
  void RenderVSMShadows(void);

private:
  //normal data
  int m_WindowSize[2];
  int m_TextureSize;
  int m_MotionStart[2];



  typedef struct modelMatrixInfo {
    bool m_AnimationFlag;
    float m_RotationAngle;
    float m_RotationSpeed;
  } t_modelMatrixInfo;
  t_modelMatrixInfo m_ModelMatrixInfo;

  //GPU data Handle
  t_RenderPassMap             m_RenderPass; 
  t_CameraInfo                m_Camera;
  std::vector<t_TextureInfo>  m_Texture;
  std::vector<t_LightInfo>    m_Light;
  
  //data refer
  std::vector<t_ObjectInfo>   m_ObjectInfo;
  t_ObjectMap                 m_Object;//object 

  //raw objects
  AkgCubeAggregate            m_CubeAggregate;
  AkgHairBall                 m_HairBall;
  AkgLineHairBall             m_LineHairBall;
  AkgRibbonHairBall           m_RibbonHairBall;
  AkgTriangleHairBall         m_TriangleHairBall;
  AkgRectangleHairBall        m_RectangleHairBall;
  AkgPlane                    m_Plane;

  //support
  ReadPixel readpixel;

};

#endif
#include <stdio.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits.h>


#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "external_cyHairFile.h"

#include "ikdDataSet.h"

class ikdObjects
{
private:
  void ikdGetVertexBox(float boxVert[], float boxNormal[], float boxTexCoord[]);
  void ikdGetVertexPlane(float planeVert[], float planeNormal[], float planeTexCoord[]);
  void ikdGetVertexFromFile(const char* fileName);
  
public:
  GLuint ikdTriangle(void);
  GLuint ikdBox(const float width, const float height, const float depth);
  GLuint ikdBox(ikdDataSet* pdataSet);
  GLuint ikdPlane(const float width, const float depth);
  GLuint ikdScalablePlane(void);
  GLuint ikdLoadObject(const char* fileName);
  GLuint ikdLoad_cyHairFile(ikdDataSet* pdataSet, float thickness);
  GLuint ikdScalable_cyHairFile(ikdDataSet * pdataSet);
  GLuint ikdRibbon_cyHairFile(ikdDataSet* pdataSet);
  GLuint ikdLoad_fromMyHairFile(char* fileName, ikdDataSet* pdataSet);
  GLuint ikdOSMLastPosition(ikdDataSet* pDataSet);
  GLuint ikdMakeFramebufferTexture_ArrayDepth(ikdDataSet* pdataSet,const int textureNum);
  GLuint ikdMakeFramebufferTexture_ArrayColor(ikdDataSet* pdataSet,const int textureNum);
  GLuint ikdMakeFramebufferTexture_SingleDepth(ikdDataSet* pDataSet);
  GLuint ikdMakeFramebufferTexture_SingleDepth_NoCompare(ikdDataSet* pDataSet);
  GLuint ikdMakeFramebufferTexture_SingleColor(ikdDataSet* pDataSet);
  void ikdChangeFramebufferTexture_SingleColor(int newWidth, int newHeight, ikdDataSet* pDataSet, 
		int deleteTexNum, int setFboNum);
  GLuint ikdMakeFramebufferTexture_SingleColor_calculatedShadow(ikdDataSet* pDataSet);

  void ikdMakeColorTexture(ikdDataSet* pDataSet, glm::vec4 color);

  GLuint ikddrawelemetnstest(ikdDataSet* pdataSet);
  GLuint ikdBillBoald(ikdDataSet* pdataSet);
};
#include <vector>
#include <iostream>
#include <map>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include "glm/glm.hpp"

#include "external_cyHairFile.h"

const int PROGRAM_HANDLE_COUNT = 100;
const int VAO_HANDLE_COUNT = 100;
const int FBO_HANDLE_COUNT = 100;
const int TEXTURE_HANDLE_COUNT = 100;
const int OSM_COUNT_MAX = 128;



class ikdDataSet
{
private:
	//flag
	float pcfFlag;
	int animationFlag;	
	int useShaderNumber;
	int editModeFlag;
	int referModeFlag;
	float revisionFlag;

	//window size
	int windowWidth;
	int windowHeight;

	int depthMapWidth;
	int depthMapHeight;

	//handles
	int progHandleCount;
	int vaoHandleCount;
	int fboHandleCount;
	int texHandleCount;
  
	int nowVaoHandleNum;
	int nowProgramHandleNum;
	int nowFboHandleNum;
	int nowTexHandleNum;

	//camera
	glm::vec3 nowCameraLookAt;
	float cameraAngle;

	//light
	std::vector<glm::vec3> lightPosition;
	std::vector<glm::mat4> lightViewMatrix;
	std::vector<glm::mat4> lightProjectionMatrix;
	std::vector<float> myClippingNear;
	std::vector<float> myClippingFar;
	std::vector<float> layerInterval;

	int maxLightNum;
	int lightCount;
	int visibleLightNum;
	int multiLightFlag;

	//mouse
	glm::vec2 motionStart;

	float boundingBox[3*8];

	//osm
	float osmFarArray[OSM_COUNT_MAX];
	float osmNearArray[OSM_COUNT_MAX];
	int osmNum;
	int shadow_noshadow;


	GLuint programHandle[PROGRAM_HANDLE_COUNT];
	GLuint vaoHandle[VAO_HANDLE_COUNT];
	GLuint fboHandle[FBO_HANDLE_COUNT];
	GLuint texHandle[TEXTURE_HANDLE_COUNT];
	//std::map<std::string, GLuint> programHandle;
	//std::map<std::string, GLuint> vaoHandle;
	//std::map<std::string, GLuint> fboHandle;
	//std::map<std::string, GLuint> texHandle;


	//オブジェクト描画用のインデックス保存
	std::vector<unsigned int> objectIndex;
	int objectIndexSize;



	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelViewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 cameraPosition;
	glm::vec2 mouseMove;


	//cyHairFileHeader///////////////////
	cyHairFileHeader hairFileHeader;
	cyHairFile hairFile;
	float opacityThickness;
	float renderingThickness;

	/////////////////////////////////
	float clipNear;
	float clipFar;
	float interval;

	//DOSM///////////////////////////////
	float opacityValue;
	float dosmLastPosPlaneSize;

	//animation
	float rotationSpeed;

	//kajiya-kay parameters
	float shininessOne;
	float shininessTwo;
	float specularWeightOne;
	float specularWeightTwo;
	float diffuseWeight;
	glm::vec3 diffuseColor;
	glm::vec3 specularColorOne;
	glm::vec3 specularColorTwo;

public:
	ikdDataSet();

	void setWindowSize(int w, int h);
	int getWindowWidth(void);
	int getWindowHeight(void);

	//速度等制御
	void setRotationSpeed(float rs);
	float getRotationSpeed(void);


	//flag
	void setPcfFlag(float pf);
	float getPcfFlag(void);
	void setAnimationFlag(int af);
	int getAnimationFlag(void);
	void setEditModeFlag(int emf);
	int getEditModeFlag(void);
	void setReferModeFlag(int rmf);
	int getReferModeFlag(void);
	void setRevisionFlag(float rf);
	float getRevisionFlag(void);



	//ハンドルセット、取得系
	void setProgramHandle(GLuint pH);
	GLuint getProgramHandle(int progNum);
	GLuint getNowProgramHandle(void);
  
	void setVaoHandle(GLuint vH);
	GLuint getVaoHandle(int vaoHandleNum);
	GLuint getNowVaoHandle(void);

	void setFboHandle(GLuint fboH);
	GLuint getFboHandle(int fboHandleNum);
	GLuint getNowFboHandle(void);

	void setTexHandle(GLuint tH);
	GLuint getTexHandle(int texHandleNum);
	GLuint getNowTexHandle(void);

	//index保存用
	void setIndex(int size, unsigned int indexArray[]);
	void getIndex(unsigned int indexArray[]);
	int getIndexSize(void);

	//カメラ関係
	void setCameraPosition(glm::vec3 pos);
	glm::vec3 getCameraPosition(void);
	void setCameraLookAt(float x, float y, float z);
	void setCameraLookAt(glm::vec2 vXY, float z);
	void setCameraLookAt(glm::vec3 vXYZ);
	glm::vec3 getCameraLookAt(void);
	void setCameraAngle(float angle);
	float getCameraAngle(void);

	//ライト関係
	/////////////////////////////////////////////////////////////////
	//初期化時用に, position, viewmatrix, projectionmatrixを同時に
	//設定する関数を用意
	//ライト位置の変換に対応するために, それぞれの行列を設定する
	//ことも可能にする。
	////////////////////////////////////////////////////////////////
	void setLight(glm::vec3 pos, glm::mat4 lvM, glm::mat4 lpM);
	void setLightPosition(int lightNum, glm::vec3 pos);
	glm::vec3 getLightPosition(int lightNum);
	float* getLightPosition(const char* str);
	void setLightViewMatrix(int lightNum, glm::mat4 lvM);
	glm::mat4 getLightViewMatrix(int lightNum);
	float* getLightViewMatrix(char* str);
	void setLightProjectionMatrix(int lightNum, glm::mat4 lpM);
	glm::mat4 getLightProjectionMatrix(int lightNum);
	float* getLightProjectionMatrix(char* str);
	int getLightCount(void);
	void setMultiLightFlag(int multi);
	int getMultiLightFlag(void);
	void setNearFarInterval(int lightNum, float n, float f, float i);
	float getNear(int lightNum);
	float getFar(int lightNum);
	float getInterval(int lightNum);
	float* getInterval(const char* str);
	void setMaxLightNum(int num);
	int getMaxLightNum(void);
	void setVisibleLightNum(int num);
	int getVisiblelightNum(void);
	


	//マウスの動き
	void setMotionStart(int x, int y);
	glm::vec2 getMotionStart(void);

  
	//行列
	void setModelMatrix(glm::mat4 mM);
	void multModelMatrix(glm::mat4 mM);
	glm::mat4 getModelMatrix(void);

	void setViewMatrix(glm::mat4 vM);
	glm::mat4 getViewMatrix(void);

	void setModelViewMatrix(glm::mat4 mvM);
	glm::mat4 getModelViewMatrix(void);
  
	void setProjectionMatrix(glm::mat4 pM);
	glm::mat4 getProjectionMatrix(void);

	

 
	//マウスの移動量の記録
	void setMouseMove(glm::vec2 mmV);
	glm::vec2 getMouseMove(void);

	//boundingBox関係
	void setBoundingPos(float minPos[], float maxPos[]);
	void getBoundingPos(float boundArray[]);

	//cyHairFile関係
	void initCyHairFile(const char* fileName);
	cyHairFileHeader getHairFileHeader(void);
	cyHairFile getHairFile(void);
	unsigned short* getHairFileSegmentsArray(void);
	float* getHairFilePointsArray(void);
	float* getHairFileThicknessArray(void);
	float* getHairFileTransparencyArray(void);
	float* getHairFileColorsArray(void);
	void setHairThickness_Opacity(float th);
	float getHairThickness_Opacity(void);
	void setHairThickness_Rendering(float th);
	float getHairThickness_Rendering(void);


	//OSM関係
	void setOSMNum(int in);
	int getOSMNum(void);
	void setOSMArray(float osmArray[], char* near_far);
	float* getOSMArray(char* near_far);
	void setShadowState(int on_off);
	int getShadowState(void);

	//シェーダ切り替え
	void setUseShaderNum(int num);
	int getUseShaderNum(void);


	//DOSM
	void setOpacityValue(float oV);
	float getOpacityValue(void);
	void setDosmLastPosPlaneSize(float planeSize);
	float getDosmLastPosPlaneSize(void);

	void setDepthMapSize(int width, int height);
	int getDepthMapWidth(void);
	int getDepthMapHeight(void);

	//kajiya-kay関係
	void setKajiyaShininess(float shin1, float shin2);
	float getKajiyaShininess(int shininess_One_or_Two);
	void setKajiyaSpecularWeight(float spec1, float spec2);
	float getKajiyaSpecularWeight(int specularWeight_One_or_Two);
	void setKajiyaDiffuseWeight(float diffWei);
	float getKajiyaDiffuseWeight(void);
	void setKajiyaDiffuseColor(glm::vec3 diffCol);
	glm::vec3 getKajiyaDiffuseColor(void);
	void setKajiyaSpecularColor(glm::vec3 specCol1, glm::vec3 specCol2);
	glm::vec3 getKajiyaSpecularColor(int specularColor_One_or_Two);


	void copyAllData(ikdDataSet* dataSet, ikdDataSet* motoDataSet);

	//texture
	void deleteTexture(const int deleteTextureNum);
};
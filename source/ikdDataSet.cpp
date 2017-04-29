#include "ikdDataSet.h"

ikdDataSet::ikdDataSet()
{
	//flag
	pcfFlag = 0;
	editModeFlag = 0;
	referModeFlag = 0;
	revisionFlag = 0;

	//camera
	cameraAngle = 50.0;

	//light
	lightCount = 0;
	maxLightNum = 1;
	visibleLightNum = 1;
	

	//Handle
	progHandleCount = 0;
	vaoHandleCount = 0;
	fboHandleCount = 0;
	texHandleCount = 0;

	nowProgramHandleNum = -1;
	nowVaoHandleNum = -1;
	nowFboHandleNum = -1;
	nowTexHandleNum = -1;
  
	depthMapWidth = -1;
	depthMapHeight = -1;

	osmNum = -1;

	//描画用パラメータ
	renderingThickness = 0.1;
	opacityThickness = 0.1;
	opacityValue = 1.0;

	memset(&nowCameraLookAt[0], 0, sizeof(int)*3);

	for (int i = 0; i < PROGRAM_HANDLE_COUNT; i++) {
		programHandle[i] = -1;
		vaoHandle[i] = -1;
		fboHandle[i] = -1;
	}
  
	//行列初期化
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::mat4(1.0f);


	//kajiya-kay初期化
	shininessOne = 100;
	shininessTwo = 10;
	specularWeightOne = 1.0;
	specularWeightTwo = 1.0;
	diffuseColor = glm::vec3(0.6, 0.2, 0.0);
	diffuseWeight = 1.0;
	specularColorOne = glm::vec3(1.0, 1.0, 1.0);
	specularColorTwo = glm::vec3(0.5, 0.5, 0.5);

	//dosm関係
	dosmLastPosPlaneSize = 100.0;
}

void ikdDataSet::setWindowSize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}
int ikdDataSet::getWindowWidth(void)
{
	return windowWidth;
}
int ikdDataSet::getWindowHeight(void)
{
	return windowHeight;
}


//アニメーション関係
void ikdDataSet::setRotationSpeed(float rs)
{
	rotationSpeed = rs;
}
float ikdDataSet::getRotationSpeed(void)
{
	return rotationSpeed;
}
//flag関係
void ikdDataSet::setPcfFlag(float pf)
{
	if (pf == 1.0 || pf == 0.0) {
		pcfFlag = pf;
	} else {
		fprintf(stderr, "1 か 0を入力してください\n");
	}
}
float ikdDataSet::getPcfFlag(void)
{
	return pcfFlag;
}
void ikdDataSet::setAnimationFlag(const int af)
{
	if (af == 0 || af == 1) {
		animationFlag = af;
	} else {
		fprintf(stderr, "0か1を入力してください\n");
	}
}
int ikdDataSet::getAnimationFlag(void)
{
	return animationFlag;
}
void ikdDataSet::setEditModeFlag(int emf)
{
	if (emf == 0 | emf == 1) {
		editModeFlag = emf;
	} else {
		fprintf(stderr, "0か1を入力してください\n");
	}
}
int ikdDataSet::getEditModeFlag(void)
{
	return editModeFlag;
}
void ikdDataSet::setReferModeFlag(int rmf)
{
	if (rmf == 1 || rmf == 0) {
		referModeFlag = rmf;
	} else {
		fprintf(stderr, "0か1を入力してください\nplease imput 0 or 1.\n");
	}
}
int ikdDataSet::getReferModeFlag(void)
{
	return referModeFlag;	
}
void ikdDataSet::setRevisionFlag(float rf)
{
	if (rf == 1 || rf == 0) {
		revisionFlag = rf;
	} else {
		std::cout << "0か1を入力してください" << std::endl;
	}
}
float ikdDataSet::getRevisionFlag(void)
{
	return revisionFlag;
}




//handle関係
void ikdDataSet::setProgramHandle(GLuint pH)
{
  programHandle[progHandleCount++] = pH;
}

GLuint ikdDataSet::getProgramHandle(int progNum)
{
  if (progNum >= 0 && progNum < progHandleCount) {
    nowProgramHandleNum = progNum;
    return programHandle[progNum];
  }  else {
    fprintf(stderr, "Can't get Program Hande.\nProgram Handle Num：%d", progNum);
  }
}

GLuint ikdDataSet::getNowProgramHandle()
{
  if (nowProgramHandleNum >= 0) {
    return programHandle[nowProgramHandleNum];
  }  else {
    fprintf(stderr, "Can't get Program Hande. Program isn't used.");
  }
}

void ikdDataSet::setVaoHandle(GLuint vH)
{
  vaoHandle[vaoHandleCount++] = vH;
}

GLuint ikdDataSet::getVaoHandle(int vaoHandleNum)
{
  if (vaoHandleNum >= 0 && vaoHandleNum < vaoHandleCount) {
    nowVaoHandleNum = vaoHandleNum;
    return vaoHandle[vaoHandleNum];
  } else {
    fprintf(stderr, "Can't get vaoHande.\nvaoHandleNum：%d", vaoHandleNum);
  }
}

GLuint ikdDataSet::getNowVaoHandle(void)
{
  if (nowVaoHandleNum >= 0) {
    return vaoHandle[nowVaoHandleNum];
  }  else {
    fprintf(stderr, "Can't get vaoHande. Vertex buffer object isn't used.");
  }
}


void ikdDataSet::setFboHandle(GLuint fboH)
{
  fboHandle[fboHandleCount++] = fboH;
}

GLuint ikdDataSet::getFboHandle(int fboNum)
{
  if (fboNum >= 0 && fboNum < fboHandleCount) {
    nowFboHandleNum = fboNum;
    return fboHandle[fboNum];
  }  else {
    fprintf(stderr, "Can't get FBO Hande.\nFBO Handle Num：%d", fboNum);
  }
}

GLuint ikdDataSet::getNowFboHandle()
{
  if (nowFboHandleNum >= 0) {
    return fboHandle[nowFboHandleNum];
  }  else {
    fprintf(stderr, "Can't get FBO Hande. Program isn't used.");
  }
}

void ikdDataSet::setTexHandle(GLuint tH)
{
	texHandle[texHandleCount++] = tH;
}

GLuint ikdDataSet::getTexHandle(int texNum)
{
	if (texNum >= 0 && texNum < fboHandleCount) {
		nowTexHandleNum = texNum;
		return texHandle[texNum];
	}  else {
		fprintf(stderr, "Can't get Texture Hande.\nTexture Handle Num：%d", texNum);
	}
}

GLuint ikdDataSet::getNowTexHandle(void)
{
  if (nowTexHandleNum >= 0) {
    return texHandle[nowTexHandleNum];
  }  else {
    fprintf(stderr, "Can't get Texture Hande. Program isn't used.");
  }
}

//index保存用
void ikdDataSet::setIndex(int size, unsigned int indexArray[])
{
	objectIndexSize = size;
	objectIndex.reserve(size);
	memcpy(&objectIndex[0], indexArray, sizeof(unsigned int) * size);

}
void ikdDataSet::getIndex(unsigned int indexArray[])
{
	memcpy(indexArray, &objectIndex[0], 
		sizeof(unsigned int) * objectIndexSize);
}
int ikdDataSet::getIndexSize(void)
{
	return objectIndexSize;
}


//camera
void ikdDataSet::setCameraLookAt(float x, float y, float z)
{
	nowCameraLookAt[0] = x;
	nowCameraLookAt[1] = y;
	nowCameraLookAt[2] = z;
}
void ikdDataSet::setCameraLookAt(glm::vec2 vXY, float z)
{
	nowCameraLookAt[0] = vXY.x;
	nowCameraLookAt[1] = vXY.y;
	nowCameraLookAt[2] = z;
}
void ikdDataSet::setCameraLookAt(glm::vec3 vXYZ)
{
	nowCameraLookAt = vXYZ;
}
glm::vec3 ikdDataSet::getCameraLookAt(void)
{
	return nowCameraLookAt;
}
void ikdDataSet::setCameraAngle(float angle)
{
	if (angle < 1) angle = 1;
	else if (angle > 179) angle = 179;
	cameraAngle = angle;
}
float ikdDataSet::getCameraAngle(void)
{
	return cameraAngle;
}


void ikdDataSet::setMotionStart(int x, int y)
{
	motionStart[0] = x;
	motionStart[1] = y;
}
glm::vec2 ikdDataSet::getMotionStart(void)
{
	return motionStart;
}


void ikdDataSet::setModelMatrix(glm::mat4 mM)
{
  modelMatrix = mM;
}

void ikdDataSet::multModelMatrix(glm::mat4 mM)
{
  modelMatrix = mM * modelMatrix;
}
glm::mat4 ikdDataSet::getModelMatrix(void)
{
  return modelMatrix;
}

void ikdDataSet::setViewMatrix(glm::mat4 vM)
{
  viewMatrix = vM;
}

glm::mat4 ikdDataSet::getViewMatrix(void)
{
  return viewMatrix;
}

void ikdDataSet::setModelViewMatrix(glm::mat4 mvM)
{
	modelViewMatrix = mvM;
}

glm::mat4 ikdDataSet::getModelViewMatrix(void)
{
	return modelViewMatrix;
}


void ikdDataSet::setProjectionMatrix(glm::mat4 pM)
{
  projectionMatrix = pM;
}

glm::mat4 ikdDataSet::getProjectionMatrix(void)
{
  return projectionMatrix;
}


//light
void ikdDataSet::setLight(glm::vec3 pos, glm::mat4 lvM, glm::mat4 lpM)
{
	if (lightCount < maxLightNum) {
		lightPosition[lightCount]			= pos;
		lightProjectionMatrix[lightCount]	= lpM;
		lightViewMatrix[lightCount]			= lvM;
		lightCount++;
	} else {
		fprintf(stderr, "you are using too many lights!\n");
	}
}
void ikdDataSet::setLightPosition(int lightNum, glm::vec3 pos)
{
	lightPosition[lightNum] = pos;
}
glm::vec3 ikdDataSet::getLightPosition(int lightNum)
{
	return lightPosition[lightNum];
}
float* ikdDataSet::getLightPosition(const char* str)
{
	if (strcmp(str, "array") == 0) {
		return &lightPosition[0][0];
	} else {
		fprintf(stderr, "please set mode\n");
	}
}
void ikdDataSet::setLightViewMatrix(int lightNum, glm::mat4 lvM)
{
	lightViewMatrix[lightNum] = lvM;
}
glm::mat4 ikdDataSet::getLightViewMatrix(int lightNum)
{
	return lightViewMatrix[lightNum];
}
float* ikdDataSet::getLightViewMatrix(char* str)
{
	if (strcmp(str, "array") == 0) {
		return &lightViewMatrix[0][0][0];
	} else {
		fprintf(stderr, "please set mode.\n");
	}

}
void ikdDataSet::setLightProjectionMatrix(int lightNum, glm::mat4 lpM)
{
	lightProjectionMatrix[lightNum] = lpM;
}
glm::mat4 ikdDataSet::getLightProjectionMatrix(int lightNum)
{
	return lightProjectionMatrix[lightNum];
}

float* ikdDataSet::getLightProjectionMatrix(char* str)
{
	if (strcmp(str, "array") == 0) {
		return &lightProjectionMatrix[0][0][0];
	} else {
		fprintf(stderr, "please set mode.\n");
	}
}
int ikdDataSet::getLightCount(void)
{
	return lightCount;
}
void ikdDataSet::setNearFarInterval(int lightNum, float n, float f, float i)
{
	myClippingNear[lightNum] = n;
	myClippingFar[lightNum] = f;
	layerInterval[lightNum] = i;
}
float ikdDataSet::getNear(int lightNum)
{
	return myClippingNear[lightNum];
}
float ikdDataSet::getFar(int lightNum)
{
	return myClippingFar[lightNum];
}
float ikdDataSet::getInterval(int lightNum)
{
	return layerInterval[lightNum];
}
float* ikdDataSet::getInterval(const char* str)
{
	if (strcmp(str, "array") == 0) {
		return &layerInterval[0];
	} else {
		fprintf(stderr, "please set mode.\n");
	}
}

void ikdDataSet::setMultiLightFlag(int multi)
{
	if (multi == 0 || multi == 1)
		multiLightFlag = multi;
	else 
		printf("please input 0 or 1.\n");
}
int ikdDataSet::getMultiLightFlag(void)
{
	return multiLightFlag;
}
void ikdDataSet::setMaxLightNum(int num)
{
	//最大ライト数の設定
	maxLightNum = num;
	printf("maxLightNum: %d\n", maxLightNum);
	lightPosition.resize(maxLightNum);
	lightViewMatrix.resize(maxLightNum);
	lightProjectionMatrix.resize(maxLightNum);
	myClippingNear.resize(maxLightNum);
	myClippingFar.resize(maxLightNum);
	layerInterval.resize(maxLightNum);
}
int ikdDataSet::getMaxLightNum(void)
{
	return maxLightNum;
}
void ikdDataSet::setVisibleLightNum(int num)
{
	if (num > 0) {
		if (num > lightCount) {
			num = 1;
		}
		visibleLightNum = num;
	} else {
		fprintf(stderr, "setting light number is too big.\n");
	}
}
int ikdDataSet::getVisiblelightNum(void)
{
	return visibleLightNum;
}





//camera
void ikdDataSet::setCameraPosition(glm::vec3 pos)
{
	cameraPosition = pos;
}

glm::vec3 ikdDataSet::getCameraPosition(void)
{
	return cameraPosition;
}

void ikdDataSet::setMouseMove(glm::vec2 mmV)
{
  mouseMove = mmV;
}

glm::vec2 ikdDataSet::getMouseMove(void)
{
  return mouseMove;
}

void ikdDataSet::setBoundingPos(float minPos[], float maxPos[])
{
	//二頂点からバウンディングボックスを計算する

	float width, height, depth;

	width = maxPos[0] - minPos[0];//x
	height = maxPos[1] - minPos[1];//y
	depth = maxPos[2] - minPos[2];//z

	float boundArray[8*3] = {
		minPos[0],			minPos[1],			minPos[2],
		minPos[0]+width,	minPos[1],			minPos[2],
		minPos[0]+width,	minPos[1]+height,	minPos[2],
		minPos[0],			minPos[1]+height,	minPos[2],
		minPos[0],			minPos[1]+height,	minPos[2]+depth,
		minPos[0],			minPos[1],			minPos[2]+depth,
		minPos[0]+width,	minPos[1],			minPos[2]+depth,
		minPos[0]+width,	minPos[1]+height,	minPos[2]+depth};

	memcpy(boundingBox, boundArray, sizeof(float) * 8*3);
}
void ikdDataSet::getBoundingPos(float boundArray[])//ヘアのみ
{
	//バウンディングボックスの座標をコピーする
	memcpy(boundArray, boundingBox, sizeof(float) * 8*3);
}

//hairfile関係
void ikdDataSet::initCyHairFile(const char* fileName)
{
	//hairFile, hairFileHeaderはクラスで定義済み
	int errorCheck = -1;

	errorCheck = hairFile.LoadFromFile(fileName);//まずこれを使う。データのロード
	hairFileHeader = hairFile.GetHeader();

	if (errorCheck < 0) {
		fprintf(stderr, "ale can't open.\n");
		exit(1);
	}

	//show file introduction
	printf("number of hair:\t\t%d\n", hairFileHeader.hair_count);
	printf("number of points:\t%d\n", hairFileHeader.point_count);
	
	if ( hairFile.GetColorsArray() == NULL) printf("ColorsArray\t\t: NULL\n");  
	else printf("ColorsArray\t\t: OK\n"); 
	if ( hairFile.GetPointsArray() == NULL) printf("PointsArray\t\t: NULL\n");
	else printf("PointsArray\t\t: OK\n");
	if ( hairFile.GetSegmentsArray() == NULL) printf("SegmentsArray\t\t: NULL\n");
	else printf("SegmentsArray\t\t: OK\n");
	if ( hairFile.GetThicknessArray() == NULL) printf("ThicknessArray\t\t: NULL\n");
	else printf("ThicknessArray\t\t: OK\n");
	if ( hairFile.GetTransparencyArray() == NULL) printf("TransparencyArray\t: NULL\n");
	else printf("TransparencyArray\t: OK\n");
}

cyHairFileHeader ikdDataSet::getHairFileHeader(void)
{
	return hairFileHeader;
}

cyHairFile ikdDataSet::getHairFile(void)
{
	return hairFile;
}

unsigned short* ikdDataSet::getHairFileSegmentsArray(void)
{
	return hairFile.GetSegmentsArray();
}

float* ikdDataSet::getHairFilePointsArray(void)
{
	return hairFile.GetPointsArray();
}

float* ikdDataSet::getHairFileThicknessArray(void)
{
	return hairFile.GetThicknessArray();
}

float* ikdDataSet::getHairFileTransparencyArray(void)
{
	return hairFile.GetTransparencyArray();
}

float* ikdDataSet::getHairFileColorsArray(void)
{
	return hairFile.GetColorsArray();
}

void ikdDataSet::setHairThickness_Opacity(float th)
{
	if (th < 0) th = 0;
    else if (th > 5) th = 5.0;
	opacityThickness = th;
}
float ikdDataSet::getHairThickness_Opacity(void)
{
	return opacityThickness;
}

void ikdDataSet::setHairThickness_Rendering(float th)
{
	if (th < 0) th = 0;
    else if (th > 5) th = 5.0;
	renderingThickness = th;
}
float ikdDataSet::getHairThickness_Rendering(void)
{
	return renderingThickness;
}


//hair関係

void ikdDataSet::setOSMNum(int in)
{
	if (in < 0 || in > OSM_COUNT_MAX) {
		fprintf(stderr, "number of OSM is over OSM_COUNT_MAX\n");
	} else {
		osmNum = in;
	}
}

int ikdDataSet::getOSMNum(void)
{
	return osmNum;
}

void ikdDataSet::setOSMArray(float osmArray[], char* near_far)
{
	if (osmNum < -1) {
		fprintf(stderr, "please set number of Opacity Shadow Map.\n");
	}
	else if (strcmp(near_far, "near") == 0) {
		memcpy(osmNearArray, osmArray, sizeof(float) * osmNum);
	} else if (strcmp(near_far, "far") == 0) {
		memcpy(osmFarArray, osmArray, sizeof(float) * osmNum);
	}
}

float* ikdDataSet::getOSMArray(char* near_far)
{
	if (strcmp(near_far, "near") == 0) {
		return osmNearArray;
	} else if (strcmp(near_far, "far") == 0) {
		return osmFarArray;
	}
}

void ikdDataSet::setShadowState(int on_off)
{
	shadow_noshadow = on_off;
}
int ikdDataSet::getShadowState(void)
{
	return shadow_noshadow;
}
void ikdDataSet::setUseShaderNum(int num)
{
	if (num >= progHandleCount) {
		fprintf(stderr, "その番号のシェーダは存在しない\n");
	} else {
		useShaderNumber = num;
	}
}
int ikdDataSet::getUseShaderNum(void)
{
	return useShaderNumber;
}

//DOSM
void ikdDataSet::setOpacityValue(float oV)
{
	if (oV > 1.0) oV = 1.0;
	opacityValue = oV;
}
float ikdDataSet::getOpacityValue(void)
{
	return opacityValue;
}
void ikdDataSet::setDosmLastPosPlaneSize(float planeSize)
{
	if (planeSize <= 0) {
		std::cout << "please input positive number." << std::endl;
	} else {
		dosmLastPosPlaneSize = planeSize;
	}
}
float ikdDataSet::getDosmLastPosPlaneSize(void)
{
	return dosmLastPosPlaneSize;
}







void ikdDataSet::setDepthMapSize(int width, int height)
{
	depthMapWidth = width;
	depthMapHeight = height;
}
int ikdDataSet::getDepthMapWidth(void)
{
	if (depthMapWidth < 0 || depthMapHeight < 0) {
		fprintf(stderr, "please set depthmap width or height.\n");
	} else {
		return depthMapWidth;
	}
}
int ikdDataSet::getDepthMapHeight(void)
{
	if (depthMapWidth < 0 || depthMapHeight < 0) {
		fprintf(stderr, "please set depthmap width or height.\n");
	} else {
		return depthMapHeight;
	}
}

//kajiya-kay
void ikdDataSet::setKajiyaShininess(float shin1, float shin2)
{
	shininessOne = shin1;
	shininessTwo = shin2;
}
float ikdDataSet::getKajiyaShininess(int shininess_One_or_Two)
{
	if (shininess_One_or_Two != 1 && shininess_One_or_Two != 2) {
		fprintf(stderr, "please set 1 or 2.\n");
		return 0.0;
	}
	else if (shininess_One_or_Two == 1) return shininessOne;
	else return shininessTwo;
}
void ikdDataSet::setKajiyaSpecularWeight(float spec1, float spec2)
{
	specularWeightOne = spec1;
	specularWeightTwo = spec2;
}
float ikdDataSet::getKajiyaSpecularWeight(int specularWeight_One_or_Two)
{
	if (specularWeight_One_or_Two != 1 && specularWeight_One_or_Two != 2) {
		fprintf(stderr, "please set 1 or 2.\n");
		return 0.0;
	}
	else if (specularWeight_One_or_Two == 1) return specularWeightOne;
	else return specularWeightTwo;
}
void ikdDataSet::setKajiyaDiffuseWeight(float diffWei)
{
	diffuseWeight = diffWei;
}
float ikdDataSet::getKajiyaDiffuseWeight(void)
{
	return diffuseWeight;
}
void ikdDataSet::setKajiyaDiffuseColor(glm::vec3 diffCol)
{
	diffuseColor = diffCol;
}
glm::vec3 ikdDataSet::getKajiyaDiffuseColor(void)
{
	return diffuseColor;
}
void ikdDataSet::setKajiyaSpecularColor(glm::vec3 specCol1, glm::vec3 specCol2)
{
	specularColorOne = specCol1;
	specularColorTwo = specCol2;
}
glm::vec3 ikdDataSet::getKajiyaSpecularColor(int specularColor_One_or_Two)
{
	if (specularColor_One_or_Two != 1 && specularColor_One_or_Two != 2) {
		fprintf(stderr, "please set 1 or 2.\n");
		return glm::vec3(0.0, 0.0, 0.0);
	}
	else if (specularColor_One_or_Two == 1) return specularColorOne;
	else return specularColorTwo;
}

void ikdDataSet::copyAllData(ikdDataSet* dataSet, ikdDataSet* motoDataSet)
{

	dataSet->nowCameraLookAt	=motoDataSet->nowCameraLookAt;
	dataSet->cameraAngle		=motoDataSet->cameraAngle	;
	dataSet->modelMatrix		=motoDataSet->modelMatrix;
	dataSet->viewMatrix			=motoDataSet->viewMatrix;
	dataSet->modelViewMatrix	=motoDataSet->modelViewMatrix;
	dataSet->projectionMatrix	=motoDataSet->projectionMatrix;
	dataSet->cameraPosition		=motoDataSet->cameraPosition;
	dataSet->mouseMove			=motoDataSet->mouseMove;
}

void ikdDataSet::deleteTexture(const int deleteTextureNum)
{
	glDeleteTextures(1, &texHandle[deleteTextureNum]);
	texHandleCount--;
	nowTexHandleNum--;
}
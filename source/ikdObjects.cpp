#include "ikdObjects.h"


GLuint ikdObjects::ikdTriangle()
{
	float positionData[] = { -0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f,  0.8f, 0.0f};

	float colorData[] = { 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f};

	//create vertex buffer object
	GLuint vboHandle[2];
	glGenBuffers(2, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint colorBufferHandle = vboHandle[1];

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	//色バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は色バッファ
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

void ikdObjects::ikdGetVertexBox(float boxVert[], float boxNormal[], float boxTexCoord[])
{
	int i, j, k;
	int index;

	float vertices[8][3] = {-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f};

	int surfaces[12][3] = {
		0,3,2, 2,1,0,
		0,1,5, 5,4,0,
		0,4,7, 7,3,0,
		1,2,6, 6,5,1,
		5,6,7, 7,4,5,
		2,3,7, 7,6,2};

	float texCoords[4][2] = {0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f};

	int texSurfaces[2][3] = {0,3,2, 2,1,0};

	//頂点配列
	index = 0;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 3; j++) {
			int ver = surfaces[i][j];
			memcpy(boxVert+index, vertices[ver], sizeof(float)*3);
			index += 3;
		}
	}

	//テクスチャ座標
	index = 0;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 2; j++) {
			for (k = 0; k < 3; k++) {
				int ver = texSurfaces[j][k];
				memcpy(boxTexCoord+index, texCoords[ver], sizeof(float)*2);
				index += 2;
			}
		}
	}


	//法線情報
	index = 0;
	for (i = 0; i < 12; i++) {
		glm::vec3 p1 = glm::vec3(boxVert[i*9+0], boxVert[i*9+1], boxVert[i*9+2]);
		glm::vec3 p2 = glm::vec3(boxVert[i*9+3], boxVert[i*9+4], boxVert[i*9+5]);
		glm::vec3 p3 = glm::vec3(boxVert[i*9+6], boxVert[i*9+7], boxVert[i*9+8]);

		glm::vec3 e1 = p2 - p1;
		glm::vec3 e2 = p3 - p2;

		glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

		for (j = 0; j < 3; j++) {
			memcpy(boxNormal+index, &normal[0], sizeof(float)*3);
			index += 3;
		}
	}

}

GLuint ikdObjects::ikdBox(const float width, const float height, const float depth)
{
	float boxVert[3*3*12];
	float boxNormal[3*3*12];
	float boxTexCoord[2*3*12];



	//頂点の取得
	ikdGetVertexBox(boxVert, boxNormal, boxTexCoord);

	//サイズの設定
	for (int i = 0; i < 3*12; i++) {
		boxVert[i*3] *= width;
		boxVert[i*3+1] *= height;
		boxVert[i*3+2] *= depth;
	}

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle	  = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*12 * sizeof(float), boxVert, GL_STATIC_DRAW);

	//法線情報バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*12 * sizeof(float), boxNormal, GL_STATIC_DRAW);

	//テクスチャ位置バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*12 * sizeof(float), boxTexCoord, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線情報バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス2はテクスチャ頂点バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdBox(ikdDataSet* pdataSet)
{
	const int pointsNum = 12*2*3;
	float boxPoints[8*3];
	pdataSet->getBoundingPos(boxPoints);

	int boxLines[12*2] = {
		0,1, 1,2, 2,3, 3,0,
		4,5, 5,6, 6,7, 7,4,
		3,4, 0,5, 2,7, 1,6};

	float boxVertices[pointsNum];

	int index = 0;
	for (int i = 0; i < 12*2; i++) {
		memcpy(boxVertices+index, &boxPoints[boxLines[i]*3], sizeof(float)*3);
		index += 3;
	}


	//create vertex buffer object
	GLuint vboHandle[1];
	glGenBuffers(1, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pointsNum * sizeof(float), boxVertices, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);	

	return vaoHandle;
}

void ikdObjects::ikdGetVertexPlane(float planeVert[], float planeNormal[], float planeTexCoord[])
{
	int i, j;
	int index;

	float vertices[4][3] = {-0.5, 0.0, -0.5,
		0.5, 0.0, -0.5,
		0.5, 0.0,  0.5,
		-0.5, 0.0, 0.5};

	int surfaces[2][3] = {0,1,2, 2,3,0};

	float texCoords[4][2] = {1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f};

	index = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			int ver = surfaces[i][j];
			memcpy(planeVert+index, vertices[ver], sizeof(float)*3);
			index += 3;
		}
	}


	index = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) { 
			int ver = surfaces[i][j];
			memcpy(planeTexCoord+index, texCoords[ver], sizeof(float)*2);
			index += 2;
		}
	}

	//法線情報の設定
	float normal[] = {0.0, 1.0, 0.0};

	index = 0;
	for (i = 0; i < 6; i++) {
		memcpy(planeNormal+index, normal, sizeof(float)*3);
		index += 3;
	}

}

GLuint ikdObjects::ikdPlane(const float width, const float depth)
{

	float planeVert[3*3*2];
	float planeNormal[3*3*2];
	float planeTexCoord[2*3*2];

	ikdGetVertexPlane(planeVert, planeNormal, planeTexCoord);

	for (int i = 0; i < 6; i++) {
		planeVert[i*3] *= width;
		planeVert[i*3+2] *= depth;
	}



	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle   = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];


	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeVert, GL_STATIC_DRAW);

	//法線情報バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeNormal, GL_STATIC_DRAW);

	//テクスチャ位置バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*2 * sizeof(float), planeTexCoord, GL_STATIC_DRAW);


	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線情報バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス2はテクスチャ頂点バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdScalablePlane(void)
{
	
	float planeVert[3*3*2];
	float planeNormal[3*3*2];
	float planeTexCoord[2*3*2];
	float planeTangent[3*3*2];
	float planeMoveVec[2*3*3] = {
		-0.5, 0.0, -0.5,
		0.5, 0.0, -0.5,
		0.5, 0.0, 0.5,

		0.5, 0.0, 0.5,
		-0.5, 0.0, 0.5,
		-0.5, 0.0, -0.5
		};

	ikdGetVertexPlane(planeVert, planeNormal, planeTexCoord);

	float startPos[] = {0.0, 0.0, 0.0};//初期点はとりあえずすべて0とする
	for (int i = 0, index = 0; i < 2*3; i++, index += 3) {
		memcpy(planeTangent+index, startPos, sizeof(float) * 3);
		memcpy(planeVert+index, startPos, sizeof(float)*3);
	}	

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle   = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle	= vboHandle[3];
	GLuint moveVecBufferHandle	= vboHandle[4];

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeVert, GL_STATIC_DRAW);

	//法線情報バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3*3*2 * sizeof(float), planeNormal, GL_STATIC_DRAW);

	
	//テクスチャ位置バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*2 * sizeof(float), planeTexCoord, GL_STATIC_DRAW);
	
	//タンジェントを送る
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*3 * sizeof(float), planeTangent, GL_STATIC_DRAW);

	//拡大する方向
	glBindBuffer(GL_ARRAY_BUFFER, moveVecBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2*3*3 * sizeof(float), planeMoveVec, GL_STATIC_DRAW);
	

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);


	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線情報バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	
	//インデックス2はテクスチャ頂点バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	
	//インデックス3はタンジェントバッファ
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス4は方向決め用のバッファ
	glBindBuffer(GL_ARRAY_BUFFER, moveVecBufferHandle);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	

	return vaoHandle;
}

GLuint ikdObjects::ikdLoadObject(const char* fileName)
{
	int vertexNum = 0, normalNum = 0, texCoordNum = 0, faceNum = 0;
	char buf[200];


	//ファイルからのデータのロード/////////////////////////////////////////////////
	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		fprintf(stderr, "%s を読み込めません\n", fileName);

	}

	while (fgets(buf, 200, fp) != NULL) {
		switch(buf[0]) {
		case 'v':
			if (buf[1] == 't') texCoordNum++;
			else if (buf[1] == 'n') normalNum++;
			else vertexNum++;
			break;
		case 'f':
			faceNum++;
			break;
		default:
			break;
		}
	}

	float * vertices = (float *) malloc(sizeof(float)*vertexNum*3);
	float * normals = (float *)malloc(sizeof(float)*normalNum*3);
	float * texCoords = (float *)malloc(sizeof(float)*texCoordNum*2);
	//int * vertFaces = (int *)malloc(sizeof(int)*faceNum*4);
	int * vertFaces = (int *)malloc(sizeof(int)*faceNum*3);
	int * normalFaces = (int *)malloc(sizeof(int)*faceNum*4);
	int * texFaces = (int *)malloc(sizeof(int)*faceNum*4);

	rewind(fp);


	fprintf(stderr, "vertexNum: %d, faceNum: %d\n", vertexNum, faceNum);


	int vertindex = 0, normalindex = 0, texindex = 0, faceindex = 0;
	while (fgets(buf, 50, fp) != NULL) {

		switch(buf[0]) {
		case 'v':
			if (buf[1] == 't') {
				sscanf(buf, "vt %f %f", &texCoords[texindex + 0], &texCoords[texindex + 1]);
				texindex += 2;
			}
			else if (buf[1] == 'n') {
				sscanf(buf, "vn %f %f %f", &normals[normalindex + 0], &normals[normalindex + 1], &normals[normalindex + 2]);
				normalindex += 3;
			}
			else {
				sscanf(buf, "v %f %f %f", &vertices[vertindex + 0], &vertices[vertindex + 1], &vertices[vertindex + 2]);
				vertindex += 3;
			}
			break;
		case 'f':
			/*
			テクスチャ、法線は省略可能
			・頂点/テクスチャ/法線４
			・頂点//法線(//)2
			・頂点/テクスチャ（/一つ）3
			・頂点(/無し)１

			複数の頂点が指定されていたら、多角形を表す。
			*/
			{

				/*
				//要素数をカウント
				int valNum = 0;
				for (int i = 0; i < 200 || buf[i] != '\n'; i++) {
				if (buf[i] == ' ') valNum++;//スペース一つにつき要素が一つだから、これでいい。
				}
				*/

				if (strchr(buf, '/') == NULL) {
					//'/'が無ければ、頂点のみ。
					/*sscanf(buf, "f %d %d %d %d",
					&vertFaces[faceindex + 0],
					&vertFaces[faceindex + 1],
					&vertFaces[faceindex + 2],
					&vertFaces[faceindex + 3]);*/
					sscanf(buf, "f %d %d %d",
						&vertFaces[faceindex + 0],
						&vertFaces[faceindex + 1],
						&vertFaces[faceindex + 2]);

				} else if (strstr(buf, "//") != NULL) {
					//"//"があれば、頂点、法線
					sscanf(buf, "f %d//%d %d//%d %d//%d %d//%d",
						&vertFaces[faceindex + 0], &normalFaces[faceindex + 0],
						&vertFaces[faceindex + 1], &normalFaces[faceindex + 1],
						&vertFaces[faceindex + 2], &normalFaces[faceindex + 2],
						&vertFaces[faceindex + 3], &normalFaces[faceindex + 3]);


				} else {
					//'/'があるが、"//"はない　'/'の数でどちらのパターンかを判断。
					char typecheck[100];//数万点を扱う時の為に、一応100文字分のバッファ
					int slashNum = 0;
					sscanf(buf+2, "%s", typecheck);//読み取った一行の、最初の要素を取り出す。
					for (int i = 0; i < 100; i++) {
						if (typecheck[i] == '/') slashNum++;//１要素だけに関して、'/'がいくつあるかカウント
					}

					if (slashNum == 1) {
						//頂点、テクスチャ
						sscanf(buf, "f %d/%d %d/%d %d/%d %d/%d",
							&vertFaces[faceindex + 0], &texFaces[faceindex + 0],
							&vertFaces[faceindex + 1], &texFaces[faceindex + 1],
							&vertFaces[faceindex + 2], &texFaces[faceindex + 2],
							&vertFaces[faceindex + 3], &texFaces[faceindex + 3]);

					} else if (slashNum == 2) {
						//頂点、テクスチャ、法線

						sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
							&vertFaces[faceindex + 0], &texFaces[faceindex + 0], &normalFaces[faceindex + 0],
							&vertFaces[faceindex + 1], &texFaces[faceindex + 1], &normalFaces[faceindex + 1],
							&vertFaces[faceindex + 2], &texFaces[faceindex + 2], &normalFaces[faceindex + 2],
							&vertFaces[faceindex + 3], &texFaces[faceindex + 3], &normalFaces[faceindex + 3]);
					}



				}

				//faceindex += 4;
				faceindex += 3;


			}
			break;
		default:
			break;
		}
	}
	fclose(fp);
	//データのロード終了------------------------------------------------------------------------------------


	//実際の頂点データ作成
	//float * objVertices = (float *)malloc(sizeof(float) * faceNum * 4 * 3);//四点で1面
	float * objVertices = (float *)malloc(sizeof(float) * faceNum * 3 * 3);//三点で1面
	float * objNormals = (float *)malloc(sizeof(float) * faceNum * 3 * 3);
	float * objTexCoords = (float *)malloc(sizeof(float) * faceNum * 4 * 2);
	float * objTangents = (float *)malloc(sizeof(float) * faceNum * 3 * 3);


	//頂点配列
	/*int index = 0;
	for (int i = 0; i < faceNum; i++) {
	for (int j = 0; j < 4; j++) {
	int ver = vertFaces[i*4+j];
	memcpy(objVertices+index, &vertices[ver], sizeof(float)*3);
	index += 3;
	}
	}*/


	int index = 0;
	for (int i = 0; i < faceNum; i++) {//faceNum -> fの行数
		for (int j = 0; j < 3; j++) {
			int ver = (vertFaces[i*3+j] - 1) * 3;//一点につき三点持っている。
			memcpy(objVertices+index, &vertices[ver], sizeof(float)*3);
			index += 3;
		}
	}


	//法線座標
	index = 0;
	for (int i = 0; i < faceNum; i++) {//面の数だけ同じ処理をする 回数だけ分かればいい
		glm::vec3 p1 = glm::vec3(objVertices[i*9+0], objVertices[i*9+1], objVertices[i*9+2]);
		glm::vec3 p2 = glm::vec3(objVertices[i*9+3], objVertices[i*9+4], objVertices[i*9+5]);
		glm::vec3 p3 = glm::vec3(objVertices[i*9+6], objVertices[i*9+7], objVertices[i*9+8]);

		glm::vec3 e1 = p2 - p1;
		glm::vec3 e2 = p3 - p2;

		glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

		for (int j = 0; j < 3; j++) {
			memcpy(objNormals+index, &normal[0], sizeof(float)*3);
			index += 3;
		}
	}
	/*
	//テクスチャ座標
	index = 0;
	for (int i = 0; i < faceNum; i++) {
	for (int j = 0; j < 4; j++) {
	int ver = texFaces[i*4 + j];
	memcpy(objTexCoords+index, &texCoords[ver], sizeof(float)*2);
	index += 2;
	}
	}
	*/

	

	//create vertex buffer object
	//GLuint vboHandle[3];
	//glGenBuffers(3, vboHandle);
	GLuint vboHandle[4];
	glGenBuffers(4, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle   = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle  = vboHandle[3];


	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, faceNum * 3 * 3 * sizeof(float), objVertices, GL_STATIC_DRAW);//四点で面


	//法線情報バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, faceNum * 3 * 3 * sizeof(float), objNormals, GL_STATIC_DRAW);

	//タンジェントバッファにデータを代入
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, faceNum * 3 * 3 * sizeof(float), objTangents, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);


	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線情報バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス2はテクスチャ頂点バッファ
	//glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	
	//インデックス3はタンジェント情報バッファ
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);


	free(vertices);
	free(normals);
	free(texCoords);
	free(vertFaces);
	free(texFaces);
	free(normalFaces);
	free(objVertices);
	free(objNormals);
	free(objTexCoords);
	free(objTangents);

	return vaoHandle;
}

GLuint ikdObjects::ikdLoad_cyHairFile(ikdDataSet* pdataSet, float thickness)
{
	
	//可変予定パラメータ
	const int partitions = 10;//一本の髪をいくつに区切るか
	const int surfaces = 12;//各セグメントを何面で作るか
	const float radius = thickness;//柱図形の半径

	//dataSetに含める可能性あり
	const int meshedDataNum = partitions*surfaces*3*3*(pdataSet->getHairFileHeader().hair_count);//メッシュ化した後の全データ数(各頂点のxyzまで)
	const int meshedVertexNum = partitions*surfaces*3*(pdataSet->getHairFileHeader().hair_count);//頂点数
	const int meshedDataNum_onehair = partitions*surfaces*3*3;
	const int meshedVertexNum_onehair = partitions*surfaces*3;



	
	const float* pointsArray = pdataSet->getHairFilePointsArray();
	float * meshedHair = new float[meshedDataNum];//すべての髪にpartitionsパート, surfaces/2面(一面に二枚の三角形), 3頂点, xyzのデータ
	float * meshedHairNormal = new float[meshedDataNum];//髪を描く際, 全頂点に対して法線を設定する
	float * meshedHairTangent = new float[meshedDataNum];//髪の流れの方向のベクトル

	int nowHairStartArrayNum = 0, nowHairEndArrayNum;

	const unsigned short* segmentsArray = pdataSet->getHairFileSegmentsArray();
	int defaultSegs = 0;
	if (segmentsArray) {
		nowHairEndArrayNum = segmentsArray[0];//これから描く髪の始点と終点
	} else {//segmentsArrayがnullのとき
		defaultSegs = pdataSet->getHairFileHeader().d_segments;
		nowHairEndArrayNum = defaultSegs;
	}

	//前もって、基本となる六角形を作っておく///////////////////////////////////////////////////////////////////////
	glm::vec4 oneHexPoint = glm::vec4(0.0, radius, 0.0, 1.0);//六角形を構成する点はこの点を基準に作る。
	glm::mat4 hexRotate = glm::rotate<float>(glm::mat4(1.0f), 360 / 6, glm::vec3(0.0, 0.0, 1.0));//各点を指定するための回転行列
	float originalHexVertices[6*3];//原点にある六角形
	for (int i = 0, index = 0; i < 6; i++, index+=3) {
		oneHexPoint = hexRotate * oneHexPoint;//回転に関して積算していく。
		memcpy(originalHexVertices+index, &oneHexPoint[0], sizeof(float)*3);
	}

	//----------------------------------------------------------------------------------------------------------------

	
	
	for (unsigned int nowHair = 0; nowHair < pdataSet->getHairFileHeader().hair_count; nowHair++) {//髪一本一本について
		if (segmentsArray) {
			nowHairEndArrayNum = nowHairStartArrayNum + segmentsArray[nowHair];//現在注目している毛の最後の頂点
		} else {
			nowHairEndArrayNum = nowHairStartArrayNum + defaultSegs;
		}
		float * hexCylinderVert = new float[7*3*(partitions+1)];//一本の髪をpartitions回分割して考える。どんな枚数になっても、最後に一枚追加する。

		//六角形を描く座標だけの配列にする。/////////////////////////////////////////////////////
		int pickupInterval;
		if (segmentsArray){
			pickupInterval = segmentsArray[nowHair] / partitions;//間引いて考えるため、一定間隔で点を抽出する。 
		} else {
			pickupInterval = defaultSegs / partitions;
		}
		float * pickedPointsArray = new float[3*(partitions+1)];

		for (int i = 0; i < (partitions + 1); i++) {
			memcpy(&pickedPointsArray[i*3], &pointsArray[nowHairStartArrayNum*3 + i*3*pickupInterval], sizeof(float)*3);
		}		
		//--------------------------------------------------------------------------------------//

		//六角形を描く//////////////////////////////////////////////////////////////////////////////////////
		glm::vec3 prevHexApper = glm::vec3(0.0, 1.0, 0.0);//基本形の上方向
		for (int segmentFace = 0; segmentFace < (partitions + 1); segmentFace++) {//各六角形に関して
			glm::vec3 p1, p2;
			if (segmentFace == partitions) {
				//最後の一枚になった時
				p1 = glm::vec3(pointsArray[nowHairEndArrayNum*3], 
					pointsArray[nowHairEndArrayNum*3 + 1], 
					pointsArray[nowHairEndArrayNum*3 + 2]);

				p2 = glm::vec3(pointsArray[nowHairEndArrayNum*3-3], 
					pointsArray[nowHairEndArrayNum*3-3+1], 
					pointsArray[nowHairEndArrayNum*3-3+2]);				
			} else {		
				p1 = glm::vec3(pickedPointsArray[segmentFace * 3], 
					pickedPointsArray[segmentFace * 3 + 1], 
					pickedPointsArray[segmentFace * 3 + 2]);//六角柱を作る部分の先頭

				p2 = glm::vec3(pickedPointsArray[segmentFace * 3 + 3], 
					pickedPointsArray[segmentFace * 3 + 4], 
					pickedPointsArray[segmentFace * 3 + 5]);//六角柱を作る部分の終点
			}

			//座標変換のためのベクトル設定////////////////////////////////////////////////////////////////////////////////
			glm::vec3 hexApper;
			if (segmentFace == 0) {
				hexApper = glm::vec3(0.0, 1.0, 0.0);//六角形のapper y
			} else {
				hexApper = prevHexApper;
			}
			
			float hexVertices[6*3];//現時点での六角形の頂点を保存するための配列。

			glm::mat4 hexTranslate = glm::translate<float>(glm::mat4(1.0f), glm::vec3(p1));//髪のセグメントへの移動行列

			glm::vec3 segNormal;
			if (segmentFace == partitions) {
				segNormal = glm::normalize(p1 - p2);//合わせたい法線 z'
				segNormal = glm::vec3(-segNormal[0],-segNormal[1],-segNormal[2]);
			} else {
				segNormal = glm::normalize(p2 - p1);//合わせたい法線 z'
			}
			
			glm::vec3 crossVec = glm::normalize(glm::cross(hexApper, segNormal));//x'

			glm::vec3 comvertedHexApper = glm::cross(segNormal, crossVec);//y'

			//基底変換行列　任意座標系に対して正しく回転させるような行列
			glm::mat4 hexCoordComvert4(glm::vec4(crossVec[0], comvertedHexApper[0], segNormal[0], 0),
				glm::vec4(crossVec[1], comvertedHexApper[1], segNormal[1], 0),
				glm::vec4(crossVec[2], comvertedHexApper[2], segNormal[2], 0),
				glm::vec4(0, 0, 0, 1));

			hexCoordComvert4 = glm::transpose(hexCoordComvert4);
			//座標変換のためのベクトル設定----------------------------------------------------------------------------------

			//頂点六個の座標の計算		
			for (int i = 0; i < 6; i++) {
				glm::vec4 changePoint = glm::vec4(originalHexVertices[i*3],originalHexVertices[i*3+1],originalHexVertices[i*3+2], 1.0);
				changePoint = hexTranslate * hexCoordComvert4 * changePoint;//積算されては困る。

				memcpy(&hexVertices[i*3], &changePoint[0], sizeof(float) * 3);//周りの六頂点
			}

			//上で作った頂点6個を配列にまとめる。
			int index = 0;
			for (int i = 0; i < 7; i++) {//hex[]に、各面を構成する頂点を入力する。
				if (i == 6) {//一周したときは、0と全く同じ座標を入れる。
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[0], sizeof(float) * 3);
				} else {
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[i * 3], sizeof(float) * 3);
				}
				index += 3;
			}
			prevHexApper = comvertedHexApper;
		}//各六角形に関して---------------------------------------------------------------------------------------------

		delete[] pickedPointsArray;




		//この時点で、hexCylinderVertに髪一本の六角形の頂点がすべて保存されている。
		//データを抽出して、メッシュを構成する頂点を指定する/////////////////////////////////////////////////////////////////
		float * hexCylinder = new float[meshedDataNum_onehair];//hexcylindervertから、現在の三角形を描くのに必要な頂点を採用する
	
		for (int i = 0, index = 0; i < partitions; i++) {//各セグメントについて行う
			//上部分の三角六枚
			for (int j = i*7; j < i*7+6; j++) {
				memcpy(&hexCylinder[index + 0], &hexCylinderVert[j*3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 3], &hexCylinderVert[(j+1) * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 6], &hexCylinderVert[(j+7) * 3], sizeof(float)*3);
				index += 9;
			}

			//下部分の三角六枚
			for (int j = (i+1)*7; j < (i+1)*7+6; j++) {
				memcpy(&hexCylinder[index + 0], &hexCylinderVert[j*3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 3], &hexCylinderVert[(j-6)*3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 6], &hexCylinderVert[(j+1)*3], sizeof(float)*3);
				index += 9;
			}
		}

		memcpy(&meshedHair[meshedDataNum_onehair*nowHair], hexCylinder, sizeof(float)*meshedDataNum_onehair);
		//-------------------------------------------------------------------------------------------------------------------


		//hexCylindervertの座標を利用して、各面の法線を設定する。
		int normalIndex = 0;
		for (int i = 0; i < partitions; i++) {//各六角柱に関して
			float hexNormals[6*3];
			int index = 0;
			for (int j = i*7; j < i*7+6; j++) {//各面に関して法線ベクトルを作成
				glm::vec3 p1, p2, p3;
				memcpy(&p1[0], &hexCylinderVert[j*3], sizeof(float)*3);
				memcpy(&p2[0], &hexCylinderVert[(j+1)*3], sizeof(float)*3);
				memcpy(&p3[0], &hexCylinderVert[(j+7)*3], sizeof(float)*3);

				glm::vec3 e1 = p2 - p1;
				glm::vec3 e2 = p3 - p2;

				glm::vec3 normal = glm::normalize(glm::cross(e1, e2));//現在の面の法線が出力される

				memcpy(hexNormals+index, &normal[0], sizeof(float)*3);

				index += 3;
			}

			for (int j = 0; j < 2; j++) {//作成した法線ベクトルを送る配列にコピー
				for (int k = 0; k < 6; k++) {
					memcpy(&meshedHairNormal[meshedDataNum_onehair*nowHair+normalIndex],   &hexNormals[k*3], sizeof(float)*3);
					memcpy(&meshedHairNormal[meshedDataNum_onehair*nowHair+normalIndex+3], &hexNormals[k*3], sizeof(float)*3);
					memcpy(&meshedHairNormal[meshedDataNum_onehair*nowHair+normalIndex+6], &hexNormals[k*3], sizeof(float)*3);

					normalIndex += 9;
				}
			}

		
		}
		
		
		//各頂点におけるtangentベクトルを計算する
		int tangentIndex =0;
		for (int i = 0; i < partitions; i++) {
			glm::vec3 tangent;	
			int index = 0;

			glm::vec3 p1, p2;
			memcpy(&p1[0], &hexCylinderVert[i*7*3], sizeof(float)*3);
			memcpy(&p2[0], &hexCylinderVert[(i+1)*7*3], sizeof(float)*3);

			tangent = p2 - p1;

			for (int j = 0; j < 2; j++) {//六角柱の上下
				for (int k = 0; k < 6; k++) {//各側から三角形が6個描かれている
					memcpy(&meshedHairTangent[meshedDataNum_onehair*nowHair+tangentIndex],   &tangent[0], sizeof(float)*3);
					memcpy(&meshedHairTangent[meshedDataNum_onehair*nowHair+tangentIndex+3], &tangent[0], sizeof(float)*3);
					memcpy(&meshedHairTangent[meshedDataNum_onehair*nowHair+tangentIndex+6], &tangent[0], sizeof(float)*3);

					tangentIndex += 9;
				}
			}
		}
		

		//-------------------------------------------------------------------------------------------------------------------

		delete[] hexCylinderVert;
		delete[] hexCylinder;


		if (segmentsArray) {
			nowHairStartArrayNum += segmentsArray[nowHair]+1;
		} else {
			nowHairStartArrayNum += defaultSegs + 1;
		}





	}//髪一本一本について-------------------------------------------------------------------------------------------
	
	/*
	glm::vec3 test = glm::vec3(1.0, 1.0, 1.0);
	int index = 0;
	for (int i = 0; i < meshedVertexNum; i++) {
		memcpy(meshedHairTangent+index, &test[0], sizeof(float)*3);
		index += 3;
	}
	*/



	//バウンディングボックスを作るための座標を保存。
	float min[3], max[3];
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	//バウンディングボックスの計算用座標を取得しておく
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[4];
	glGenBuffers(4, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];



	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//法線バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);
	
	//tangentバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), NULL, GL_STATIC_DRAW);

	//tangentバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairTangent, GL_STATIC_DRAW);
	
	delete[] meshedHair;
	delete[] meshedHairNormal;
	delete[] meshedHairTangent;
	
	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス2はテクスチャ座標バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス3はtangentバッファ
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdScalable_cyHairFile(ikdDataSet* pdataSet)
{


  //可変予定パラメータ
    const int partitions = 10;//一本の髪をいくつに区切るか
	const int surfaces = 6*2;//各セグメントを何面で作るか
	const float radius = 0.1;//柱図形の半径

	//dataSetに含める可能性あり
	const int meshedDataNum = partitions*surfaces*3*3*(pdataSet->getHairFileHeader().hair_count);//メッシュ化した後の全データ数(各頂点のxyzまで)
	const int meshedVertexNum = partitions*surfaces*3*(pdataSet->getHairFileHeader().hair_count);//頂点数
	const int meshedDataNum_onehair = partitions*surfaces*3*3;
	const int meshedVertexNum_onehair = partitions*surfaces*3;



	const unsigned short* segmentsArray = pdataSet->getHairFileSegmentsArray();
	const float* pointsArray = pdataSet->getHairFilePointsArray();
	float * meshedHair = new float[meshedDataNum];//すべての髪にpartitionsパート, surfaces/2面(一面に二枚の三角形), 3頂点, xyzのデータ
	float * meshedHairNormal = new float[meshedDataNum];//髪を描く際, 全頂点に対して法線を設定する
	float * meshedHairTangent = new float[meshedDataNum];//髪の流れの方向のベクトル
	int nowHairStartArrayNum = 0, nowHairEndArrayNum;//これから描く髪の始点と終点
	int defaultSegs = 0;
	if (segmentsArray) {
		nowHairEndArrayNum = segmentsArray[0];//これから描く髪の始点と終点
	} else {//segmentsArrayがnullのとき
		defaultSegs = pdataSet->getHairFileHeader().d_segments;
		nowHairEndArrayNum = defaultSegs;
	} 



	//前もって、基本となる六角形を作っておく///////////////////////////////////////////////////////////////////////
	glm::vec4 oneHexPoint = glm::vec4(0.0, radius, 0.0, 1.0);//六角形を構成する点はこの点を基準に作る。
	glm::mat4 hexRotate = glm::rotate<float>(glm::mat4(1.0f), 360 / 6, glm::vec3(0.0, 0.0, 1.0));//各点を指定するための回転行列
	float originalHexVertices[6*3];//原点にある六角形
	for (int i = 0, index = 0; i < 6; i++, index+=3) {
		oneHexPoint = hexRotate * oneHexPoint;//回転に関して積算していく。
		memcpy(originalHexVertices+index, &oneHexPoint[0], sizeof(float)*3);
	}



	for (unsigned int nowHair = 0; nowHair < pdataSet->getHairFileHeader().hair_count; nowHair++) {//髪一本一本について
      if (segmentsArray) {
		  nowHairEndArrayNum = nowHairStartArrayNum + segmentsArray[nowHair];//現在注目している毛の最後の頂点
	  } else {
		  nowHairEndArrayNum = nowHairStartArrayNum + defaultSegs;
	  }
        //配置した六角形の頂点保存用
		float * hexCylinderVert = new float[7*3*(partitions+1)];

		int pickupInterval = 0;
        //六角形を描く頂点を選び出す
		if (segmentsArray) {
	        pickupInterval = segmentsArray[nowHair] / partitions;//間引いて考えるため、一定間隔で点を抽出する。 
		} else {
			pickupInterval = defaultSegs / partitions;
		}
		float * pickedPointsArray = new float[3*(partitions+1)];
		for (int i = 0, interval = 0; i < (partitions + 1); i++, interval += 3) {
			if (i == partitions) memcpy(pickedPointsArray + interval, &pointsArray[nowHairEndArrayNum * 3], sizeof(float) * 3);
			else memcpy(pickedPointsArray + interval, &pointsArray[nowHairStartArrayNum * 3 + i*3*pickupInterval], sizeof(float)*3);
		}		



        //ベクトル生成用
        glm::vec3 prevHexApper = glm::vec3(0.0, 1.0, 0.0);//基本形の上方向

		//六角形を描く
		for (int segmentFace = 0; segmentFace < (partitions + 1); segmentFace++) {//各六角形に関して
			glm::vec3 p1, p2;
            p1 = glm::vec3(
						pickedPointsArray[segmentFace * 3], 
						pickedPointsArray[segmentFace * 3 + 1], 
						pickedPointsArray[segmentFace * 3 + 2]);
			if (segmentFace == partitions) {
				//最後の一枚になった時
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace - 1) * 3], 
						pickedPointsArray[(segmentFace - 1) * 3 + 1], 
						pickedPointsArray[(segmentFace - 1) * 3 + 2]);				
			} else {		
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace + 1) * 3], 
						pickedPointsArray[(segmentFace + 1) * 3 + 1], 
						pickedPointsArray[(segmentFace + 1) * 3 + 2]);//六角柱を作る部分の終点
			}

			//座標変換のためのベクトル設定////////////////////////////////////////////////////////////////////////////////
			glm::vec3 hexApper;
			if (segmentFace == 0) hexApper = glm::vec3(0.0, 1.0, 0.0);//六角形のapper
			else hexApper = prevHexApper;
			
			

            //六角形の移動
			glm::mat4 hexTranslate = glm::translate<float>(glm::mat4(1.0f), glm::vec3(p1));

			glm::vec3 segNormal;
			if (segmentFace == partitions) {
				segNormal = glm::normalize(p1 - p2);//合わせたい法線 z'
                //segNormal = -segNormal;
			} else {
				segNormal = glm::normalize(p2 - p1);//合わせたい法線 z'
			}
			
			glm::vec3 crossVec = glm::normalize(glm::cross(hexApper, segNormal));//x'

			glm::vec3 comvertedHexApper = glm::normalize(glm::cross(segNormal, crossVec));//y'

			//基底変換行列　任意座標系あわせて回転させるような行列
			glm::mat4 hexCoordComvert4(
				glm::vec4(crossVec[0], comvertedHexApper[0], segNormal[0], 0),
				glm::vec4(crossVec[1], comvertedHexApper[1], segNormal[1], 0),
				glm::vec4(crossVec[2], comvertedHexApper[2], segNormal[2], 0),
				glm::vec4(0, 0, 0, 1));

			hexCoordComvert4 = glm::transpose(hexCoordComvert4);
			//座標変換のためのベクトル設定----------------------------------------------------------------------------------



            float hexVertices[6*3];//六角形一枚の座標を入れる

			//六角形一枚の計算	
			for (int i = 0, index = 0; i < 6; i++, index += 3) {
				glm::vec4 changePoint = glm::vec4(originalHexVertices[i*3],originalHexVertices[i*3+1],originalHexVertices[i*3+2], 1.0);
				changePoint = hexTranslate * hexCoordComvert4 * changePoint;

				memcpy(hexVertices + index, &changePoint[0], sizeof(float) * 3);//六角形一枚の頂点
			}

			//六角形を髪一本の座標を入れる配列にまとめる
			for (int i = 0, index = 0; i < 7; i++, index += 3) {//各面を構成する頂点を入力する。
				if (i == 6) 
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[0], sizeof(float) * 3);
				else 
					memcpy(hexCylinderVert + segmentFace * 7 * 3 + index, &hexVertices[i * 3], sizeof(float) * 3);
			}

           

			prevHexApper = comvertedHexApper;
		}//各六角形に関して---------------------------------------------------------------------------------------------

		

		//hexcylindervertの頂点から面を描く
		float * hexCylinder = new float[meshedDataNum_onehair];
		for (int row = 0, index = 0; row < partitions; row++) {//各セグメントについて行う
			int nwRArNum = row * 7;//nowRowArrayNum  
            int nxRArNum = (row + 1) * 7;//nextRowArrayNum

			for (int column = 0; column < 6; column++) {
				memcpy(&hexCylinder[index + 0], &hexCylinderVert[(nwRArNum + column)     * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 3], &hexCylinderVert[(nwRArNum + column + 1) * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index + 6], &hexCylinderVert[(nxRArNum + column)     * 3], sizeof(float)*3);

				memcpy(&hexCylinder[index + 9], &hexCylinderVert[(nxRArNum + column)     * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index +12], &hexCylinderVert[(nwRArNum + column + 1) * 3], sizeof(float)*3);
				memcpy(&hexCylinder[index +15], &hexCylinderVert[(nxRArNum + column + 1) * 3], sizeof(float)*3);
				
                index += 18;
			}	
		}

		memcpy(&meshedHair[meshedDataNum_onehair * nowHair], hexCylinder, sizeof(float) * meshedDataNum_onehair);
		//-------------------------------------------------------------------------------------------------------------------





        //法線の作成
        //単純に自分と隣で作った法線
        float simpleNormalVec[partitions+1][(surfaces / 2)+1][3];
        for (int row = 0; row < partitions+1; row++) {
          for (int column = 0; column < surfaces / 2+1; column++) {
            if (column == surfaces / 2) {
              memcpy(simpleNormalVec[row][column], simpleNormalVec[row][0], sizeof(float) * 3);
            } else {
              int nwRArNum = row * (surfaces / 2 + 1);
              int nxRArNum = (row + 1) * (surfaces / 2 + 1);//nextRowArrayNum
              int prRArNum = (row - 1) * (surfaces / 2 + 1);
			  glm::vec3 p1, p2, p3;

			  memcpy(&p1[0], &hexCylinderVert[(nwRArNum + column)     * 3], sizeof(float)*3);
			  memcpy(&p2[0], &hexCylinderVert[(nwRArNum + column + 1) * 3], sizeof(float)*3);
			  if (row == partitions)
                memcpy(&p3[0], &hexCylinderVert[(prRArNum + column)     * 3], sizeof(float)*3);
              else 
                memcpy(&p3[0], &hexCylinderVert[(nxRArNum + column)     * 3], sizeof(float)*3);

			  glm::vec3 e1 = p2 - p1;
			  glm::vec3 e2 = p3 - p1;
              glm::vec3 normal;
              if (row == partitions) {
                normal = glm::normalize(glm::cross(e2, e1));
              } else {
  			    normal = glm::normalize(glm::cross(e1, e2));
              }
  			  memcpy(simpleNormalVec[row][column], &normal[0], sizeof(float) * 3);
            }
          }
        }

        //自分の右を使って法線を作ったので, 左と合成
        float left_right_margeNormalVec[partitions+1][(surfaces / 2)+1][3];
        for (int row = 0; row < partitions+1; row++) {
          for (int column = 0; column < surfaces / 2 + 1; column++) {
            if (column == surfaces / 2) {
              memcpy(left_right_margeNormalVec[row][column], left_right_margeNormalVec[row][0], sizeof(float) * 3);   
            } else {
              glm::vec3 p1, p2;
              glm::vec3 normal;

              memcpy(&p1[0], simpleNormalVec[row][column], sizeof(float) * 3);

              if (column == 0) {
                memcpy(&p2[0], simpleNormalVec[row][surfaces / 2 - 1], sizeof(float) * 3);
              } else {
                memcpy(&p2[0], simpleNormalVec[row][column-1], sizeof(float) * 3);
              }

              normal = glm::normalize(p1 + p2);

              memcpy(left_right_margeNormalVec[row][column], &normal[0], sizeof(float) * 3);
            }
          }
        }

        //上下を合成
        float normalVec[partitions+1][(surfaces / 2) + 1][3];
        for (int row = 0; row < partitions + 1; row++) {
          for (int column = 0; column < surfaces / 2 + 1; column++) {
            if (column == surfaces / 2) {
              memcpy(normalVec[row][column], normalVec[row][0], sizeof(float) * 3);
            } else {
              glm::vec3 p1, p2;
              glm::vec3 normal;


              if (row == 0) {
                memcpy(normalVec[row][column], left_right_margeNormalVec[row][column], sizeof(float) * 3); 
              } else {
                memcpy(&p1[0], left_right_margeNormalVec[row][column], sizeof(float) * 3);
                memcpy(&p2[0], left_right_margeNormalVec[row-1][column], sizeof(float) * 3);

                normal = glm::normalize(p1+p2);
                memcpy(normalVec[row][column], &normal[0], sizeof(float)*3);
              }
            }
          }
        }

      
        
        int nowHairIndex = meshedDataNum_onehair * nowHair;
        for (int row = 0; row < partitions; row++) {
          const int nowRowIndex = row * surfaces * 3 * 3; 
          for (int column = 0, index = 0; column < surfaces / 2; column++, index += 18) {
            
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 0], &normalVec[row][column], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 3], &normalVec[row][column+1], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 6], &normalVec[row+1][column], sizeof(float) * 3);

            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index + 9], &normalVec[row+1][column], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index +12], &normalVec[row][column+1], sizeof(float) * 3);
            memcpy(&meshedHairNormal[nowHairIndex + nowRowIndex + index +15], &normalVec[row+1][column+1], sizeof(float) * 3);
          }
        }
        
		//-------------------------------------------------------------------------------------------------------------------
    
        //tangent vector
        float tangentVec[partitions+1][3];
		for (int row = 0; row < partitions+1; row++) {
			glm::vec3 tangent;//local
            int nwRArNum = row * 3;
            int nxRArNum = (row + 1) * 3;
			
            if (row == partitions) {
                memcpy(tangentVec[row], tangentVec[partitions - 1], sizeof(float) * 3);
            } else {
		        glm::vec3 p1, p2;
		        memcpy(&p1[0], &pickedPointsArray[nwRArNum], sizeof(float) * 3);
		        memcpy(&p2[0], &pickedPointsArray[nxRArNum], sizeof(float) * 3);

		        tangent = glm::normalize(p2 - p1);


                memcpy(tangentVec[row], &tangent[0], sizeof(float) * 3);
            }
		}

        for (int row = 0; row < partitions; row++) {
            int nowRowIndex = row * surfaces * 3 * 3;
            for (int column = 0, index = 0; column < surfaces / 2; column++, index += 18) {
                //上の三角形
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 0], &tangentVec[row], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 3], &tangentVec[row], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 6], &tangentVec[row+1], sizeof(float) * 3);

                //下の三角形
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index + 9], &tangentVec[row+1], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index +12], &tangentVec[row], sizeof(float) * 3);
                memcpy(&meshedHairTangent[nowHairIndex + nowRowIndex + index +15], &tangentVec[row+1], sizeof(float) * 3);
            }
        }
		//-------------------------------------------------------------------------------------------------------------------
        delete[] pickedPointsArray;
		delete[] hexCylinderVert;
		delete[] hexCylinder;

		if (segmentsArray) {
			nowHairStartArrayNum += segmentsArray[nowHair] + 1;
		} else {
			nowHairStartArrayNum += defaultSegs + 1;
		}
	}//髪一本一本について-------------------------------------------------------------------------------------------
      


	//バウンディングボックスを作るための座標を保存。
	float min[3], max[3];
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	//バウンディングボックスの計算用座標を取得しておく
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[4];
	glGenBuffers(4, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];



	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//法線バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);
	
	//textureバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), NULL, GL_STATIC_DRAW);

    /*
    float* testColor = new float[meshedVertexNum];
    for (int i = 0; i < meshedVertexNum; i++) {
      if (i % 36 >= 0 && i % 36 <= 5) {
        testColor[i] = 0.0;
      } else if (i % 36 >= 6 && i % 36 <= 11) {
        testColor[i] = 1.0;
      } else if (i % 36 >= 12 && i % 36 <= 17) {
        testColor[i] = 2.0;
      } else if (i % 36 >= 18 && i % 36 <= 23) {
        testColor[i] = 3.0;
      } else if (i % 36 >= 24 && i % 36 <= 29) {
        testColor[i] = 4.0;
      } else if (i % 36 >= 30 && i % 36 <= 35) {
        testColor[i] = 5.0;
      }
    }

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, meshedVertexNum * sizeof(float), testColor,GL_STATIC_DRAW);
    */

	//tangentバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairTangent, GL_STATIC_DRAW);
	
	delete[] meshedHair;
	delete[] meshedHairNormal;
	delete[] meshedHairTangent;
    //delete[] testColor;

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    
	//インデックス2はテクスチャ座標バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    /*
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	*/
    
    //インデックス3はtangentバッファ
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	return vaoHandle;
    
}

GLuint ikdObjects::ikdRibbon_cyHairFile(ikdDataSet* pdataSet)
{


  //可変予定パラメータ
    const int partitions = 10;//一本の髪をいくつに区切るか
	const int surfaces = 2;//各セグメントを何面で作るか　三角二枚で作ってみる
	

	//dataSetに含める可能性あり
	const int meshedDataNum				= (partitions + 1) * 2 * 3 * (pdataSet->getHairFileHeader().hair_count);//メッシュ化した後の全データ数(各頂点のxyzまで)
	const int meshedDataNum_onehair		= (partitions + 1) * 2 * 3;
	const int meshedVertexNum			= (partitions + 1) * 2 * (pdataSet->getHairFileHeader().hair_count);//頂点は共有する
	const int meshedVertexNum_onehair	= (partitions + 1) * 2;
	const int meshedIndexNum			= partitions * 2 * 3 * (pdataSet->getHairFileHeader().hair_count);
	const int meshedIndexNum_onehair	= partitions * 2 * 3;


	const unsigned short* segmentsArray = pdataSet->getHairFileSegmentsArray();
	const float* pointsArray = pdataSet->getHairFilePointsArray();
	float * meshedHair = new float[meshedDataNum];//すべての髪にpartitionsパート, surfaces/2面(一面に二枚の三角形), 3頂点, xyzのデータ
	unsigned int * meshedHairIndex = new unsigned int[meshedIndexNum];
	float * meshedHairNormal = new float[meshedDataNum];//髪を描く際, 全頂点に対して法線を設定する
	float * meshedHairTangent = new float[meshedDataNum];//髪の流れの方向のベクトル
	int nowHairStartArrayNum = 0, nowHairEndArrayNum;//これから描く髪の始点と終点
	int defaultSegs = 0;
	if (segmentsArray) {
		nowHairEndArrayNum = segmentsArray[0];//これから描く髪の始点と終点
	} else {//segmentsArrayがnullのとき
		defaultSegs = pdataSet->getHairFileHeader().d_segments;
		nowHairEndArrayNum = defaultSegs;
	} 

	//元になる頂点二つ
	glm::vec4 originRibbonVert = glm::vec4(0.0, 0.0, 0.0, 1.0);

	//indexの作成
	//インデックスの元を作成 これに手を加えて座標の指定を行う。
	unsigned int singleHairIndex[meshedIndexNum_onehair];
	for (int i = 0; i < partitions; i++) {
		if (i == 0) {
			singleHairIndex[0] = 0;
			singleHairIndex[1] = 1;
			singleHairIndex[2] = 2;
			singleHairIndex[3] = 2;
			singleHairIndex[4] = 1;
			singleHairIndex[5] = 3;
		} else {
			int nowStart = i * 6;
			int prevStart = (i - 1) * 6;
			for (int j = 0; j < 6; j++) {
				singleHairIndex[nowStart + j]	= singleHairIndex[prevStart + j] + 2;
			}
		}
	}





	for (unsigned int nowHair = 0; nowHair < pdataSet->getHairFileHeader().hair_count; nowHair++) {//髪一本一本について
		  if (segmentsArray) {
			  nowHairEndArrayNum = nowHairStartArrayNum + segmentsArray[nowHair];//現在注目している毛の最後の頂点
		  } else {
			  nowHairEndArrayNum = nowHairStartArrayNum + defaultSegs;
		  }
        //髪一本の頂点を保存
		float * ribbonVert = new float[meshedDataNum_onehair];

		int pickupInterval = 0;
        
		//節の位置
		if (segmentsArray) {
	        pickupInterval = segmentsArray[nowHair] / partitions;//間引いて考えるため、一定間隔で点を抽出する。 
		} else {
			pickupInterval = defaultSegs / partitions;
		}
		float * pickedPointsArray = new float[(partitions+1) * 3];
		for (int i = 0, interval = 0; i < (partitions + 1); i++, interval += 3) {
			if (i == partitions) memcpy(pickedPointsArray + interval, &pointsArray[nowHairEndArrayNum * 3], sizeof(float) * 3);
			else memcpy(pickedPointsArray + interval, &pointsArray[nowHairStartArrayNum * 3 + i*3*pickupInterval], sizeof(float)*3);
		}		



        //ベクトル生成用
        glm::vec3 prevRibbonApper = glm::vec3(0.0, 1.0, 0.0);//基本形の上方向

		//2頂点の位置を決める
		for (int segmentFace = 0; segmentFace < (partitions + 1); segmentFace++) {//各2頂点に関して
			glm::vec3 p1, p2;
            p1 = glm::vec3(
						pickedPointsArray[segmentFace * 3], 
						pickedPointsArray[segmentFace * 3 + 1], 
						pickedPointsArray[segmentFace * 3 + 2]);
			if (segmentFace == partitions) {
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace - 1) * 3], 
						pickedPointsArray[(segmentFace - 1) * 3 + 1], 
						pickedPointsArray[(segmentFace - 1) * 3 + 2]);				
			} else {		
				p2 = glm::vec3(
						pickedPointsArray[(segmentFace + 1) * 3], 
						pickedPointsArray[(segmentFace + 1) * 3 + 1], 
						pickedPointsArray[(segmentFace + 1) * 3 + 2]);
			}

			//座標変換の基底変換ベクトル////////////////////////////////////////////////////////////////////////////////
			glm::vec3 ribbonApper;
			if (segmentFace == 0) ribbonApper = glm::vec3(0.0, 1.0, 0.0);
			else ribbonApper = prevRibbonApper;
			
            //2頂点の移動
			glm::mat4 ribbonTranslate = glm::translate<float>(glm::mat4(1.0f), glm::vec3(p1));

			glm::vec3 segNormal;
			if (segmentFace == partitions) {
				segNormal = glm::normalize(p1 - p2);//変換後のz'
			} else {
				segNormal = glm::normalize(p2 - p1);//変換後のz'
			}
			
			glm::vec3 crossVec = glm::normalize(glm::cross(ribbonApper, segNormal));//変換後のx'

			glm::vec3 comvertedRibbonApper = glm::normalize(glm::cross(segNormal, crossVec));//変換後のy'

			//基底変換行列　任意座標系あわせて回転させるような行列
			glm::mat4 ribbonCoordComvert(
				glm::vec4(crossVec[0], comvertedRibbonApper[0], segNormal[0], 0),
				glm::vec4(crossVec[1], comvertedRibbonApper[1], segNormal[1], 0),
				glm::vec4(crossVec[2], comvertedRibbonApper[2], segNormal[2], 0),
				glm::vec4(0, 0, 0, 1));

			ribbonCoordComvert = glm::transpose(ribbonCoordComvert);
			//座標変換のためのベクトル設定----------------------------------------------------------------------------------

            float nowRibbonVertices[2*3];//変換後の2頂点の座標

			//2頂点の計算
			for (int i = 0, index = 0; i < 2; i++, index += 3) {
				glm::vec4 changePoint = glm::vec4(originRibbonVert[0], originRibbonVert[1], originRibbonVert[2], 1.0);
				changePoint = ribbonTranslate * ribbonCoordComvert * changePoint;
					
				memcpy(nowRibbonVertices + index, &changePoint[0], sizeof(float) * 3);
			}

			//2頂点をを髪一本の座標を入れる配列にまとめる
			for (int i = 0, index = 0; i < 2; i++, index += 3) {//各面を構成する頂点を入力する。
				memcpy(ribbonVert + segmentFace * 2 * 3 + index, &nowRibbonVertices[i * 3], sizeof(float) * 3);
			}

			prevRibbonApper = comvertedRibbonApper;
		}//各2頂点に関して--------------------------------------------------------------------------------------------

		memcpy(&meshedHair[meshedDataNum_onehair * nowHair], ribbonVert, sizeof(float) * meshedDataNum_onehair);

		//indexの設定
		int nowHairStart = meshedIndexNum_onehair * nowHair;
		int nowHairIndexOffset = meshedVertexNum_onehair * nowHair;
		for (int i = 0; i < meshedIndexNum_onehair; i++) {
			meshedHairIndex[nowHairStart + i] = singleHairIndex[i] + nowHairIndexOffset;
		}

        //tangent vector
		//髪一本分のタンジェントを入れる
		
		float tangentVec[partitions+1][3];
		for (int row = 0; row < partitions+1; row++) {
			glm::vec3 tangent;
            int nwRArNum = row * 3;
            int nxRArNum = (row + 1) * 3;
			
            if (row == partitions) {
                memcpy(tangentVec[row], tangentVec[partitions - 1], sizeof(float) * 3);
            } else {
		        glm::vec3 p1, p2;
		        memcpy(&p1[0], &pickedPointsArray[nwRArNum], sizeof(float) * 3);
		        memcpy(&p2[0], &pickedPointsArray[nxRArNum], sizeof(float) * 3);

		        tangent = glm::normalize(p2 - p1);

                memcpy(tangentVec[row], &tangent[0], sizeof(float) * 3);
            }
		}

		

		//meshedHairTangentに対して、データを入れていく
		nowHairStart = meshedDataNum_onehair * nowHair;
		for (int row = 0; row < partitions + 1; row++) {
			int nowPartStart = row * 2;
			memcpy(&meshedHairTangent[nowHairStart + nowPartStart * 3]		, tangentVec[row], sizeof(float) * 3);
			memcpy(&meshedHairTangent[nowHairStart + (nowPartStart + 1) * 3]	, tangentVec[row], sizeof(float) * 3);
		}
		//-------------------------------------------------------------------------------------------------------------------

		
        delete[] pickedPointsArray;
		delete[] ribbonVert;

		if (segmentsArray) {
			nowHairStartArrayNum += segmentsArray[nowHair] + 1;
		} else {
			nowHairStartArrayNum += defaultSegs + 1;
		}
	}//髪一本一本について-------------------------------------------------------------------------------------------
      

	pdataSet->setIndex(meshedIndexNum, meshedHairIndex);//index配列のコピー完了

	float * meshedHairExpandDirection = new float[meshedVertexNum];
	for (int i = 0; i < meshedVertexNum; i++) {
		if (i % 2 == 0) {
			meshedHairExpandDirection[i] = -1.0;
		} else if (i % 2 == 1) {
			meshedHairExpandDirection[i] = 1.0;
		}
	}

	//バウンディングボックスを作るための座標を保存。
	float min[3], max[3];
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	//バウンディングボックスの計算用座標を取得しておく
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[5];
	glGenBuffers(5, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];
	GLuint expandDirectionHandle = vboHandle[4];


	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//法線バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);
	
	//textureバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), NULL, GL_STATIC_DRAW);

	//tangentバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairTangent, GL_STATIC_DRAW);

	//expandDirectionバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedVertexNum * sizeof(float), meshedHairExpandDirection, GL_STATIC_DRAW);
	
	delete[] meshedHair;
	delete[] meshedHairNormal;
	delete[] meshedHairTangent;
	delete[] meshedHairExpandDirection;
    //delete[] testColor;

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    
	//インデックス2はテクスチャ座標バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    /*
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	*/
    
    //インデックス3はtangentバッファ
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス4はexpand directionバッファ
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	return vaoHandle;
    
}

GLuint ikdObjects::ikdLoad_fromMyHairFile(char* fileName, ikdDataSet* pdataSet)
{
	char buf[100];
	FILE* fp = fopen(fileName, "r"); 
	if (fp == NULL) {
		printf("file open error.\n");
		exit(1);
	}
	int meshedDataNum;
	int vindex = 0, nindex = 0;
	
	fscanf(fp, "%d", &meshedDataNum);
	float *meshedHair = new float[meshedDataNum];
	float *meshedHairNormal = new float[meshedDataNum];
	while (fgets(buf, 50, fp) != NULL) {
		switch(buf[0]) {
		case 'v':
			sscanf(buf, "v %f %f %f", &meshedHair[vindex], &meshedHair[vindex+1], &meshedHair[vindex+2]);
			vindex += 3;
			break;
		case 'n':
			sscanf(buf, "n %f %f %f", &meshedHairNormal[nindex], &meshedHairNormal[nindex+1], &meshedHairNormal[nindex+2]);
			nindex += 3;
			break;
		default:
			
			break;
		}
	}
	
	fclose(fp);


	float min[3], max[3];//バウンディングボックスを作るための座標を保存。
	for (int i = 0; i < 3; i++) {
		memset(&min[i], FLT_MAX, sizeof(float));
		memset(&max[i], FLT_MIN, sizeof(float));
	}

	
	int meshedVertexNum = meshedDataNum / 3;
	//バウンディングボックスの計算用座標を取得しておく
	for (int i = 0; i < meshedVertexNum; i++) {
		for (int j = 0; j < 3; j++) {
			if (meshedHair[i*3+j] > max[j]) max[j] = meshedHair[i*3+j];
			if (meshedHair[i*3+j] < min[j]) min[j] = meshedHair[i*3+j];
		}
	}
	pdataSet->setBoundingPos(min, max);


	//create vertex buffer object
	GLuint vboHandle[2];
	glGenBuffers(2, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];



	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHair, GL_STATIC_DRAW);

	//法線バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, meshedDataNum * sizeof(float), meshedHairNormal, GL_STATIC_DRAW);

	delete[] meshedHair;
	delete[] meshedHairNormal;
	
	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdOSMLastPosition(ikdDataSet* pDataSet)
{
	//opacity mapの最終地点をユーザが動的に設定できるよう、平面を表示する

	float planeVert[3*3*2];
	float planeTangent[2*3*3] = {
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0
	};
	float planeMoveVec[2*3] = {
		0, 1, 3,
		3, 1, 2
	};


	float startPos[] = {0.0, 0.0, 0.0};//初期点はとりあえずすべて0とする
	for (int i = 0, index = 0; i < 2*3; i++, index += 3) {
		memcpy(planeVert+index, startPos, sizeof(float)*3);
	}	

	//create vertex buffer object
	GLuint vboHandle[5];
	glGenBuffers(5, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];
	GLuint expandDirectionHandle = vboHandle[4];


	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), planeVert, GL_STATIC_DRAW);

	//法線バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
	
	//textureバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);

	//tangentバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 3 * sizeof(float), planeTangent, GL_STATIC_DRAW);

	//expandDirectionバッファにデータを送る
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(float), planeMoveVec, GL_STATIC_DRAW);
	
	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス1は法線バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    
	//インデックス2はテクスチャ座標バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    
    //インデックス3はtangentバッファ
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	//インデックス4はexpand directionバッファ
	glBindBuffer(GL_ARRAY_BUFFER, expandDirectionHandle);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	return vaoHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_ArrayDepth(ikdDataSet* pDataSet,const int textureNum)
{
	GLfloat border[] = {1.0f, 0.0f, 0.0f, 1.0f};

	GLuint depthTex = 0;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthTex);//2次元テクスチャを配列として定義
  
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), textureNum, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);//テクスチャの本体
	
	pDataSet->setTexHandle(depthTex);

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	GLenum drawBuffers[] = {GL_DEPTH_ATTACHMENT};
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_ArrayColor(ikdDataSet* pDataSet,const int textureNum)
{
	GLfloat border[] = {1.0f, 1.0f, 1.0f, 1.0f};

	GLuint colorTex = 0;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, colorTex);//2次元テクスチャを配列として定義
  
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 
    textureNum, 0, GL_RGBA, GL_FLOAT, NULL);//テクスチャの本体

  pDataSet->setTexHandle(colorTex);

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight());

  
	//深度バッファをFBOにバインドする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);
     

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleDepth(ikdDataSet* pDataSet)
{

	GLfloat border[]={1.0f, 0.0f, 0.0f, 0.0f};

	//テクスチャを作成
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);//テクスチャの本体
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
 
	//使うテクスチャの番号
	//glActiveTexture(GL_TEXTURE0+activeTextureNum);
	//glActiveTexture(GL_TEXTURE10);
	//glBindTexture(GL_TEXTURE_2D, depthTex);

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	//fboにテクスチャをバインド
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	//GLenum drawBuffers[] = {GL_NONE};
	//glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す
	glBindTexture(GL_TEXTURE_2D, 0);


	pDataSet->setTexHandle(depthTex);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleDepth_NoCompare(ikdDataSet* pDataSet)
{

	GLfloat border[]={1.0f, 0.0f, 0.0f, 0.0f};

	//テクスチャを作成
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);//テクスチャの本体
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	//fboにテクスチャをバインド
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す
	glBindTexture(GL_TEXTURE_2D, 0);


	pDataSet->setTexHandle(depthTex);

	return fboHandle;
}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleColor(ikdDataSet *pDataSet)
{

	GLfloat border[]={1.0f, 1.0f, 1.0f, 1.0f};

	//テクスチャを作成
	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_RGBA, GL_FLOAT, NULL);//テクスチャの本体

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	//fboにテクスチャをバインド
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colorTex, 0);


	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight());

	//深度バッファをFBOにバインドする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);

	pDataSet->setTexHandle(colorTex);

	/*
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す

	glBindTexture(GL_TEXTURE_2D, 0);

	

	return fboHandle;
}

void ikdObjects::ikdChangeFramebufferTexture_SingleColor(int newWidth, int newHeight, ikdDataSet* pDataSet, 
		int deleteTexNum, int setFboNum)
{
	
	//古いテクスチャを削除
	pDataSet->deleteTexture(deleteTexNum);


	//新しいテクスチャを作成
	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newWidth, newHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);//テクスチャの本体

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//既存のフレームバッファオブジェクト呼び出し
	GLuint fboHandle = pDataSet->getFboHandle(setFboNum);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	//fboにテクスチャをバインド
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colorTex, 0);


	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			newWidth, newHeight);

	//深度バッファをFBOにバインドする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);

	pDataSet->setTexHandle(colorTex);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す

	glBindTexture(GL_TEXTURE_2D, 0);

}

GLuint ikdObjects::ikdMakeFramebufferTexture_SingleColor_calculatedShadow(ikdDataSet *pDataSet)
{
	//最終的に計算した後の影を保存するためのテクスチャ
	//ひとまず普通のテクスチャと同じパラメータで設定しているが, 
	//ある程度進んだら, スクリーン上に貼り付けるようにする


	GLfloat border[]={1.0f, 1.0f, 1.0f, 1.0f};

	//テクスチャを作成
	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight(), 0,
		GL_RGBA, GL_FLOAT, NULL);//テクスチャの本体

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);//fbo有効化

	//fboにテクスチャをバインド
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colorTex, 0);


	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			pDataSet->getDepthMapWidth(), pDataSet->getDepthMapHeight());

	//深度バッファをFBOにバインドする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);

	pDataSet->setTexHandle(colorTex);

	/*
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す

	glBindTexture(GL_TEXTURE_2D, 0);

	

	return fboHandle;
}

void ikdObjects::ikdMakeColorTexture(ikdDataSet* pDataSet, glm::vec4 color)
{
	//無地テクスチャの作成////////////////////////////////////////////////////////////////////////

	float colorArray[4];
	memcpy(colorArray, &color[0], sizeof(float)*4);

	GLuint texHandle;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
		GL_RGBA, GL_FLOAT, colorArray);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//振る舞いの設定
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	pDataSet->setTexHandle(texHandle);

	glBindTexture(GL_TEXTURE_2D, 0);
}

//vaoHandleと一緒に, indexを保存しておくような配列を用意したほうがいい。
GLuint ikdObjects::ikddrawelemetnstest(ikdDataSet* pdataSet)
{
	float boxVert[3*3*12];
	float boxNormal[3*3*12];
	float boxTexCoord[2*3*12];
	//頂点の取得
	ikdGetVertexBox(boxVert, boxNormal, boxTexCoord);


	float vertices[8][3] = {-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f};

	//サイズの設定
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			vertices[i][j] *= 100;
		}
	}

	//create vertex buffer object
	GLuint vboHandle[1];
	glGenBuffers(1, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];


	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

	
	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;
}

GLuint ikdObjects::ikdBillBoald(ikdDataSet* pdataSet)	
{

	glm::vec3 p1 = glm::vec3(-0.5, 0.5, 0.0);
	glm::vec3 p2 = glm::vec3(0.5, -0.5, 0.0);

	float vertices[] = {
		  p1[0], p1[1], p1[2], 
		  p1[0], p1[1], p1[2], 
		  p2[0], p2[1], p2[2], 
		  p2[0], p2[1], p2[2], 
	};
	float normals[] = {
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};

	
	glm::vec3 tang = p1 - p2;
	tang = glm::normalize(tang);


	float tangents[] = {
		tang[0], tang[1], tang[2],
		tang[0], tang[1], tang[2],
		tang[0], tang[1], tang[2],
		tang[0], tang[1], tang[2]
	};

	float expand[] = {
		-1.0,
		 1.0,
		 1.0,
		-1.0
	};


	for (int i = 0; i < 12; i++) {
		vertices[i] *= 100;
	}

	//create vertex buffer object
	GLuint vboHandle[5];
	glGenBuffers(5, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
	GLuint normalBufferHandle = vboHandle[1];
	GLuint texBufferHandle = vboHandle[2];
	GLuint tangentBufferHandle = vboHandle[3];
	GLuint expandBufferHandle = vboHandle[4];

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), tangents, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, expandBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), expand, GL_STATIC_DRAW);

	//create vertex array object
	GLuint vaoHandle;

	//頂点配列オブジェクトの作成と設定
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//頂点属性配列を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	

	//インデックス0は位置バッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, expandBufferHandle);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	return vaoHandle;

}

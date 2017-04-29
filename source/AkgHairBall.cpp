#include "AkgHairBall.h"

#include "AkgLoadObj.h"

#include <vector>
#include <algorithm>

void AkgHairBall::Create(t_ObjectInfo* objectInfo)
{
	float uv[8][2] = {
    1.0, 1.0,
    1.0, 0.0,
    0.0, 1.0,
    0.0, 0.0
  };

  std::vector<float> vert;

  AkgLoadObj loadObj;
  loadObj.Load("objects/hairball.obj", vert, m_Index);

  GetBoundingBox(&vert);

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
  GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);

  //法線バッファにデータを挿入
  glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);

	//テクスチャ位置バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4*2 * sizeof(float), uv, GL_STATIC_DRAW);

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

  //インデックス1は法線バッファ
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
  
	//インデックス2はテクスチャ頂点バッファ
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

  objectInfo->handle = vaoHandle;
  objectInfo->index = &m_Index[0];
  objectInfo->indexSize = m_Index.size();
  objectInfo->opacityValue = 0.0f;
  memcpy(objectInfo->bbox, m_BBox, sizeof(float) * 3*8);
}

void AkgHairBall::GetClipping(float nearFar[], glm::mat4 modelMtx, glm::mat4 viewMtx)
{
    for (unsigned int j = 0; j < 8; j++) {
      float base[3];
      memcpy(base, m_BBox[j], sizeof(float)* 3);
      glm::vec4 point = glm::vec4(base[0], base[1], base[2], 1.0);
      point = viewMtx * modelMtx * point;
      float z = -point.z;

      nearFar[0] = std::min<float>(z, nearFar[0]);
      nearFar[1] = std::max<float>(z, nearFar[1]);
    }
}


void AkgHairBall::GetBoundingBox(std::vector<float>* vert)
{

  int vertexNum = vert->size() / 3;
  float min[3], max[3];
  min[0] = min[1] = min[2] = FLT_MAX;
  max[0] = max[1] = max[2] = -FLT_MAX;
  for (int i = 0; i < vertexNum; i++) {
    float x = vert->at(i);
    float y = vert->at(i+1);
    float z = vert->at(i+2);

    min[0] = std::min<float>(x, min[0]);
    min[1] = std::min<float>(y, min[1]);
    min[2] = std::min<float>(z, min[2]);

    max[0] = std::max<float>(x, max[0]);
    max[1] = std::max<float>(y, max[1]);
    max[2] = std::max<float>(z, max[2]);  
  }


  float bbox[8][3] = {
    min[0], min[1], min[2],
    max[0], min[1], min[2],
    min[0], max[1], min[2],
    max[0], max[1], min[2],
    min[0], min[1], max[2],
    max[0], min[1], max[2],
    min[0], max[1], max[2],
    max[0], max[1], max[2],
  };

  memcpy(m_BBox, bbox, sizeof(float)*8*3);
}
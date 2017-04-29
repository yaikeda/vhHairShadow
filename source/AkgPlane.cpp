#include "AkgPlane.h"

void AkgPlane::Create(t_ObjectInfo* objectInfo)
{
    float vert[4][3] = {
    -0.5f,  0.0f, -0.5f,
     0.5f,  0.0f, -0.5f,
     0.5f,  0.0f,  0.5f,
    -0.5f,  0.0f,  0.5f};

  unsigned int index_base[6] = {0,3,2, 2,1,0};
  m_Index = std::vector<unsigned int>(index_base, index_base+6);

	//create vertex buffer object
	GLuint vboHandle[3];
	glGenBuffers(3, vboHandle);
	GLuint positionBufferHandle = vboHandle[0];
  GLuint normalBufferHandle = vboHandle[1];
	GLuint texCoordBufferHandle = vboHandle[2];

	//位置バッファにデータを入れる
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4*3 * sizeof(float), vert, GL_STATIC_DRAW);

  //法線バッファにデータを挿入
  glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 4*3 * sizeof(float), vert, GL_STATIC_DRAW);

  //テクスチャ位置バッファにデータを挿入
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4*2 * sizeof(float), vert, GL_STATIC_DRAW);

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
}
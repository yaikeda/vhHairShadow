#include "AkgRectangleHairBall.h"
#include "AkgLoadObj.h"

#include <algorithm>

void AkgRectangleHairBall::Create(t_ObjectInfo* objectInfo)
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

  float divValue = 1.0f / 3.0f;
  std::vector<float> LineVert;

  //line
  for (int i = 0; i < vert.size(); i += 3 * 3) {
    glm::vec3 vecs[3];
    vecs[0] = glm::vec3(vert[i], vert[i + 1], vert[i + 2]);
    vecs[1] = glm::vec3(vert[i + 3], vert[i + 4], vert[i + 5]);
    vecs[2] = glm::vec3(vert[i + 6], vert[i + 7], vert[i + 8]);

    glm::vec3 point;
    point[0] = (vecs[0].x + vecs[1].x + vecs[2].x) * divValue;
    point[1] = (vecs[0].y + vecs[1].y + vecs[2].y) * divValue;
    point[2] = (vecs[0].z + vecs[1].z + vecs[2].z) * divValue;

    LineVert.push_back(point[0]);
    LineVert.push_back(point[1]);
    LineVert.push_back(point[2]);
  }

  //Rectangle
  std::vector<float> RectangleVert;
  std::vector<float> RectangleUV;

  int mabiki = 1;

  for (int i = 0; i < LineVert.size(); i += 3*mabiki) {    
    RectangleVert.push_back(LineVert[i]-0.0001);
    RectangleVert.push_back(LineVert[i + 1]+0.0001);
    RectangleVert.push_back(LineVert[i + 2]);


    RectangleVert.push_back(LineVert[i]+0.0001);
    RectangleVert.push_back(LineVert[i + 1]+0.0001);
    RectangleVert.push_back(LineVert[i + 2]);

    RectangleVert.push_back(LineVert[i]+0.0001);
    RectangleVert.push_back(LineVert[i + 1]-0.0001);
    RectangleVert.push_back(LineVert[i + 2]);

    RectangleVert.push_back(LineVert[i]-0.0001);
    RectangleVert.push_back(LineVert[i + 1]-0.0001);
    RectangleVert.push_back(LineVert[i + 2]);


    RectangleUV.push_back(0.0);
    RectangleUV.push_back(2.0);

    RectangleUV.push_back(2.0);
    RectangleUV.push_back(2.0);

    RectangleUV.push_back(2.0);
    RectangleUV.push_back(0.0);

    RectangleUV.push_back(0.0);
    RectangleUV.push_back(0.0);
  }

  m_Index.clear();
  int index = 0;
  for (int i = 0; i < LineVert.size(); i += 3*mabiki) {
    m_Index.push_back(index+0);
    m_Index.push_back(index+1);
    m_Index.push_back(index+2);
    m_Index.push_back(index+3);
    index+= 4;
  }


  //tangent
  std::vector<float> tangent;
  for (int i = 0; i < LineVert.size() - 3; i += 3*mabiki) {
    //頂点数-1個のtangentを作成する
    glm::vec3 vecs[2];
    vecs[0] = glm::vec3(LineVert[i + 0], LineVert[i + 1], LineVert[i + 2]);
    vecs[1] = glm::vec3(LineVert[i + 3], LineVert[i + 4], LineVert[i + 5]);

    glm::vec3 result = vecs[1] - vecs[0];

    tangent.push_back(result[0]);
    tangent.push_back(result[1]);
    tangent.push_back(result[2]);
  }
  
  std::vector<float> moveDir;
  for (int i = 0; i < LineVert.size(); i += 3*mabiki) {
    moveDir.push_back(-1);
    moveDir.push_back(1);
    moveDir.push_back(0);

    moveDir.push_back(1);
    moveDir.push_back(1);
    moveDir.push_back(0);

    moveDir.push_back(1);
    moveDir.push_back(-1);
    moveDir.push_back(0);

    moveDir.push_back(-1);
    moveDir.push_back(-1);
    moveDir.push_back(0);
  }


  GetBoundingBox(&RectangleVert);


  //create vertex buffer object
  GLuint vboHandle[4];
  glGenBuffers(4, vboHandle);
  GLuint positionBufferHandle = vboHandle[0];
  GLuint tangentBufferHandle = vboHandle[1];
  GLuint moveDirBufferHandle = vboHandle[2];
  GLuint UVBufferhandle = vboHandle[3];

  //位置バッファにデータを入れる
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, RectangleVert.size() * sizeof(float), &RectangleVert[0], GL_STATIC_DRAW);

  //法線バッファにtangentを挿入
  glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(float), &tangent[0], GL_STATIC_DRAW);

  //テクスチャ位置バッファにデータを挿入
  glBindBuffer(GL_ARRAY_BUFFER, moveDirBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, moveDir.size() * sizeof(float), &moveDir[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, UVBufferhandle);
  glBufferData(GL_ARRAY_BUFFER, RectangleUV.size() * sizeof(float), &RectangleUV[0], GL_STATIC_DRAW);

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
  glBindBuffer(GL_ARRAY_BUFFER, tangentBufferHandle);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

  //インデックス2はテクスチャ頂点バッファ
  glBindBuffer(GL_ARRAY_BUFFER, moveDirBufferHandle);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
  
  //uv
  glBindBuffer(GL_ARRAY_BUFFER, UVBufferhandle);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

  objectInfo->handle = vaoHandle;
  objectInfo->index = &m_Index[0];
  objectInfo->indexSize = m_Index.size();
  objectInfo->opacityValue = 0.0f;
  memcpy(objectInfo->bbox, m_BBox, sizeof(float)* 3 * 8);
}

void AkgRectangleHairBall::GetBoundingBox(std::vector<float>* vert)
{
  int vertexsize = vert->size() / 3;
  float min[3], max[3];
  min[0] = min[1] = min[2] = FLT_MAX;
  max[0] = max[1] = max[2] = -FLT_MAX;
  for (int i = 0; i < vertexsize; i++) {
    float x = vert->at(i);
    float y = vert->at(i + 1);
    float z = vert->at(i + 2);

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

  memcpy(m_BBox, bbox, sizeof(float)* 8 * 3);
}
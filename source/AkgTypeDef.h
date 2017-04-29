#ifndef __AKGTYPEDEF_H__
#define __AKGTYPEDEF_H__

#include <map>
#include <iostream>

#include "GL/glew.h"

typedef struct structObjectInfo {
  char*         name;
  unsigned int* index;
  int           indexSize;
  GLuint        handle;
  float         opacityValue;
  float         thickness;
  float         bbox[8][3];
  float         modelMatrix[48];
} t_ObjectInfo;

typedef struct structRenderPassInfo {
  char*   name;
  GLuint  program;
} t_RenderPassInfo;

typedef struct structTextureInfo {
  char*   name;
  GLuint  texture;
  GLuint  fbo;
  GLint   size;
} t_TextureInfo;

typedef struct structLight {
  float pos[3];
  float look[3];
  float up[3];
  float viewMatrix[16];
  float projMatrix[16];
} t_LightInfo;

typedef struct structCamera {
  float pos[3];
  float look[3];
  float up[3];
  float angleAspectNearFar[4];
} t_CameraInfo;

class AkgObjects;


typedef std::map<std::string, AkgObjects*> t_ObjectMap;
typedef std::map<std::string, GLuint> t_RenderPassMap;


#endif
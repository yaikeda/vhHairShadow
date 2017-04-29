#include <string>
#include <fstream>
#include <stdio.h>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

class ikdGetShader
{
  //vertex, fragmentシェーダの二つを使用する場合のみ対応
private:
  GLuint getShader(GLenum shaderType, const char* shaderFileName);
public:
  GLuint getProgramHandle(const char* filename);
};
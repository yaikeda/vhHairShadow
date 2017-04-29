#include <stdio.h>
#include <string>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

class ikdErrorCheck
{
public:
  void shaderObjectError(GLuint shader, const char* str);
  void shaderCompileError(GLuint shader, const char* str);
  void programObjectError(GLuint program, const char* str);
  void programLinkError(GLuint program, const char* str);
  void fileLoadError(const char* fileName, const std::string str);
};
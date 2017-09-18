
#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include "glm/glm.hpp"
#include <iostream>

class ikdContactWithGPU
{
public:
  void setMatrix4f(const char* locationName, GLuint programHandle, glm::mat4 matrix);
  void setTexture(const char* locationName, GLuint programHandle, const int textureNum);
  void setVec4(const char* locationName, GLuint programHandle, glm::vec4 vector);
  void setVec3(const char* locationName, GLuint programHandle, glm::vec3 vector);
  void setFloat(const char* locationName, GLuint programHandle, float value);
  void setIntValue(const char* locationName, GLuint programHandle, int value);
  void setFloatArray(const char* locationName, GLuint programHandle, int size, float floatArray[]);
  void setMatrix4fArray(const char* locationName, GLuint programHandle, int size, float mat4Array[]);
  void setVec3Array(const char* locationName, GLuint programHandle, int size, float vec3Array[]);
};
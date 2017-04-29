#ifndef __AKGSHADERGENERATOR_H__
#define __AKGSHADERGENERATOR_H__

#include "GL/glew.h"

class AkgShaderGenerator {
 public:
  GLuint LoadShader(const char* filename);
 
 protected:
  GLuint GetShader(GLenum shaderType, const char* shaderFileName);

 private:

};


#endif
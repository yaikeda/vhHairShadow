#include "ikdPrepareShader.h"
#include "ikdErrorCheck.h"
#include "ikdDataSet.h"


GLuint ikdGetShader::getShader(GLenum shaderType, const char* shaderFileName)
{
  ikdErrorCheck errorCheck;

  GLuint shader = glCreateShader(shaderType);
  errorCheck.shaderObjectError(shader, shaderFileName);

  //ファイルの読み込み
	std::ifstream in(shaderFileName);
	std::istreambuf_iterator<char> dataBegin(in);
	std::istreambuf_iterator<char> dataEnd;
	std::string str(dataBegin, dataEnd);

  errorCheck.fileLoadError(shaderFileName, str);

	const char * file = str.c_str();

	//シェーダオブジェクトにファイルから読んだ情報を入れる
	glShaderSource(shader, 1, &file, NULL);

  return shader;
}

GLuint ikdGetShader::getProgramHandle(const char* filename)
{
  ikdErrorCheck errorCheck;
  GLuint vertShader, fragShader;//create shader object

  //load shader source code.
  std::string vert(".vert");
  std::string frag(".frag");

  vert = filename + vert;
  frag = filename + frag;

  vertShader = getShader(GL_VERTEX_SHADER, vert.c_str());
  fragShader = getShader(GL_FRAGMENT_SHADER, frag.c_str());

  //shader compile
  glCompileShader(vertShader);
  glCompileShader(fragShader);
  errorCheck.shaderCompileError(vertShader, vert.c_str());
  errorCheck.shaderCompileError(fragShader, frag.c_str());

  GLuint programHandle = glCreateProgram();
  errorCheck.programObjectError(programHandle, filename);

  glAttachShader(programHandle, vertShader);
  glAttachShader(programHandle, fragShader);

  glLinkProgram(programHandle);
  errorCheck.programLinkError(programHandle, filename);

  return programHandle;
}


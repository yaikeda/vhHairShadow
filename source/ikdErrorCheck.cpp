#include "ikdErrorCheck.h"

void ikdErrorCheck::shaderObjectError(GLuint shader, const char* str)
{
  if (0 == shader) {
    fprintf(stderr, "シェーダ生成エラー：%s", str);
    exit(1);
  }
}

void ikdErrorCheck::shaderCompileError(GLuint shader, const char* str)
{
  GLint result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (GL_FALSE == result) {
    fprintf(stderr, "シェーダコンパイルエラー\nFile name：%s", str);

    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = (char *)malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog(shader, logLen, &written, log);

      fprintf(stderr, "シェーダ　ログ：\n%s", log);
      free(log);
    }
  }
}

void ikdErrorCheck::programObjectError(GLuint program, const char* str)
{
  if (0 == program) {
    fprintf(stderr, "プログラムオブジェクト作成エラー：%s", str);
    exit(1);
  }
}

void ikdErrorCheck::programLinkError(GLuint program, const char* str)
{
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    fprintf(stderr, "シェーダプログラムのリンク失敗\nFile name：%s\n", str);

    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char* log = (char*)malloc(logLen);
      GLsizei written;
      glGetProgramInfoLog(program, logLen, &written, log);
      fprintf(stderr, "プログラム　ログ：%s", log);
      free(log);
    }
  }
}


  void ikdErrorCheck::fileLoadError(const char* fileName, const std::string str)
  {
    if (str.length() == 0) {
      fprintf(stderr, "File load error：%s", fileName);
    }
  }

#include "ikdContactWithGPU.h"

void ikdContactWithGPU::setMatrix4f(const char* locationName, GLuint programHandle, glm::mat4 matrix)
{
  int location = glGetUniformLocation(programHandle, locationName);
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
  } else {
	  fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
  }
}


void ikdContactWithGPU::setTexture(const char* locationName, GLuint programHandle, const int textureNum)
{
  int location = glGetUniformLocation(programHandle, locationName);
  if (location >= 0) {
	  glUniform1i(location, textureNum);
  } else {
	  fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
  }
}

void ikdContactWithGPU::setVec4(const char* locationName, GLuint programHandle, glm::vec4 vector)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform4fv(location, 1, &vector[0]);
	} else {
		fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
	}
}

void ikdContactWithGPU::setVec3(const char* locationName, GLuint programHandle, glm::vec3 vector)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform3fv(location, 1, &vector[0]);
	} else {
		fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
	}
}

void ikdContactWithGPU::setFloat(const char* locationName, GLuint programHandle, float value)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform1f(location, value);
	} else {
		fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
	}
}

void ikdContactWithGPU::setIntValue(const char* locationName, GLuint programHandle, int value)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform1d(location, value);
	} else {
		fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
	}
}

void ikdContactWithGPU::setFloatArray(const char* locationName, GLuint programHandle, int size, float floatArray[])
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform1fv(location, size, floatArray);
	} else {
		fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
	}
}

void ikdContactWithGPU::setMatrix4fArray(const char* locationName, GLuint programHandle, int size, float* mat4Array)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniformMatrix4fv(location, size, GL_FALSE, mat4Array);
	} else {
		fprintf(stderr, "ユニフォーム変数%sが見つからない\n", locationName);
	}
}

void ikdContactWithGPU::setVec3Array(const char* locationName, GLuint programHandle, int size, float vec3Array[])
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform3fv(location, size, vec3Array);
	} else {
		fprintf(stderr, "ユニフォーム変数%sがみつからない\n", locationName);
	}
}



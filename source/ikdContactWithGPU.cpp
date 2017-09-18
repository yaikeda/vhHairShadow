#include "ikdContactWithGPU.h"

void ikdContactWithGPU::setMatrix4f(const char* locationName, GLuint programHandle, glm::mat4 matrix)
{
  int location = glGetUniformLocation(programHandle, locationName);
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
  } else {
	  std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
  }
}


void ikdContactWithGPU::setTexture(const char* locationName, GLuint programHandle, const int textureNum)
{
  int location = glGetUniformLocation(programHandle, locationName);
  if (location >= 0) {
	  glUniform1i(location, textureNum);
  } else {
	  std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
  }
}

void ikdContactWithGPU::setVec4(const char* locationName, GLuint programHandle, glm::vec4 vector)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform4fv(location, 1, &vector[0]);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
	}
}

void ikdContactWithGPU::setVec3(const char* locationName, GLuint programHandle, glm::vec3 vector)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform3fv(location, 1, &vector[0]);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
	}
}

void ikdContactWithGPU::setFloat(const char* locationName, GLuint programHandle, float value)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform1f(location, value);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
	}
}

void ikdContactWithGPU::setIntValue(const char* locationName, GLuint programHandle, int value)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform1d(location, value);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
	}
}

void ikdContactWithGPU::setFloatArray(const char* locationName, GLuint programHandle, int size, float floatArray[])
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform1fv(location, size, floatArray);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
	}
}

void ikdContactWithGPU::setMatrix4fArray(const char* locationName, GLuint programHandle, int size, float* mat4Array)
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniformMatrix4fv(location, size, GL_FALSE, mat4Array);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "が見つからない\n" << std::endl;
	}
}

void ikdContactWithGPU::setVec3Array(const char* locationName, GLuint programHandle, int size, float vec3Array[])
{
	int location = glGetUniformLocation(programHandle, locationName);
	if (location >= 0) {
		glUniform3fv(location, size, vec3Array);
	} else {
		std::cerr << "ユニフォーム変数" << locationName << "がみつからない\n" << std::endl;
	}
}



#ifndef __READPIXEL_H__
#define __READPIXEL_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

class ReadPixel {
public:
  ReadPixel(void);
  void saveImage(const unsigned int imageWidth, const unsigned int imageHeight); 
private:
  int count;
};
#endif
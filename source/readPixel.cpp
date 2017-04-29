#include "readPixel.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//#include "opencv2/opencv.hpp"

ReadPixel::ReadPixel(void)
{
  count = 0;
}

void ReadPixel::saveImage( const unsigned int imageWidth, const unsigned int imageHeight )
{
  const unsigned int channelNum = 3; // RGBなら3, RGBAなら4
  void* dataBuffer = NULL;
  dataBuffer = ( GLubyte* )malloc( imageWidth * imageHeight * channelNum );

  // 読み取るOpneGLのバッファを指定 GL_FRONT:フロントバッファ　GL_BACK:バックバッファ
  glReadBuffer( GL_BACK );

  // OpenGLで画面に描画されている内容をバッファに格納
  glReadPixels(
    0,                 //読み取る領域の左下隅のx座標
    0,                 //読み取る領域の左下隅のy座標 //0 or getCurrentWidth() - 1
    imageWidth,             //読み取る領域の幅
    imageHeight,            //読み取る領域の高さ
    GL_BGR, //it means GL_BGR,           //取得したい色情報の形式
    GL_UNSIGNED_BYTE,  //読み取ったデータを保存する配列の型
    dataBuffer      //ビットマップのピクセルデータ（実際にはバイト配列）へのポインタ
    );

  char str[256] = {'\0'};
  sprintf_s(str, "%d", count);
  std::string sstr = str;
  GLubyte* p = static_cast<GLubyte*>( dataBuffer );
  /*std::string fname = "outputImage_" + sstr + ".jpg";
  IplImage* outImage = cvCreateImage( cvSize( imageWidth, imageHeight ), IPL_DEPTH_8U, 3 );

  for ( unsigned int j = 0; j < imageHeight; ++ j )
  {
    for ( unsigned int i = 0; i < imageWidth; ++i )
    {
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 0 ] = *p;
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 1 ] = *( p + 1 );
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 2 ] = *( p + 2 );
      p += 3;
    }
  }

  cvSaveImage( fname.c_str(), outImage );

  free( dataBuffer );
  cvReleaseImage(&outImage);
*/
  count = (count + 1) % 30;
}
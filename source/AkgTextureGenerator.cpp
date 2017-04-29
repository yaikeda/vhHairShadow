#include "AkgTextureGenerator.h"

void AkgTextureGenerator::GenerateTexture(t_TextureInfo* textureInfo, int textureNum)
{

	GLuint textureHandle = 0;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureHandle);//2次元テクスチャを配列として定義
  
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//パラメータ
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//パラメータ
  //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, textureInfo->size, textureInfo->size, 
    textureNum, 0, GL_RGBA, GL_FLOAT, NULL);//テクスチャの本体
  //textureとfboの接続は，textureArrayの場合レンダリングの直前で行う．
  

	//フレームバッファオブジェクト作成
	GLuint fboHandle;
	glGenFramebuffers(1, &fboHandle);//フレームバッファオブジェクトを生成
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureInfo->size, textureInfo->size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuf);
     

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//普通のフレームバッファに戻す
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);


  textureInfo->texture = textureHandle;
  textureInfo->fbo = fboHandle;

}

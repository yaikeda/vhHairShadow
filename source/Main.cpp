#define _CRT_SECURE_NO_WARNINGS//sscanf -> sscanf_sのwarningを出さないため。
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

//#include <crtdbg.h>

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ikdPrepareShader.h"
#include "ikdObjects.h"
#include "ikdContactWithGPU.h"

#include "external_futil.h"





const int SHOW_DEPTH_MAP = 0;//チェック用変数


const int WIDTH = 512;
const int HEIGHT = 512;
const int OSM_NUM = 12;//opacity shadow map
int DOSM_NUM = 1;//論文曰く、一枚で距離を変えると十分きれいな影ができる。


char* mapType[3] = {"OSM", "PCF", "DOSM"};
int mapTypeNum;
char* pcfOnOff[2] ={"OFF", "ON"};

//新フレームワーク用変数////////////////////////////////////////
#include "AkgGeneralManager.h"
AkgGeneralManager k_generalManager;
//////////////////////////////////////////////////////////////////

//プロトタイプ宣言/////////////////////////////////
void display(void);
void reshape(int w, int h);
void init(void);
void idle(void);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow(argv[0]);

  GLenum err =  glewInit();
  if (err != GLEW_OK) 
    fprintf(stderr, "glewの初期化エラー\n");

  glutInitDisplayMode(GL_DOUBLE | GL_RGBA | GL_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();

  glutMainLoop();

  return 0;
}

void display(void)
{
   k_generalManager.Render();
}

#include "AkgReshape.h"
void reshape(int w, int h)
{
  AkgReshape reshape;
  reshape.Exec(&k_generalManager, w, h);
  return;
}

#include "AkgIdle.h"
void idle(void)
{
  AkgIdle idle;
  idle.Exec();
  return;
}

#include "AkgMotion.h"
void motion(int x, int y)
{
  AkgMotion motion;
  motion.Exec(&k_generalManager, x, y);
  return;
}

#include "AkgKeyboard.h"
void keyboard(unsigned char key, int x, int y)
{
  AkgKeyboard keyboard;
  keyboard.exec(&k_generalManager, key, x, y);
  return;
}

#include "AkgMouse.h"
void mouse(int button, int state, int x, int y)
{
  AkgMouse mouse;
  mouse.Exec(&k_generalManager, button, state, x, y);
  return;
}

void init(void)
{
  float pos[3] = {282.0f, 150.0f, -4.93f};
  float look[3] = {275.0f, 143.0f, 0.0f};
  float up[3] = {0.0f, 1.0f, 0.0f};
  float angleAspectNearFar[4] = {110.0f, 1.0f, 1.0f, 5000.0f};
  k_generalManager.SetCamera(pos, look, up, angleAspectNearFar);
  
  k_generalManager.SetWindowSize(WIDTH, HEIGHT);
  k_generalManager.SetTextureSize(512);
  k_generalManager.SetLight();
  k_generalManager.SetObjects();
  k_generalManager.SetRenderPass();
  k_generalManager.SetTexture();
  k_generalManager.SetAnimationFlag(false);
  k_generalManager.SetRotationSpeed(3.0f);
}
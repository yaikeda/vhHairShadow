/* *****************************************************************************
 * ファイル名		： futil.c
 * 機能				： ユーティリティ関数
 * ----------------------------------------------------------------------------
 * 作成者　　		： Arakin
 * ***************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "GL/glew.h"
#include "GL/wglew.h"
//#include "glut.h"
#include "GL/freeglut.h"

#include "external_loadbmp.h"
#include "external_futil.h"


static int loadShader(GLuint shader, char *file);
static void printShaderInfoLog(GLuint shader);
static void printProgramInfoLog(GLuint program);

/* シェーダコンパイルのログバッファ */
#define MAX_SHADER_LOG_SIZE		(1024)
char s_logBuffer[MAX_SHADER_LOG_SIZE]; 

/* glutBitmapCharacter用フォント */
#ifdef F9X15
#define FUTL_FONT_TYPE		GLUT_BITMAP_9_BY_15
#define FUTL_FONT_HEIGHT	(15)
#else
#define FUTL_FONT_TYPE		GLUT_BITMAP_8_BY_13
#define FUTL_FONT_HEIGHT	(13)
#endif


/*
 * テクスチャの読み込み
 */
int FUTL_LoadTexture(
	char *name,
	unsigned int *lpTexID
)
{
	ImageData img;
	unsigned int texID;

	memset(&img, 0, sizeof(img));
	if (loadBMP(&img, name) < 0)
	{
		fprintf(stderr, "%s is not found!!\n", name);
		return -1;
	}

	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* テクスチャ画像は４バイトアライメント */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		img.width, img.height, 0, GL_RGBA, 
		GL_UNSIGNED_INT_8_8_8_8_REV,	/* GL_UNSIGNED_SHORT_1_5_5_5_REV, */
		img.data);

	*lpTexID = texID;

	releaseBMP(&img);

	return 0;
}


/*
 * 文字の描画
 */
void FUTL_DrawString(
	char *str,					/* 文字列（終端'\0'） */
	int x,						/* 文字の描画位置 */
	int y						/* 文字の描画位置 */
)
{
	int wy;
	int width, height;

	width = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	height = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

	/* 固定パイプラインに戻す */
	glUseProgram(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0f, (float)width, 0.0f, (float)height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

#if OPENGL_IMAGE_COORDINATE
	wy = y;
#else
	wy = height - y - FUTL_FONT_HEIGHT;
#endif
	glRasterPos2i(x, wy);

	for (; *str != 0; str++)
	{
		if (*str == '\n')
		{
			wy -= FUTL_FONT_HEIGHT;
			glRasterPos2i( x, wy );
			continue;
		}

		glutBitmapCharacter(FUTL_FONT_TYPE, *str);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


/*
 * 時間計測
 */
float FUTL_SubPCounter(
	LARGE_INTEGER *lpEndTime,
	LARGE_INTEGER *lpStartTime
)
{
	LARGE_INTEGER ticksPerSec;
	double qpf;
	float ftime;

	QueryPerformanceFrequency(&ticksPerSec);
	qpf = (double)ticksPerSec.QuadPart;

	ftime = (float)((lpEndTime->QuadPart - lpStartTime->QuadPart) / qpf);

	return ftime;
}



/* 光源設定 */
int FUTL_SetLight(
	int lightID,
	FLight *lpLight
)
{
	glLightfv(lightID, GL_AMBIENT, (GLfloat *)(&(lpLight->ambient)));
	glLightfv(lightID, GL_DIFFUSE, (GLfloat *)(&(lpLight->diffuse)));
	glLightfv(lightID, GL_SPECULAR, (GLfloat *)(&(lpLight->specular)));
	glLightfv(lightID, GL_POSITION, (GLfloat *)(&(lpLight->position)));

	/* 平行光源の場合、下記パラメータは無視 */
	if (lpLight->position.w == 0.0f)
		return 0;

	glLightfv(lightID, GL_SPOT_DIRECTION, (GLfloat *)(&(lpLight->spotDirection)));
	glLightfv(lightID, GL_SPOT_EXPONENT, &(lpLight->spotExponent));
	glLightfv(lightID, GL_SPOT_CUTOFF, &(lpLight->spotCutoff));
	
	glLightfv(lightID, GL_CONSTANT_ATTENUATION, &(lpLight->attenuation0));
	glLightfv(lightID, GL_LINEAR_ATTENUATION, &(lpLight->attenuation1));
	glLightfv(lightID, GL_QUADRATIC_ATTENUATION, &(lpLight->attenuation2));

	return 0;
}

/* マテリアル設定 */
int FUTL_SetMaterial(
	FMaterial *lpMaterial
)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)(&(lpMaterial->ambient)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)(&(lpMaterial->diffuse)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)(&(lpMaterial->specular)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)(&(lpMaterial->emission)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat *)(&(lpMaterial->shininess)));

	return 0;
}


/*
 * シェーダプログラムのロード
 */
int FUTL_LoadShader(
	char *vtxShdName,
	char *frgShdName,
	GLuint *lpProg
)
{
	GLuint vtxShader;
	GLuint frgShader;
	GLuint prog;
	GLint linked;

	/* シェーダオブジェクトの作成 */
	vtxShader = glCreateShader(GL_VERTEX_SHADER);
	frgShader = glCreateShader(GL_FRAGMENT_SHADER);

	/* バーテックスシェーダのロードとコンパイル */
	if (loadShader(vtxShader, vtxShdName) < 0)
	{
		return -1;
	}

	/* フラグメントシェーダのロードとコンパイル */
	if (loadShader(frgShader, frgShdName) < 0)
	{
		return -1;
	}

	/* プログラムオブジェクトの作成 */
	prog = glCreateProgram();

	/* シェーダオブジェクトのシェーダプログラムへの登録 */
	glAttachShader(prog, vtxShader);
	glAttachShader(prog, frgShader);

	/* シェーダオブジェクトの削除 */
	glDeleteShader(vtxShader);
	glDeleteShader(frgShader);

	/* シェーダプログラムのリンク */
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	printProgramInfoLog(prog);
	if (linked == GL_FALSE)
	{
		fprintf(stderr, "Link error of %s & %s!!\n", vtxShdName, frgShdName);
		return -1;
	}

	*lpProg = prog;

	return 0;
}


/*
 * シェーダープログラムをロードし、コンパイル
 */
static int loadShader(
	GLuint shader, 
	char *name
)
{
	errno_t err;
	FILE *fp;
	void *buf;
	int size;
	GLint compiled;

	if ((err = fopen_s(&fp, name, "rb")) != 0)
	{
		fprintf(stderr, "%s is not found!!\n", name);
		return -1;
	}
  
	/* ファイルサイズの取得 */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	/* シェーダプログラムの読み込み領域を確保 */
	if ((buf = (void *)malloc(size)) == NULL)
	{
		fprintf(stderr, "Memory is not enough for %s\n", name);
		fclose(fp);
		return -1;
	}
  
	/* ファイルを先頭から読み込む */
	fseek(fp, 0, SEEK_SET);
	fread(buf, 1, size, fp);

	/* シェーダオブジェクトにプログラムをセット */
	glShaderSource(shader, 1, (GLchar **)&buf, &size);
  
	/* シェーダ読み込み領域の解放 */
	free(buf);
	fclose(fp);

	/* シェーダのコンパイル */
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(shader);		/* コンパイルログの出力 */
	if (compiled == GL_FALSE)
	{
		fprintf(stderr, "Compile error in %s!!\n", name);
		return -1;
	}

	return 0;
}

/* シェーダコンパイルエラーの出力 */
static void printShaderInfoLog(
	GLuint shader
)
{
	int logSize;
	int length;

	/* ログの長さは、最後のNULL文字も含む */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		fprintf(stderr, "Shader Info Log\n%s\n", s_logBuffer);
	}
}


/* シェーダリンクエラーの出力 */
static void printProgramInfoLog(
	GLuint program
)
{
	int logSize;
	int length;

	/* ログの長さは、最後のNULL文字も含む */
	glGetProgramiv(program, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetProgramInfoLog(program, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		fprintf(stderr, "Program Info Log\n%s\n", s_logBuffer);
	}
}

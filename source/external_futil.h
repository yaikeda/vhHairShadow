/* *****************************************************************************
 * ファイル名		： futil.h
 * 機能				： ユーティリティ関数
 * ----------------------------------------------------------------------------
 * 作成者　　		： Arakin
 * ***************************************************************************** */
#ifndef __FUTIL_H
#define __FUTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/freeglut.h"
//#include "glut.h"

/* 画像座標系 */
/* 1: 左下原点(OpenGL仕様), 0:左上原点 */
#define OPENGL_IMAGE_COORDINATE		(1)

/* シフトによる積算 */
#define FUTL_MULT2(x)		((x) << 1)
#define FUTL_MULT4(x) 		((x) << 2)
#define FUTL_MULT8(x) 		((x) << 3)
#define FUTL_MULT16(x)		((x) << 4)
#define FUTL_MULT32(x)		((x) << 5)
#define FUTL_MULT64(x)		((x) << 6)
#define FUTL_MULT128(x)		((x) << 7)
#define FUTL_MULT256(x)		((x) << 8)
#define FUTL_NMULT(x, n)	((x) << (n))

/* シフトによる除算 */
#define FUTL_DIV2(x)		((x) >> 1)
#define FUTL_DIV4(x) 		((x) >> 2)
#define FUTL_DIV8(x) 		((x) >> 3)
#define FUTL_DIV16(x)		((x) >> 4)
#define FUTL_DIV32(x)		((x) >> 5)
#define FUTL_DIV64(x)		((x) >> 6)
#define FUTL_DIV128(x)		((x) >> 7)
#define FUTL_DIV256(x)		((x) >> 8)
#define FUTL_DIV512(x)		((x) >> 9)
#define FUTL_DIV1024(x)		((x) >> 10)
#define FUTL_DIV2048(x)		((x) >> 11)
#define FUTL_NDIV(x, n)		((x) >> (n))

/* ビット剰余算 */
#define FUTL_MOD2(x)		((x) & 0x1)
#define FUTL_MOD4(x) 		((x) & 0x3)
#define FUTL_MOD8(x) 		((x) & 0x7)
#define FUTL_MOD16(x)		((x) & 0xF)
#define FUTL_MOD32(x)		((x) & 0x1F)
#define FUTL_MOD64(x)		((x) & 0x3F)
#define FUTL_MOD128(x)		((x) & 0x7F)
#define FUTL_MOD256(x)		((x) & 0xFF)
#define FUTL_MOD512(x)		((x) & 0x1FF)
#define FUTL_MOD1024(x)		((x) & 0x3FF)
#define FUTL_NMOD(x, n)		((x) & ((n) - 1))

#define FUTL_ALLIGN4(x)		((x) & (~0x3))
#define FUTL_ALLIGN32(x)	((x) & (~0x1F))

#define FUTL_UPALLIGN4(x)	FUTL_ALLIGN4((x) + 0x3)
#define FUTL_UPALLIGN32(x)	FUTL_ALLIGN32((x) + 0x1F)


/* π */
#define FUTL_PI (3.141592654f)

/* DEGREEからRADIANへの変換 */
#define FUTL_DEG_TO_RAD(degree)\
    	( (FUTL_PI/180.0f) * (degree) )
#define FUTL_RAD_TO_DEG(rad)\
		( (rad) * (180.0f/FUTL_PI) )

/* ************************************************************ */
/* 頂点の定義													*/
/* ************************************************************ */
/* ２次元頂点、ベクトルの定義 */
typedef struct {
    float x;
    float y;
} FVector2D;

/* ３次元頂点、ベクトルの定義 */
typedef struct {
    float x;
    float y;
    float z;
} FVector3D;

/* ４次元頂点、ベクトルの定義 */
typedef struct {
    float x;
    float y;
    float z;
    float w;
} FVector4D;

/* ************************************************************ */
/* マトリックスの定義											*/
/* ************************************************************ */
typedef struct {
    float m00;
	float m10;
	float m20;
	float m30;
	float m01;
	float m11;
	float m21;
	float m31;
	float m02;
	float m12;
	float m22;
	float m32;
	float m03;
	float m13;
	float m23;
	float m33;
} FMatrix4x4, FMatrix;

/* クォータニオン */
typedef struct {
	float w;	/* 実数部 */
	float x;	/* 虚数部i */
	float y;	/* 虚数部j */
	float z;	/* 虚数部k */
} FQuat;

/* カラータイプ */
typedef struct {
	float r;
	float g;
	float b;
	float a;
} FColorRGBA;

/* 光源 */
typedef struct _tmpFLight {
	/* 平行光源　　　：w  = 0.0f  x,y,zで方向 */
	/* 点光源　　　　：w != 0.0f  x,y,z,wで光源位置 */
	/* スポットライト：w != 0.0f  x,y,z,wで光源位置 */
	FVector4D position;			/* 位置 */

	FColorRGBA ambient;			/* アンビエント */
	FColorRGBA diffuse;			/* ディフューズ */
	FColorRGBA specular;		/* スペキュラ */

	FVector3D spotDirection;	/* スポットライト方向 */

	/* [0,128]の範囲：0.0fはカットオフの範囲内は一様 */
	float spotExponent;			/* ライト強調 */

	/* [0,90]の範囲：180.0fは点光源 */
	float spotCutoff;			/* カットオフ */

	/* 減衰係数：平行光源では無効 */
	float attenuation0;			/* 減衰係数 */
	float attenuation1;			/* 減衰係数 */
	float attenuation2;			/* 減衰係数 */
} FLight;

/* マテリアル */
typedef struct _tmpFMaterial {
	FColorRGBA ambient;			/* アンビエント */
	FColorRGBA diffuse;			/* ディフューズ */
	FColorRGBA specular;		/* スペキュラ */
	FColorRGBA emission;		/* エミッション */
	float shininess;			/* スペキュラー強調 */
} FMaterial;


/* ************************************************ */
/* プロトタイプ宣言									*/
/* ************************************************ */


/* ************************************************ */
/* ユーティリティ関数								*/
/* ************************************************ */
/* GL拡張機能の初期化 */
int FUTL_InitalizeGLExt(void);

/* 文字列の描画(左上原点) */
void FUTL_DrawString(char *lpStr, int x, int y);

/* Perfomance Counter間の時間 */
float FUTL_SubPCounter(LARGE_INTEGER *lpEndTime, LARGE_INTEGER *lpStartTime);

/* テクスチャの読み込み */
int FUTL_LoadTexture(char *name, unsigned int *lpTexID);

/* トーラスモデルの作成と移動 */
int FUTL_MakeTorus(char *texFile);
int FUTL_DrawTorus(int count);
int FUTL_DrawTorusVBO(int count);

/* 光源設定 */
int FUTL_SetLight(int lightID, FLight *lpLight);

/* マテリアル設定 */
int FUTL_SetMaterial(FMaterial *lpMaterial);

/* シェーダの読み込み */
int FUTL_LoadShader(char *vtxShName, char *frgShName, GLuint *lpProg);



/* ************************************************ */
/* float用の算術演算								*/
/* ************************************************ */
/* 平方根 */
/* 平行根逆数 y = 1.0f / (x)^0.5 */
float FUTL_InvSqrt(float x);
float FUTL_Sqrt(float x);
                     
/* 三角関数	*/
float FUTL_Sin(float rad);
float FUTL_Cos(float rad);
float FUTL_Tan(float rad);
float FUTL_Atan(float x);
float FUTL_Atan2(float y, float x);

/* その他の算術関数	*/
float FUTL_Fabs(float x);
float FUTL_Pow(float a, float b);
float FUTL_Floor(float x);
                       
/* ベクトルの正規化（正規化前の長さを返す） */
float FUTL_VecNormalize(FVector3D *lpV);
float FUTL_Vec2DNormalize(FVector2D *lpV);

/* ************************************************ */
/* float用の行列演算								*/
/* ************************************************ */
/* 頂点の座標変換 */
void FUTL_MatVector(FVector3D *lpS, FMatrix *lpM, FVector3D *lpV);

/* 方向ベクトルの座標変換 */
void FUTL_MatDirection(FVector3D *lpSDir, FMatrix *lpM, FVector3D *lpDir);

/* 単位行列化 */
void FUTL_MatIdentity(FMatrix *lpM);

/* 行列コピー */
void FUTL_MatCopy(FMatrix *lpMa, FMatrix *lpMb);

/* 平行移動 */
void FUTL_MatTranslate(FMatrix *lpM, float x, float y, float z);
void FUTL_MatMove(FMatrix *lpM, float x, float y, float z);

/* グローバル回転 */
void FUTL_MatRotateX(FMatrix *lpM, float angle);
void FUTL_MatRotateY(FMatrix *lpM, float angle);
void FUTL_MatRotateZ(FMatrix *lpM, float angle);

/* ローカル回転 */
void FUTL_MatTurnX(FMatrix *lpM, float angle);
void FUTL_MatTurnY(FMatrix *lpM, float angle);
void FUTL_MatTurnZ(FMatrix *lpM, float angle);
      
/* スケーリング */
void FUTL_MatScale(FMatrix *lpM,
			   float scalex, float scaley, float scalez);
void FUTL_MatSize(FMatrix *lpM,
			   float scalex, float scaley, float scalez);

/*
 * クォータニオン
 */
/* クォータニオンの積 */
void FUTL_QuatMult(FQuat *lpR, FQuat *lpP, FQuat *lpQ);

/* クォータニオンを行列に変換 */
void FUTL_QuatToMatrix(FMatrix *lpM, FQuat *lpQ);

/* 回転クォータニオンの作成 */
void FUTL_QuatRotation(FQuat *lpQ, float rad, float ax, float ay, float az);

/* 単位クォータニオンの作成 */
void FUTL_QuatIdentity(FQuat *lpQ);

/* クォータニオンのコピー */
void FUTL_QuatCopy(FQuat *lpTo, FQuat *lpFrom);

#ifdef __cplusplus
};
#endif

#endif	/* __RGFLIB_H */

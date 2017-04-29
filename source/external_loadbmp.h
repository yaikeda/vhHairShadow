/* *****************************************************************************
 * ファイル名		： loadbmp.h
 * 機能				： BMPファイル読み込み用ヘッダ
 * ----------------------------------------------------------------------------
 * 作成者　　		： Arakin
 * ***************************************************************************** */
#ifndef	_LOADBMP_H_
#define	_LOADBMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ファイルヘッダ */
typedef struct _tmpBMPFileHeader {
	unsigned short bfType;			/* ファイルタイプ */
	unsigned int bfSize;			/* ファイルサイズ */
	unsigned short bfReserved1;		/* 予約領域 */
	unsigned short bfReserved2;		/* 予約領域 */
	unsigned int bfOffBits;			/* ファイル先頭から画像データまでのオフセット */
} BMPFileHeader;

/* BMP情報ヘッダ */
typedef struct _tmpBMPInfoHeader {
	unsigned int biSize;			/* 情報ヘッダーのサイズ */
	int biWidth;					/* 幅 */
	int biHeight;					/* 高さ(正ならば下から上、負ならば上から下) */
	unsigned short biPlanes;		/* プレーン数(常に1) */
	unsigned short biBitCount;		/* 1画素あたりのビット数(1,4,8,24,32) */
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;			/* パレットの色数 */
	unsigned int biClrImportant;
} BMPInfoHeader;

/* パレットデータ */
typedef struct _tmpRGBQuad {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQuad;


/*
 * 読み込んだ BMP File を格納する先
 */
typedef struct _tmpImageData {
	unsigned int format;	/* 画像のフォーマット */
	int  width, height;		/* 画像の大きさ */
	void *data;				/* 画像データ */
} ImageData;

#define MAX_COLOR_PALETTE		(256)

int	loadBMP(ImageData *pp, char *name);
int	releaseBMP(ImageData *pp);

#ifdef __cplusplus
}
#endif

#endif	/* _DOC_FILE_ */

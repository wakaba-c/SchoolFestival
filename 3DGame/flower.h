//========================================================================================
//
// 飾り[flower.h]
// Author : masayasu wakita
//
//========================================================================================
#ifndef _FLOWER_H_
#define _FLOWER_H_
//========================================================================================
// インクルードファイル
//========================================================================================
#include "main.h"

//========================================================================================
// 構造体
//========================================================================================
typedef enum
{
	TEXTURETYPE_FLOWER = 0,
	TEXTURETYPE_CONTOUR,
	TEXTURETYPE_CORE,
	TEXTURETYPE_WAVE,
	TEXTURETYPE_MAX
} TEXTURETYPE;

typedef struct
{
	D3DXVECTOR3 pos;							//現在の位置
	D3DXVECTOR3 size;							//サイズ
	D3DXVECTOR3 rot;							//回転情報
	float fAngle;								//
	float fLength;
} FLOWER;

//========================================================================================
// プロトタイプ宣言
//========================================================================================
void InitFlower(void);
void UninitFlower(void);
void UpdateFlower(void);
void DrawFlower(void);
void MakeVertexFlower(int nIndex, D3DXVECTOR3 pos, D3DXVECTOR3 size);
#endif
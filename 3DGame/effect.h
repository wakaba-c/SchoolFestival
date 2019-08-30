// ----------------------------------------------------------------------------//
// エフェクトの処理 [effect.h]
// Author : Ryouma Inoue
//
// ----------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------
#include "main.h"

//=============================================================================
// エフェクトの種類
//=============================================================================
typedef enum
{
	EFFECTTYPE_NORMAL = 0,
	EFFECTTYPE_LIFE,
	EFFECTTYPE_HIBANA,
	EFFECTTYPE_MAX
} EFFECTTYPE;


// ----------------------------------------------------------------------------
// 構造体定義
// ----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;
	D3DXCOLOR col;			// 色
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	EFFECTTYPE type;
	float fRadius;			// 半径
	int nLife;				// ライフ
	bool bUse;				// 使用しているかどうか
} EFFECT;


// ----------------------------------------------------------------------------
// プロトタイプ宣言
// ----------------------------------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife, EFFECTTYPE type);

#endif

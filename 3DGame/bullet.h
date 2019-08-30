//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	BULLETTYPE_NONE = -1,
	BULLETTYPE_ENEMY,
	BULLETTYPE_PLAYER,
	BULLETTYPE_MAX
} BULLETTYPE;
typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 move;										//移動量
	D3DXVECTOR3 rot;										//回転
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	BULLETTYPE	type;										//バレットのタイプ
	bool bUse;												//使用しているかどうか
	int nLife;												//玉の耐久値
	int nIndexShadow;										//影のインデックス
} BULLET;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, BULLETTYPE type);
BULLET *GetBullet(void);
#endif
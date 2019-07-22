//========================================================================================
//
// モデル[model.h]
// Author : masayasu wakita
//
//========================================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//========================================================================================
// インクルードファイル
//========================================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

//========================================================================================
// 構造体
//========================================================================================
typedef enum
{
	COLLISIONTYPE_NORMAL = 0,
	COLLISIONTYPE_WEAPON,
	COLLISIONTYPE_MAX
} COLLISIONTYPE;

typedef enum
{
	LAYOUTTYPE_NONE = -1,
	LAYOUTTYPE_NORMAL,
	LAYOUTTYPE_WALL,
	LAYOUTTYPE_CIRCLE,
	LAYOUTTYPE_GROUP,
	LAYOUTTYPE_MAX
} LAYOUTTYPE;

typedef enum
{
	ITEMTYPE_NONE = -1,
	ITEMTYPE_DESK,
	ITEMTYPE_CHAIR,
	ITEMTYPE_FLOWER,
	ITEMTYPE_STAIRS,
	ITEMTYPE_COMPO,
	ITEMTYPE_MAX
} ITEMTYPE;

//========================================================================================
// プロトタイプ宣言
//========================================================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

void SetModel(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool SphereModel(COLLISIONTYPE  nType,D3DXVECTOR3 *C1, D3DXVECTOR3 *C2, D3DXVECTOR3 *M1, D3DXVECTOR3 *M2, float R1, float R2);

void SetLayout(LAYOUTTYPE nType);
void ChangeModel(void);
#endif
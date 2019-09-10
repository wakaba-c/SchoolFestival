//========================================================================================
//
// スキル[skill.h]
//
// Author : Ryouma Inoue
// 07/18
//
//========================================================================================
#ifndef _SKILL_H_
#define _SKILL_H_

//========================================================================================
// インクルードファイル
//========================================================================================
#include "main.h"

//========================================================================================
// 構造体
//========================================================================================
typedef enum
{
	TEXTURETYPE_ASUKA = 0,
	TEXTURETYPE_ASUKA2,
	TEXTURETYPE_ASUKA3,
	TEXTURETYPE_MAX
} TEXTURETYPE;

typedef struct
{
	D3DXVECTOR3 pos;							//現在の位置
	D3DXVECTOR3 size;							//サイズ
	D3DXVECTOR3 rot;							//回転情報
	TEXTURETYPE type;
	float fAngle;
	float fLength;
	int nCounterAnim;							// カウンター
	int nPatternAnim;							// パターンNo
	bool bUse;
} SKILL;

//========================================================================================
// プロトタイプ宣言
//========================================================================================
void InitSkill(void);
void UninitSkill(void);
void UpdateSkill(void);
void DrawSkill(void);
void MakeVertexSkill(int nIndex, D3DXVECTOR3 pos, D3DXVECTOR3 size, TEXTURETYPE type);
SKILL *GetSkill(void);

#endif
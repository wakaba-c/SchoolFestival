//========================================================================================
//
// polygon基礎[bg.h]
//
// Author : masayasu wakita
// 07/18
//
//========================================================================================
#ifndef _TELOP_H_
#define _TELOP_H_
//========================================================================================
// インクルードファイル
//========================================================================================
#include "main.h"

//========================================================================================
// 構造体定義
//========================================================================================
typedef enum
{
	TELOPTEXTURE_MESSAGEWINDOW = 0,
	TELOPTEXTURE_MESSAGEWIDE,
	TELOPTEXTURE_MESSAGE,
	TELOPTEXTURE_MAX
} TELOPTEXTURE;

typedef enum
{
	TELOPTYPE_MESSAGEWINDOW = 0,
	TELOPTYPE_DETAILS,
	TELOPTYPE_MAX
} TELOPTYPE;

typedef enum
{
	TELOPPAGE_START = 5,
	TELOPPAGE_PHASE1,
	TELOPPAGE_PHASE2,
	TELOPPAGE_PHASE3,
	TELOPPAGE_PHASE4,
	TELOPPAGE_PHASE5,
	TELOPPAGE_END,
	TELOPPAGE_MAX
} TELOPPAGE;

typedef enum
{
	TELOPMODE_NONE = 0,
	TELOPMODE_IN,
	TELOPMODE_OUT,
	TELOPMODE_MAX
} TELOPMODE;

//========================================================================================
// プロトタイプ宣言
//========================================================================================
void InitTelop(void);
void UninitTelop(void);
void UpdateTelop(void);
void DrawTelop(void);
void SetVertexTelop(TELOPTEXTURE index, TELOPPAGE pgae, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);

void SetStageTelop(TELOPTYPE index, TELOPPAGE page);
bool GetTelopState(void);
#endif
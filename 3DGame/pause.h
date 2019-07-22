//========================================================================================
//
// pause処理[pause.h]
// Author : masayasu wakita
//
//========================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_
//========================================================================================
// インクルードファイル
//========================================================================================
#include "game.h"

//========================================================================================
// 構造体定義
//========================================================================================
typedef enum
{
	PAUSESTATE_CONTINUE = 0,
	PAUSESTATE_TITLE,
	PAUSESTATE_MAX
} PAUSESTATE;

//========================================================================================
// プロトタイプ宣言
//========================================================================================
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(bool bPause);
void SetPauseMode(GAMESTATE mode);
GAMESTATE GetPauseMode(void);
void SetVertexPause(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);
#endif
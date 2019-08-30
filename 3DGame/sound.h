//=============================================================================
//
// サウンド処理 [sound.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// サウンドファイル
//=============================================================================
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_SE_GAMEOVER,		// ジャンプ音
	SOUND_LABEL_SE_AIM,			// 魔法の発射音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

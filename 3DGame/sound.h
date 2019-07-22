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
	SOUND_LABEL_SE_JUMP,		// ジャンプ音
	SOUND_LABEL_SE_SLASH,		// ヒュッ
	SOUND_LABEL_MAX,
} SOUND_LABEL;

typedef struct
{
	D3DXVECTOR3		position;										// 位置
	D3DXVECTOR3		rotation;										// 回転
	D3DXVECTOR3		velocity;										// 速度
	D3DXVECTOR3		normalize;										//上方向ベクトル
	float			SourceVoiceChannelVolumes;						// ボリューム
	bool			bUse;											// 3Dサウンドの使用
} SOUND;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label, bool Stereophonic);
void StopSound(SOUND_LABEL label);
void StopSound(void);

void UpdateSound(void);
void UpdateListener(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 nor);
void UpdateEmitter(SOUND_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 nor);
void SetVolume(SOUND_LABEL label, float fVolume);
#endif

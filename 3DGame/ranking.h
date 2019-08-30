//========================================================================================
//
// polygon基礎[bg.h]
//
// Author : masayasu wakita
// 07/18
//
//========================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#define _CRT_SECURE_NO_WARNINGS

//========================================================================================
// インクルードファイル
//========================================================================================
#include "main.h"

//========================================================================================
// 構造体定義
//========================================================================================
typedef enum
{
	RANKINGSTATE_1 = 0,
	RANKINGSTATE_2,
	RANKINGSTATE_3,
	RANKINGSTATE_4,
	RANKINGSTATE_5,
	RANKINGSTATE_6,
	RANKINGSTATE_MAX
} RANKINGSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR	col;
	int			nScore;
} RANKING;

//========================================================================================
// プロトタイプ宣言
//========================================================================================
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void MakeVertexRanking(RANKINGSTATE rank, D3DXVECTOR3 pos, D3DXCOLOR col);
void SetVertexRanking(int index, D3DXVECTOR3 pos, float fWidth, float fHeight, float nNumber);
void ConvertRanking(int nNumber);
void SaveRanking(void);
void LoadRanking(void);
void SetResultIndex(int nScore);
#endif
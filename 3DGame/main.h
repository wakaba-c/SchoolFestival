//=============================================================================
//
// メイン処理 [main.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"
#include "XAudio2.h"
#include "X3DAudio.h"

//=============================================================================
// ライブラリのリンク
//=============================================================================
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "XAudio2.lib")

//=============================================================================
// マクロ定義
//=============================================================================
#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	MODE_NONE = -1,
	MODE_TITLE,
	MODE_GAME,
	MODE_RESULT,
	MODE_RANKING,
	MODE_MAX
} MODE;
// ２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR	col;	//頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_3D;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void SetMode(MODE mode);
MODE GetMode(void);

LPDIRECT3DDEVICE9 GetDevice(void);

#endif
// ==========================================================
// 
// メイン[main.h]
// Author:Ryouma Inoue
// 
// ==========================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

// インクルードファイル
#include <stdio.h>
#include<Windows.h>
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"
#include "xaudio2.h"

// ==========================================================
// ライブラリのリンク
// ==========================================================
#pragma comment(lib,"d3d9.lib")					// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")				// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")				// DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")				// システム時間取得に必要
#pragma comment(lib,"dinput8.lib")				// 入力に必要

// ==========================================================
// マクロ定義
// ==========================================================
#define SCREEN_WIDTH	(1280)					// ウィンドウの幅
#define SCREEN_HEIGHT	(720)					// ウィンドウの高さ
#define MAX_POLGON (500)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 2Dポリゴン情報
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float		rhw;	// 1.0で固定、同次座標
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_2D;

int GetFPS(void);

#endif
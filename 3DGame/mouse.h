//=============================================================================
//
// マウス処理 [mouse.h]
// Author : Kanan Naganawa
//
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	DIMOUSESTATE2 mouseState;
	float posX;
	float posY;
} MOUSE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UpdateMouse(void);
void UninitMouse(void);
bool GetMousePress(int nButton);
bool GetMousePressTrigger(int nButton);
LONG GetMouseX(void);
LONG GetMouseY(void);

#endif

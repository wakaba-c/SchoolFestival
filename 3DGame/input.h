//=============================================================================
//
// 入力処理 [input.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetTriggerKeyboard(int nKey);
bool GetUpKeyboard(int nKey);
bool GetKeyboardPress(int nKey);
#endif

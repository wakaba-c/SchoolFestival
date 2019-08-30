//=============================================================================
//
// DirectX コントローラー処理　[controller.h]
// Author : YUUJI AIDA
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// キーの種類
//=============================================================================
typedef enum
{
	JOYPADKEY_X = 0,				//[0]　Xボタン
	JOYPADKEY_Y,					//[1]　Yボタン
	JOYPADKEY_A,					//[2]　Aボタン
	JOYPADKEY_B,					//[3]　Bボタン
	JOYPADKEY_LEFT_SHOULDER,		//[4]　左手前ボタン
	JOYPADKEY_RIGHT_SHOULDER,		//[5]　右手前ボタン
	JOYPADKEY_LEFT_TRIGGER,			//[6]　左ボタン
	JOYPADKEY_RIGHT_TRIGGER,		//[7]　右ボタン
	JOYPADKEY_LEFT_THUMB,			//[8]　右スティックボタン
	JOYPADKEY_RIGHT_THUMB,			//[9]　左スティックボタン
	JOYPADKEY_BACK,					//[10]　バックボタン
	JOYPADKEY_START,				//[11]　スタートボタン

	JOYPADKEY_LEFT,					//[12]　方向キー[左]
	JOYPADKEY_RIGHT,				//[13]　方向キー[右]
	JOYPADKEY_UP,					//[14]　方向キー[上]
	JOYPADKEY_DOWN,					//[15]　方向キー[下]
	JOYPADKEY_MAX					
}JOYPADKEY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitController(HINSTANCE hInstance, HWND hWnd);
void UninitController(void);
void UpdateController(void);

bool GetControllerPress(int nIDPad, JOYPADKEY nKey);
bool GetControllerTrigger(int nIDPad, JOYPADKEY nKey);
bool GetControllerRelease(int nIDPad, JOYPADKEY nKey);
bool GetControllerRepeat(int nIDPad, JOYPADKEY nKey);

int GetJoypadTriggerLeft(int nIDPad);
int GetJoypadTriggerRight(int nIDPad);

void GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV);
void GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV);

static BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
static BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef);
void SetKeyStateJoypad(int nIDPad);

#endif

//=============================================================================
//
// 入力処理 [mouse.cpp]
// Author : wakita masayasu
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "mouse.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	NUM_KEY_MAX				(256)	// キーの最大数
#define	NUM_BUTTON_MAX			(8)		// ボタン(マウス)の最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECTINPUT8			g_pMouseput = NULL;						// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;						// 入力デバイス(マウス)へのポインタ

MOUSE					g_mouse;								// マウスの情報

BYTE					g_aButtonState[NUM_BUTTON_MAX];			// マウスの入力情報ワーク
BYTE					g_aButtonStateTrigger[NUM_BUTTON_MAX];	// マウスのトリガー

HWND					g_hWnd;									// ウィンドウハンドル

//=============================================================================
// マウスの初期化処理
//=============================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& g_pMouseput, NULL)))
	{
		return E_FAIL;
	}

	g_pMouseput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL);
	g_pDevMouse->SetDataFormat(&c_dfDIMouse2); // マウス用のデータ・フォーマットを設定

	g_pDevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// デバイスの設定    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// 入力制御開始    
	g_pDevMouse->Acquire();

	// 代入
	g_hWnd = hWnd;

	return S_OK;
}

//=============================================================================
// マウスの更新処理
//=============================================================================
void UpdateMouse(void)
{
	// 変数宣言
	int nCntButton;
	POINT point;

	// 値の初期化    
	g_mouse.mouseState.rgbButtons[0] = 0;

	GetCursorPos(&point);
	ScreenToClient(g_hWnd, &point);
	g_mouse.posX = (float)point.x;
	g_mouse.posY = (float)point.y;

	// 値の更新 
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &g_mouse.mouseState)))
	{
		for (nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// 排他的論理和かどうか判断
			g_aButtonStateTrigger[nCntButton] = (g_aButtonState[nCntButton] ^ g_mouse.mouseState.rgbButtons[nCntButton]) & g_mouse.mouseState.rgbButtons[nCntButton];
			g_aButtonState[nCntButton] = g_mouse.mouseState.rgbButtons[nCntButton];	// キープレス情報保存
		}
	}
	else
	{
		g_pDevMouse->Acquire(); // １発目や２発目にエラーが出るが無視してよい。 
	}
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void UninitMouse(void)
{
	// 入力デバイス(マウス)の開放
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();	// マウスへのアクセス権を開放
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
	// DirectInputオブジェクトの開放
	if (g_pMouseput != NULL)
	{
		g_pMouseput->Release();
		g_pMouseput = NULL;
	}
}


//=============================================================================
// マウスのプレス状態を取得
//=============================================================================
bool GetMousePress(int nButton)
{
	return (g_mouse.mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// マウスのトリガー処理
//=============================================================================
bool GetMousePressTrigger(int nButton)
{
	return (g_aButtonStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// マウスのX座標の取得
//=============================================================================
LONG GetMouseX(void)
{
	return (LONG)g_mouse.posX - SCREEN_WIDTH / 2;
}

//=============================================================================
// マウスのY座標の取得
//=============================================================================
LONG GetMouseY(void)
{
	return ((LONG)g_mouse.posY - SCREEN_HEIGHT / 2) * -1;
}
// ==========================================================
//
// キーボード[inputController.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _INPUTCONTROLLER_H_
#define _INPUTCONTROLLER_H_

#include "main.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	NUM_KEY_MAX			(256)						// キーの最大数
#define COUNT_WAIT_REPEAT	(20)						//リピート開始までの待ち時間
#define INTERVAL_REPEAT		(1)							//リピート間隔
#define	NUM_JOYPAD_MAX		(4)							//パッド最大数
#define JOY_MAX_RANGE		(1024)						//パッドの有効範囲

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CInputController : public CInput
{
public:
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

	CInputController();				// コンストラクタ
	~CInputController();				// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetControllerPress(int nIDPad, JOYPADKEY nKey);
	bool GetControllerTrigger(int nIDPad, JOYPADKEY nKey);
	bool GetControllerRelease(int nIDPad, JOYPADKEY nKey);
	bool GetControllerRepeat(int nIDPad, JOYPADKEY nKey);

	int GetJoypadTriggerLeft(int nIDPad);
	int GetJoypadTriggerRight(int nIDPad);

	void GetJoypadStickLeft(int nIDPad, int *pValueH, int *pValueV);
	void GetJoypadStickRight(int nIDPad, int *pValueH, int *pValueV);
	void SetKeyStateJoypad(int nIDPad);

	static BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
	static BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef);

protected:

private:
	LPDIRECTINPUTDEVICE8 m_pDevController = NULL;				//入力デバイス(キーボード)へのポインタ

	BYTE m_aJoyKeyState[NUM_KEY_MAX];
	int m_nCountWaitRepeat = 0;									//リピート待ち時間
	int m_nIntervalRepeat = 0;									//リピート間隔

	DIJOYSTATE2 m_aJoypadState[NUM_JOYPAD_MAX];					//ジョイパッド状態を受け取るワーク
	bool m_aKeyStateJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		//ジョイパッドの押下状態を保持するワーク
	bool m_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//ジョイパッドのトリガー状態を保持するワーク
	bool m_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//ジョイパッドのリリース状態を保持するワーク
	bool m_aKeyStateRepeatJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//ジョイパッドのリピート状態を保持するワーク
	int m_aKeyStateRepeatCntJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];//ジョイパッドのリピートカウンタ
	D3DXVECTOR3 m_aKeyStateAxis[NUM_JOYPAD_MAX];

	static LPDIRECTINPUT8 m_pController;						//Direct Inputオブジェクトへのポインタ
	static LPDIRECTINPUTDEVICE8 m_apDIDevJoypad[NUM_JOYPAD_MAX];	//IDirectInputDevice8へのポインタ（ジョイパッド）
	static int m_nCntPad;
};
#endif
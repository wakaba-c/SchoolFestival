//=============================================================================
//
// DirectX コントローラー処理 [inputController.cpp]
// Author : Ryouma Inoue
//
//=============================================================================
#include "inputController.h"
#include "input.h"

//=============================================================================
// 静的メンバー変数の初期化
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputController::m_apDIDevJoypad[NUM_JOYPAD_MAX];
LPDIRECTINPUT8 CInputController::m_pController = NULL;
int CInputController::m_nCntPad = 0;

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CInputController::CInputController()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CInputController::~CInputController()
{

}

//=============================================================================
// コントローラーの初期化
//=============================================================================
HRESULT CInputController::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pController == NULL)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pController, NULL);

		m_nCountWaitRepeat = COUNT_WAIT_REPEAT;
		m_nIntervalRepeat = INTERVAL_REPEAT;
	}

	for (m_nCntPad = 0; m_nCntPad < NUM_JOYPAD_MAX; m_nCntPad++)
	{
		hr = m_pController->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || m_apDIDevJoypad[m_nCntPad] == NULL)
		{
			//MessageBox(hWnd,"ジョイパッドがありません","警告",MB_ICONWARNING)
			return hr;
		}

		// データフォーマットを設定
		hr = m_apDIDevJoypad[m_nCntPad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ジョイパッドのデータフォーマットを設定できませんでした。", "警告", MB_ICONWARNING);
			return hr;
		}

		// 協調モードを設定（フォアグラウンド＆非排他モード）
		hr = m_apDIDevJoypad[m_nCntPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ジョイパッドの協調モードを設定できませんでした。", "警告", MB_ICONWARNING);
			return hr;
		}

		hr = m_apDIDevJoypad[m_nCntPad]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ジョイパッドが見つかりませんでした。", "警告", MB_ICONWARNING);
			return hr;
		}

		//デバイスの設定
		{
			DIPROPRANGE diprg;

			ZeroMemory(&diprg, sizeof(diprg));
			diprg.diph.dwSize = sizeof(diprg);
			diprg.diph.dwHeaderSize = sizeof(diprg.diph);

			diprg.diph.dwObj = DIJOFS_X;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//X方向の範囲を指定
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_Y;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//Y方向の範囲を指定
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_Z;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//Z方向の範囲を指定
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_RZ;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//RZ方向の範囲を指定
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		}

		// コントローラーへのアクセス権を獲得(入力制御開始)
		m_apDIDevJoypad[m_nCntPad]->Acquire();

		//ワーククリア
		memset(&m_aJoypadState[m_nCntPad], 0, sizeof m_aJoyKeyState[m_nCntPad]);
		memset(m_aKeyStateJoypad[m_nCntPad], 0, sizeof m_aKeyStateJoypad[m_nCntPad]);
		memset(m_aKeyStateTriggerJoypad[m_nCntPad], 0, sizeof m_aKeyStateTriggerJoypad[m_nCntPad]);
		memset(m_aKeyStateReleaseJoypad[m_nCntPad], 0, sizeof m_aKeyStateReleaseJoypad[m_nCntPad]);
		memset(m_aKeyStateRepeatJoypad[m_nCntPad], 0, sizeof m_aKeyStateRepeatJoypad[m_nCntPad]);
	}

	return hr;
}

//=============================================================================
// コントローラーの終了処理
//=============================================================================
void CInputController::Uninit(void)
{
	CInput::Uninit();
	// 入力デバイス(コントローラー)の開放
	if (m_apDIDevJoypad[m_nCntPad] != NULL)
	{
		m_apDIDevJoypad[m_nCntPad]->Unacquire();						//キーボードへのアクセス権を開放
		m_apDIDevJoypad[m_nCntPad]->Release();
		m_apDIDevJoypad[m_nCntPad] = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pController != NULL)
	{
		m_pController->Release();
		m_pController = NULL;
	}
}

//=============================================================================
// コントローラーの更新処理
//=============================================================================
void CInputController::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[JOYPADKEY_MAX];

	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
	{
		if (m_apDIDevJoypad[nCntPad] == NULL)
		{
			return;
		}

		//前回のデータを保存
		for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = m_aKeyStateJoypad[nCntPad][nCntKey];
		}

		//デバイスからデータを取得
		hr = m_apDIDevJoypad[nCntPad]->GetDeviceState(sizeof(m_aJoypadState[nCntPad]), &m_aJoypadState[nCntPad]);
		if (SUCCEEDED(hr))
		{
			//キー情報設定
			SetKeyStateJoypad(nCntPad);
			//軸位置を記録
			m_aKeyStateAxis[nCntPad].x = (float)m_aJoypadState[nCntPad].lX / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntPad].y = (float)m_aJoypadState[nCntPad].lY / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntPad].z = (float)m_aJoypadState[nCntPad].lZ / JOY_MAX_RANGE;

			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				//トリガー・リリース情報の作成
				m_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntPad][nCntKey])&m_aKeyStateJoypad[nCntPad][nCntKey];
				m_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntPad][nCntKey]) & !m_aKeyStateJoypad[nCntPad][nCntKey];

				//リピート情報の作成
				m_aKeyStateRepeatJoypad[nCntPad][nCntKey] = m_aKeyStateTriggerJoypad[nCntPad][nCntKey];
				if (m_aKeyStateJoypad[nCntPad][nCntKey])
				{
					m_aKeyStateRepeatCntJoypad[nCntPad][nCntKey]++;
					if (m_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] < m_nCountWaitRepeat)
					{
						if (m_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] == 1 || m_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] >= m_nCountWaitRepeat)
						{
							m_aKeyStateRepeatJoypad[nCntPad][nCntKey] = m_aKeyStateJoypad[nCntPad][nCntKey];
						}
						else
						{
							m_aKeyStateRepeatJoypad[nCntPad][nCntKey] = 0;
						}
					}
					else
					{
						if (((m_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] - m_nCountWaitRepeat) % m_nIntervalRepeat) == 0)
						{
							m_aKeyStateRepeatJoypad[nCntPad][nCntKey] = m_aKeyStateJoypad[nCntPad][nCntKey];
						}
						else
						{
							m_aKeyStateRepeatJoypad[nCntPad][nCntKey] = 0;
						}
					}
				}
				else
				{
					m_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] = 0;
					m_aKeyStateRepeatJoypad[nCntPad][nCntKey] = 0;
				}
			}
		}
		else
		{
			m_apDIDevJoypad[nCntPad]->Acquire();
		}
	}
}

//=============================================================================
// ジョイスティック問い合わせ用コールバック関数
//=============================================================================
BOOL CALLBACK CInputController::EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[2];	//各デバイスの識別子を格納
	DIDEVCAPS	diDevCaps;				//デバイス情報
	HRESULT		hRes;

	//ジョイスティック用デバイスオブジェクトを作成
	hRes = m_pController->CreateDevice(lpddi->guidInstance, &m_apDIDevJoypad[m_nCntPad], NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;			//列挙を続ける
	}

	//ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = m_apDIDevJoypad[m_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (m_apDIDevJoypad[m_nCntPad])
		{
			m_apDIDevJoypad[m_nCntPad]->Release();
			m_apDIDevJoypad[m_nCntPad] = NULL;
		}
		//列挙を続ける
		return DIENUM_CONTINUE;
	}

	//デバイスの識別子を保存
	pad_discrimination[m_nCntPad] = lpddi->guidInstance;

	//このデバイスを使うので列挙を終了する
	return DIENUM_STOP;
}

BOOL CALLBACK CInputController:: EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef)
{
	HRESULT hr;

	for (int nCntPad = 0; nCntPad < 2; nCntPad++)
	{
		DIPROPRANGE diprg;

		//スティック軸の値の範囲を設定（-32768～32767）
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = lpddi->dwType;
		diprg.diph.dwHow = DIPH_BYID;
		diprg.lMin = -32768;
		diprg.lMax = 32767;

		if (m_apDIDevJoypad[nCntPad] != NULL)
		{
			hr = m_apDIDevJoypad[nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (FAILED(hr))
			{
				return DIENUM_STOP;
			}
		}
	}

	return DIENUM_CONTINUE;

}
//=============================================================================
// ジョイパッドのキー情報設定
//=============================================================================
void CInputController::SetKeyStateJoypad(int nIDPad)
{
	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{//十字キー[左]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{//十字キー[右]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((m_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100) || (m_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{//十字キー[上]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{//十字キー[下]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (m_aJoypadState[nIDPad].rgbButtons[nKey])
		{//ボタンが押されている
			m_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			m_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//=============================================================================
// ジョイパッドの情報取得
//=============================================================================
bool CInputController::GetControllerPress(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateJoypad[nIDPad][nKey];
}

//=============================================================================
// ジョイパッドの情報取得
//=============================================================================
bool CInputController::GetControllerTrigger(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateTriggerJoypad[nIDPad][nKey];
}

//=============================================================================
// ジョイパッドの情報取得
//=============================================================================
bool CInputController::GetControllerRelease(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateReleaseJoypad[nIDPad][nKey];
}

//=============================================================================
// ジョイパッドの情報取得
//=============================================================================
bool CInputController::GetControllerRepeat(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateRepeatJoypad[nIDPad][nKey];
}

//=============================================================================
// ジョイパッドのトリガー（左）の値取得
//=============================================================================
int CInputController::GetJoypadTriggerLeft(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//=============================================================================
// ジョイパッドのトリガー（右）の値取得
//=============================================================================
int CInputController::GetJoypadTriggerRight(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//=============================================================================
// ジョイパッドのアナログスティック（左）の値取得
//=============================================================================
void CInputController::GetJoypadStickLeft(int nIDPad, int *pValueH, int *pValueV)
{
	*pValueH = m_aJoypadState[nIDPad].lX;
	*pValueV = -m_aJoypadState[nIDPad].lY;
}

//=============================================================================
// ジョイパッドのアナログスティック（右）の値取得
//=============================================================================
void CInputController::GetJoypadStickRight(int nIDPad, int *pValueH, int *pValueV)
{
	*pValueH = m_aJoypadState[nIDPad].lZ;
	*pValueV = -m_aJoypadState[nIDPad].lRz;
}
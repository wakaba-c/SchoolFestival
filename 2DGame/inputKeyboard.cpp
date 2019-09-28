// ==========================================================
//
// キーボード[keyboard.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "inputKeyboard.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
BYTE CInputKeyboard::m_aKeyState[NUM_KEY_MAX] = {};
BYTE CInputKeyboard::m_aKeyStateTngger[NUM_KEY_MAX] = {};

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CInputKeyboard::CInputKeyboard()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CInputKeyboard::~CInputKeyboard()
{

}

// ==========================================================
// 初期化処理
// ==========================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevKeyboard->Acquire();
	return S_OK;
}
// ==========================================================
// 終了
// ==========================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
	// 入力デバイス(キーボード)の開放
	if (m_pDevKeyboard != NULL)
	{
		// 入力デバイス（キーボード）の解放
		m_pDevKeyboard->Unacquire();		// キーボードへのアクセス権解放
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}

}

// ==========================================================
// 更新
// ==========================================================
void CInputKeyboard::Update(void)
{
	// キーボードの入力情報
	BYTE aKeyState[NUM_KEY_MAX];
	int nCuntKey;
	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCuntKey = 0; nCuntKey < NUM_KEY_MAX; nCuntKey++)
		{
			// トリガー処理
			m_aKeyStateTngger[nCuntKey] = (m_aKeyState[nCuntKey] ^ aKeyState[nCuntKey])&aKeyState[nCuntKey];
			// キーブレス情報保存
			m_aKeyState[nCuntKey] = aKeyState[nCuntKey];
		}
	}
	else
	{
		// キーボードへのアクセス権取得
		m_pDevKeyboard->Acquire();
	}
}

// ----------------------------------------------------------------------------
// キーボードのプレス状態を取得
// ----------------------------------------------------------------------------
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

// ----------------------------------------------------------------------------
// キーボードのトリガー状態を取得
// ----------------------------------------------------------------------------
bool CInputKeyboard::GetKeyboardTngger(int nKey)
{
	return (m_aKeyStateTngger[nKey] & 0x80) ? true : false;
}
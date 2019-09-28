// ==========================================================
//
// キーボード[inputKeyboard.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "main.h"
#include "input.h"

#define	NUM_KEY_MAX			(256)	// キーの最大数

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();				// コンストラクタ
	~CInputKeyboard();				// デストラクタ
	
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static bool GetKeyboardPress(int nKey);
	static bool GetKeyboardTngger(int nKey);

protected:

private:
	static BYTE m_aKeyState[NUM_KEY_MAX];						// キーボードの入力情報ワーク
	static BYTE m_aKeyStateTngger[NUM_KEY_MAX];				// キーボードトリガーの入力情報ワーク
};
#endif
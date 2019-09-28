// ==========================================================
//
// �L�[�{�[�h[inputKeyboard.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "main.h"
#include "input.h"

#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();				// �R���X�g���N�^
	~CInputKeyboard();				// �f�X�g���N�^
	
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static bool GetKeyboardPress(int nKey);
	static bool GetKeyboardTngger(int nKey);

protected:

private:
	static BYTE m_aKeyState[NUM_KEY_MAX];						// �L�[�{�[�h�̓��͏�񃏁[�N
	static BYTE m_aKeyStateTngger[NUM_KEY_MAX];				// �L�[�{�[�h�g���K�[�̓��͏�񃏁[�N
};
#endif
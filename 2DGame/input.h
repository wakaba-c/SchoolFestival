// ==========================================================
//
// �`��[input.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CInput
{
public:
	CInput();

	virtual~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	static LPDIRECTINPUTDEVICE8 m_pDevKeyboard;				// ���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^

private:

};
#endif
// ==========================================================
//
// �L�[�{�[�h[keyboard.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "inputKeyboard.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
BYTE CInputKeyboard::m_aKeyState[NUM_KEY_MAX] = {};
BYTE CInputKeyboard::m_aKeyStateTngger[NUM_KEY_MAX] = {};

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CInputKeyboard::CInputKeyboard()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CInputKeyboard::~CInputKeyboard()
{

}

// ==========================================================
// ����������
// ==========================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�i�L�[�{�[�h�j�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevKeyboard->Acquire();
	return S_OK;
}
// ==========================================================
// �I��
// ==========================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevKeyboard != NULL)
	{
		// ���̓f�o�C�X�i�L�[�{�[�h�j�̉��
		m_pDevKeyboard->Unacquire();		// �L�[�{�[�h�ւ̃A�N�Z�X�����
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}

}

// ==========================================================
// �X�V
// ==========================================================
void CInputKeyboard::Update(void)
{
	// �L�[�{�[�h�̓��͏��
	BYTE aKeyState[NUM_KEY_MAX];
	int nCuntKey;
	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCuntKey = 0; nCuntKey < NUM_KEY_MAX; nCuntKey++)
		{
			// �g���K�[����
			m_aKeyStateTngger[nCuntKey] = (m_aKeyState[nCuntKey] ^ aKeyState[nCuntKey])&aKeyState[nCuntKey];
			// �L�[�u���X���ۑ�
			m_aKeyState[nCuntKey] = aKeyState[nCuntKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X���擾
		m_pDevKeyboard->Acquire();
	}
}

// ----------------------------------------------------------------------------
// �L�[�{�[�h�̃v���X��Ԃ��擾
// ----------------------------------------------------------------------------
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

// ----------------------------------------------------------------------------
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
// ----------------------------------------------------------------------------
bool CInputKeyboard::GetKeyboardTngger(int nKey)
{
	return (m_aKeyStateTngger[nKey] & 0x80) ? true : false;
}
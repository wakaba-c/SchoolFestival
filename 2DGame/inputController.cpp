//=============================================================================
//
// DirectX �R���g���[���[���� [inputController.cpp]
// Author : Ryouma Inoue
//
//=============================================================================
#include "inputController.h"
#include "input.h"

//=============================================================================
// �ÓI�����o�[�ϐ��̏�����
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputController::m_apDIDevJoypad[NUM_JOYPAD_MAX];
LPDIRECTINPUT8 CInputController::m_pController = NULL;
int CInputController::m_nCntPad = 0;

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CInputController::CInputController()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CInputController::~CInputController()
{

}

//=============================================================================
// �R���g���[���[�̏�����
//=============================================================================
HRESULT CInputController::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pController == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pController, NULL);

		m_nCountWaitRepeat = COUNT_WAIT_REPEAT;
		m_nIntervalRepeat = INTERVAL_REPEAT;
	}

	for (m_nCntPad = 0; m_nCntPad < NUM_JOYPAD_MAX; m_nCntPad++)
	{
		hr = m_pController->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || m_apDIDevJoypad[m_nCntPad] == NULL)
		{
			//MessageBox(hWnd,"�W���C�p�b�h������܂���","�x��",MB_ICONWARNING)
			return hr;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = m_apDIDevJoypad[m_nCntPad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = m_apDIDevJoypad[m_nCntPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		hr = m_apDIDevJoypad[m_nCntPad]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h��������܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		//�f�o�C�X�̐ݒ�
		{
			DIPROPRANGE diprg;

			ZeroMemory(&diprg, sizeof(diprg));
			diprg.diph.dwSize = sizeof(diprg);
			diprg.diph.dwHeaderSize = sizeof(diprg.diph);

			diprg.diph.dwObj = DIJOFS_X;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//X�����͈̔͂��w��
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_Y;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//Y�����͈̔͂��w��
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_Z;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//Z�����͈̔͂��w��
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_RZ;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//RZ�����͈̔͂��w��
			m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		}

		// �R���g���[���[�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_apDIDevJoypad[m_nCntPad]->Acquire();

		//���[�N�N���A
		memset(&m_aJoypadState[m_nCntPad], 0, sizeof m_aJoyKeyState[m_nCntPad]);
		memset(m_aKeyStateJoypad[m_nCntPad], 0, sizeof m_aKeyStateJoypad[m_nCntPad]);
		memset(m_aKeyStateTriggerJoypad[m_nCntPad], 0, sizeof m_aKeyStateTriggerJoypad[m_nCntPad]);
		memset(m_aKeyStateReleaseJoypad[m_nCntPad], 0, sizeof m_aKeyStateReleaseJoypad[m_nCntPad]);
		memset(m_aKeyStateRepeatJoypad[m_nCntPad], 0, sizeof m_aKeyStateRepeatJoypad[m_nCntPad]);
	}

	return hr;
}

//=============================================================================
// �R���g���[���[�̏I������
//=============================================================================
void CInputController::Uninit(void)
{
	CInput::Uninit();
	// ���̓f�o�C�X(�R���g���[���[)�̊J��
	if (m_apDIDevJoypad[m_nCntPad] != NULL)
	{
		m_apDIDevJoypad[m_nCntPad]->Unacquire();						//�L�[�{�[�h�ւ̃A�N�Z�X�����J��
		m_apDIDevJoypad[m_nCntPad]->Release();
		m_apDIDevJoypad[m_nCntPad] = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pController != NULL)
	{
		m_pController->Release();
		m_pController = NULL;
	}
}

//=============================================================================
// �R���g���[���[�̍X�V����
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

		//�O��̃f�[�^��ۑ�
		for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = m_aKeyStateJoypad[nCntPad][nCntKey];
		}

		//�f�o�C�X����f�[�^���擾
		hr = m_apDIDevJoypad[nCntPad]->GetDeviceState(sizeof(m_aJoypadState[nCntPad]), &m_aJoypadState[nCntPad]);
		if (SUCCEEDED(hr))
		{
			//�L�[���ݒ�
			SetKeyStateJoypad(nCntPad);
			//���ʒu���L�^
			m_aKeyStateAxis[nCntPad].x = (float)m_aJoypadState[nCntPad].lX / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntPad].y = (float)m_aJoypadState[nCntPad].lY / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntPad].z = (float)m_aJoypadState[nCntPad].lZ / JOY_MAX_RANGE;

			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				//�g���K�[�E�����[�X���̍쐬
				m_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntPad][nCntKey])&m_aKeyStateJoypad[nCntPad][nCntKey];
				m_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntPad][nCntKey]) & !m_aKeyStateJoypad[nCntPad][nCntKey];

				//���s�[�g���̍쐬
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
// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK CInputController::EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[2];	//�e�f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS	diDevCaps;				//�f�o�C�X���
	HRESULT		hRes;

	//�W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	hRes = m_pController->CreateDevice(lpddi->guidInstance, &m_apDIDevJoypad[m_nCntPad], NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;			//�񋓂𑱂���
	}

	//�W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = m_apDIDevJoypad[m_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (m_apDIDevJoypad[m_nCntPad])
		{
			m_apDIDevJoypad[m_nCntPad]->Release();
			m_apDIDevJoypad[m_nCntPad] = NULL;
		}
		//�񋓂𑱂���
		return DIENUM_CONTINUE;
	}

	//�f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[m_nCntPad] = lpddi->guidInstance;

	//���̃f�o�C�X���g���̂ŗ񋓂��I������
	return DIENUM_STOP;
}

BOOL CALLBACK CInputController:: EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef)
{
	HRESULT hr;

	for (int nCntPad = 0; nCntPad < 2; nCntPad++)
	{
		DIPROPRANGE diprg;

		//�X�e�B�b�N���̒l�͈̔͂�ݒ�i-32768�`32767�j
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
// �W���C�p�b�h�̃L�[���ݒ�
//=============================================================================
void CInputController::SetKeyStateJoypad(int nIDPad)
{
	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{//�\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{//�\���L�[[�E]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((m_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100) || (m_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{//�\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{//�\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (m_aJoypadState[nIDPad].rgbButtons[nKey])
		{//�{�^����������Ă���
			m_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			m_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool CInputController::GetControllerPress(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool CInputController::GetControllerTrigger(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateTriggerJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool CInputController::GetControllerRelease(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateReleaseJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool CInputController::GetControllerRepeat(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateRepeatJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[�i���j�̒l�擾
//=============================================================================
int CInputController::GetJoypadTriggerLeft(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[�i�E�j�̒l�擾
//=============================================================================
int CInputController::GetJoypadTriggerRight(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N�i���j�̒l�擾
//=============================================================================
void CInputController::GetJoypadStickLeft(int nIDPad, int *pValueH, int *pValueV)
{
	*pValueH = m_aJoypadState[nIDPad].lX;
	*pValueV = -m_aJoypadState[nIDPad].lY;
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N�i�E�j�̒l�擾
//=============================================================================
void CInputController::GetJoypadStickRight(int nIDPad, int *pValueH, int *pValueV)
{
	*pValueH = m_aJoypadState[nIDPad].lZ;
	*pValueV = -m_aJoypadState[nIDPad].lRz;
}
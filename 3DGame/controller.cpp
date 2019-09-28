//=============================================================================
//
// DirectX �R���g���[���[���� [controller.cpp]
// Author : YUUJI AIDA
//
//=============================================================================
#include "controller.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_KEY_MAX			(256)						// �L�[�̍ő吔
#define COUNT_WAIT_REPEAT	(20)						//���s�[�g�J�n�܂ł̑҂�����
#define INTERVAL_REPEAT		(1)							//���s�[�g�Ԋu
#define	NUM_JOYPAD_MAX		(2)							//�p�b�h�ő吔
#define JOY_MAX_RANGE		(1024)						//�p�b�h�̗L���͈�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECTINPUT8 g_pController = NULL;						//Direct Input�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevController = NULL;				//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

BYTE g_aJoyKeyState[NUM_KEY_MAX];
int g_nCountWaitRepeat = 0;									//���s�[�g�҂�����
int g_nIntervalRepeat = 0;									//���s�[�g�Ԋu

LPDIRECTINPUTDEVICE8 g_apDIDevJoypad[NUM_JOYPAD_MAX] = {};	//IDirectInputDevice8�ւ̃|�C���^�i�W���C�p�b�h�j
DIJOYSTATE2 g_aJoypadState[NUM_JOYPAD_MAX];					//�W���C�p�b�h��Ԃ��󂯎�郏�[�N
bool g_aKeyStateJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		//�W���C�p�b�h�̉�����Ԃ�ێ����郏�[�N
bool g_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//�W���C�p�b�h�̃g���K�[��Ԃ�ێ����郏�[�N
bool g_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//�W���C�p�b�h�̃����[�X��Ԃ�ێ����郏�[�N
bool g_aKeyStateRepeatJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//�W���C�p�b�h�̃��s�[�g��Ԃ�ێ����郏�[�N
int g_aKeyStateRepeatCntJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];//�W���C�p�b�h�̃��s�[�g�J�E���^
D3DXVECTOR3 g_aKeyStateAxis[NUM_JOYPAD_MAX];
int g_nCntPad = 0;

//=============================================================================
// �R���g���[���[�̏�����
//=============================================================================
HRESULT InitController(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (g_pController == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pController, NULL);

		g_nCountWaitRepeat = COUNT_WAIT_REPEAT;
		g_nIntervalRepeat = INTERVAL_REPEAT;
	}
	
	for (g_nCntPad = 0; g_nCntPad < NUM_JOYPAD_MAX; g_nCntPad++)
	{
		hr = g_pController->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || g_apDIDevJoypad[g_nCntPad] == NULL)
		{
			//MessageBox(hWnd,"�W���C�p�b�h������܂���","�x��",MB_ICONWARNING)
			return hr;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = g_apDIDevJoypad[g_nCntPad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = g_apDIDevJoypad[g_nCntPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		hr = g_apDIDevJoypad[g_nCntPad]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
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
			g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_Y;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//Y�����͈̔͂��w��
			g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_Z;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//Z�����͈̔͂��w��
			g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

			diprg.diph.dwObj = DIJOFS_RZ;
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = -JOY_MAX_RANGE;
			diprg.lMax = JOY_MAX_RANGE;

			//RZ�����͈̔͂��w��
			g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		}

		// �R���g���[���[�ւ̃A�N�Z�X�����l��(���͐���J�n)
		g_apDIDevJoypad[g_nCntPad]->Acquire();

		//���[�N�N���A
		memset(&g_aJoypadState[g_nCntPad], 0, sizeof g_aJoyKeyState[g_nCntPad]);
		memset(g_aKeyStateJoypad[g_nCntPad], 0, sizeof g_aKeyStateJoypad[g_nCntPad]);
		memset(g_aKeyStateTriggerJoypad[g_nCntPad], 0, sizeof g_aKeyStateTriggerJoypad[g_nCntPad]);
		memset(g_aKeyStateReleaseJoypad[g_nCntPad], 0, sizeof g_aKeyStateReleaseJoypad[g_nCntPad]);
		memset(g_aKeyStateRepeatJoypad[g_nCntPad], 0, sizeof g_aKeyStateRepeatJoypad[g_nCntPad]);
	}

	return hr;
}

//=============================================================================
// �R���g���[���[�̏I������
//=============================================================================
void UninitController(void)
{
	//�W���C�p�b�h�̏I������
	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
	{
		// ���̓f�o�C�X(�R���g���[���[)�̊J��
		if (g_apDIDevJoypad[g_nCntPad] != NULL)
		{
			g_apDIDevJoypad[g_nCntPad]->Unacquire();						//�L�[�{�[�h�ւ̃A�N�Z�X�����J��
			g_apDIDevJoypad[g_nCntPad]->Release();
			g_apDIDevJoypad[g_nCntPad] = NULL;
		}

		// DirectInput�I�u�W�F�N�g�̊J��
		if (g_pController != NULL)
		{
			g_pController->Release();
			g_pController = NULL;
		}
	}
}

//=============================================================================
// �R���g���[���[�̍X�V����
//=============================================================================
void UpdateController(void)
{
	HRESULT hr;
	bool aKeyStateOld[JOYPADKEY_MAX];

	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
	{
		if (g_apDIDevJoypad[nCntPad] == NULL)
		{
			return;
		}

		//�O��̃f�[�^��ۑ�
		for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = g_aKeyStateJoypad[nCntPad][nCntKey];
		}

		//�f�o�C�X����f�[�^���擾
		hr = g_apDIDevJoypad[nCntPad]->GetDeviceState(sizeof(g_aJoypadState[nCntPad]), &g_aJoypadState[nCntPad]);
		if (SUCCEEDED(hr))
		{
			//�L�[���ݒ�
			SetKeyStateJoypad(nCntPad);
			//���ʒu���L�^
			g_aKeyStateAxis[nCntPad].x = (float)g_aJoypadState[nCntPad].lX / JOY_MAX_RANGE;
			g_aKeyStateAxis[nCntPad].y = (float)g_aJoypadState[nCntPad].lY / JOY_MAX_RANGE;
			g_aKeyStateAxis[nCntPad].z = (float)g_aJoypadState[nCntPad].lZ / JOY_MAX_RANGE;

			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				//�g���K�[�E�����[�X���̍쐬
				g_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] ^ g_aKeyStateJoypad[nCntPad][nCntKey])&g_aKeyStateJoypad[nCntPad][nCntKey];
				g_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] ^ g_aKeyStateJoypad[nCntPad][nCntKey])&!g_aKeyStateJoypad[nCntPad][nCntKey];

				//���s�[�g���̍쐬
				g_aKeyStateRepeatJoypad[nCntPad][nCntKey] = g_aKeyStateTriggerJoypad[nCntPad][nCntKey];
				if (g_aKeyStateJoypad[nCntPad][nCntKey])
				{
					g_aKeyStateRepeatCntJoypad[nCntPad][nCntKey]++;
					if (g_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] < g_nCountWaitRepeat)
					{
						if (g_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] == 1 || g_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] >= g_nCountWaitRepeat)
						{
							g_aKeyStateRepeatJoypad[nCntPad][nCntKey] = g_aKeyStateJoypad[nCntPad][nCntKey];
						}
						else
						{
							g_aKeyStateRepeatJoypad[nCntPad][nCntKey] = 0;
						}
					}
					else
					{
						if (((g_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] - g_nCountWaitRepeat) % g_nIntervalRepeat) == 0)
						{
							g_aKeyStateRepeatJoypad[nCntPad][nCntKey] = g_aKeyStateJoypad[nCntPad][nCntKey];
						}
						else
						{
							g_aKeyStateRepeatJoypad[nCntPad][nCntKey] = 0;
						}
					}
				}
				else
				{
					g_aKeyStateRepeatCntJoypad[nCntPad][nCntKey] = 0;
					g_aKeyStateRepeatJoypad[nCntPad][nCntKey] = 0;
				}
			}
		}
		else
		{
			g_apDIDevJoypad[nCntPad]->Acquire();
		}
	}
}

//=============================================================================
// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[2];	//�e�f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS	diDevCaps;				//�f�o�C�X���
	HRESULT		hRes;

	//�W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	hRes = g_pController->CreateDevice(lpddi->guidInstance, &g_apDIDevJoypad[g_nCntPad], NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;			//�񋓂𑱂���
	}

	//�W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = g_apDIDevJoypad[g_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (g_apDIDevJoypad[g_nCntPad])
		{
			g_apDIDevJoypad[g_nCntPad]->Release();
			g_apDIDevJoypad[g_nCntPad] = NULL;
		}
		//�񋓂𑱂���
		return DIENUM_CONTINUE;
	}

	//�f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[g_nCntPad] = lpddi->guidInstance;

	//���̃f�o�C�X���g���̂ŗ񋓂��I������
	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef)
{
	HRESULT hr;

	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
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

		if (g_apDIDevJoypad[nCntPad] != NULL)
		{
			hr = g_apDIDevJoypad[nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
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
void SetKeyStateJoypad(int nIDPad)
{
	if (g_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{//�\���L�[[��]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (g_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{//�\���L�[[�E]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((g_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100) || (g_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{//�\���L�[[��]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (g_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{//�\���L�[[��]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (g_aJoypadState[nIDPad].rgbButtons[nKey])
		{//�{�^����������Ă���
			g_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			g_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool GetControllerPress(int nIDPad, JOYPADKEY nKey)
{
	return g_aKeyStateJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool GetControllerTrigger(int nIDPad, JOYPADKEY nKey)
{
	return g_aKeyStateTriggerJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool GetControllerRelease(int nIDPad, JOYPADKEY nKey)
{
	return g_aKeyStateReleaseJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̏��擾
//=============================================================================
bool GetControllerRepeat(int nIDPad, JOYPADKEY nKey)
{
	return g_aKeyStateRepeatJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[�i���j�̒l�擾
//=============================================================================
int GetJoypadTriggerLeft(int nIDPad)
{
	return g_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[�i�E�j�̒l�擾
//=============================================================================
int GetJoypadTriggerRight(int nIDPad)
{
	return g_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N�i���j�̒l�擾
//=============================================================================
void GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV)
{	
	
	*pValueV = -g_aJoypadState[nIDPad].lY / 32767.0f;
	*pValueH = g_aJoypadState[nIDPad].lX / 32767.0f;
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N�i�E�j�̒l�擾
//=============================================================================
void GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueH = g_aJoypadState[nIDPad].lZ / 32767.0f;
	*pValueV = -g_aJoypadState[nIDPad].lRz / 32767.0f;
}
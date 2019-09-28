// ==========================================================
//
// �L�[�{�[�h[inputController.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _INPUTCONTROLLER_H_
#define _INPUTCONTROLLER_H_

#include "main.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_KEY_MAX			(256)						// �L�[�̍ő吔
#define COUNT_WAIT_REPEAT	(20)						//���s�[�g�J�n�܂ł̑҂�����
#define INTERVAL_REPEAT		(1)							//���s�[�g�Ԋu
#define	NUM_JOYPAD_MAX		(4)							//�p�b�h�ő吔
#define JOY_MAX_RANGE		(1024)						//�p�b�h�̗L���͈�

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CInputController : public CInput
{
public:
	//=============================================================================
	// �L�[�̎��
	//=============================================================================
	typedef enum
	{
		JOYPADKEY_X = 0,				//[0]�@X�{�^��
		JOYPADKEY_Y,					//[1]�@Y�{�^��
		JOYPADKEY_A,					//[2]�@A�{�^��
		JOYPADKEY_B,					//[3]�@B�{�^��
		JOYPADKEY_LEFT_SHOULDER,		//[4]�@����O�{�^��
		JOYPADKEY_RIGHT_SHOULDER,		//[5]�@�E��O�{�^��
		JOYPADKEY_LEFT_TRIGGER,			//[6]�@���{�^��
		JOYPADKEY_RIGHT_TRIGGER,		//[7]�@�E�{�^��
		JOYPADKEY_LEFT_THUMB,			//[8]�@�E�X�e�B�b�N�{�^��
		JOYPADKEY_RIGHT_THUMB,			//[9]�@���X�e�B�b�N�{�^��
		JOYPADKEY_BACK,					//[10]�@�o�b�N�{�^��
		JOYPADKEY_START,				//[11]�@�X�^�[�g�{�^��

		JOYPADKEY_LEFT,					//[12]�@�����L�[[��]
		JOYPADKEY_RIGHT,				//[13]�@�����L�[[�E]
		JOYPADKEY_UP,					//[14]�@�����L�[[��]
		JOYPADKEY_DOWN,					//[15]�@�����L�[[��]
		JOYPADKEY_MAX
	}JOYPADKEY;

	CInputController();				// �R���X�g���N�^
	~CInputController();				// �f�X�g���N�^

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
	LPDIRECTINPUTDEVICE8 m_pDevController = NULL;				//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

	BYTE m_aJoyKeyState[NUM_KEY_MAX];
	int m_nCountWaitRepeat = 0;									//���s�[�g�҂�����
	int m_nIntervalRepeat = 0;									//���s�[�g�Ԋu

	DIJOYSTATE2 m_aJoypadState[NUM_JOYPAD_MAX];					//�W���C�p�b�h��Ԃ��󂯎�郏�[�N
	bool m_aKeyStateJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		//�W���C�p�b�h�̉�����Ԃ�ێ����郏�[�N
	bool m_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//�W���C�p�b�h�̃g���K�[��Ԃ�ێ����郏�[�N
	bool m_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//�W���C�p�b�h�̃����[�X��Ԃ�ێ����郏�[�N
	bool m_aKeyStateRepeatJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	//�W���C�p�b�h�̃��s�[�g��Ԃ�ێ����郏�[�N
	int m_aKeyStateRepeatCntJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];//�W���C�p�b�h�̃��s�[�g�J�E���^
	D3DXVECTOR3 m_aKeyStateAxis[NUM_JOYPAD_MAX];

	static LPDIRECTINPUT8 m_pController;						//Direct Input�I�u�W�F�N�g�ւ̃|�C���^
	static LPDIRECTINPUTDEVICE8 m_apDIDevJoypad[NUM_JOYPAD_MAX];	//IDirectInputDevice8�ւ̃|�C���^�i�W���C�p�b�h�j
	static int m_nCntPad;
};
#endif
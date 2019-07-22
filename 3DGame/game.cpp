//========================================================================================
//
// �Q�[���{��[game.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "time.h"
#include "effect.h"
#include "pause.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "effect.h"
#include "gradation.h"
#include "flower.h"
#include "meshField.h"
#include "enemy.h"
#include "meshorbit.h"
#include "tutorial.h"
#include "model.h"
#include "telop.h"
#include "sound.h"
#include "hitpoint.h"

//========================================================================================
// �}�N����`
//========================================================================================

//========================================================================================
// �v���g�^�C�v�錾
//========================================================================================

//========================================================================================
// �O���[�o���ϐ�
//========================================================================================
GAMESTATE g_gameState = GAMESTATE_NORMAL;				//�Q�[�����
int g_gameStateLog = GAMESTATE_NONE;
int g_nCounterGameState = 0;						//�Q�[����ԊǗ��J�E���^�[

bool g_bUseUI = true;										//UI�̕\�����邩

//========================================================================================
// ����������
//========================================================================================
void InitGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �`���[�g���A�����I����Ă��邩�ǂ���
	if (IsFinish())
	{
		g_gameState = GAMESTATE_NORMAL;
	}
	else
	{
		g_gameState = GAMESTATE_TUTORIAL;
	}

	//�`���[�g���A���̏���������
	InitTutorial();

	//�e���b�v�̏���������
	InitTelop();

	//�G�t�F�N�g
	InitEffect();

	//�|�[�Y
	InitPause();

	//�e�̏���������
	InitShadow();

	// �|���S���̏���������
	InitPolygon();

	// �O�Ղ̏�����
	InitOrbit();

	// �ǂ̏���������
	InitWall();

	//�v���C���[�̏���������
	InitPlayer();

	// �G�̏���������
	InitEnemy();

	//�t�B�[���h�̏���������
	InitMeshField();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�O���f�[�V�����̏���������
	InitGradation();

	//flower�̏���������
	InitFlower();

	//hitpoint�̏���������
	InitHitPoint();

	PlaySound(SOUND_LABEL_BGM001, false);

	g_nCounterGameState = 0;
}

//========================================================================================
// �I������
//========================================================================================
void UninitGame(void)
{
	// �`���[�g���A���̏I������
	UninitTutorial();

	//�e���b�v�̏I������
	UninitTelop();

	// 3D�|���S���̏I������
	UninitPolygon();

	// �ǂ̏I������
	UninitWall();

	// ���f���̏I������
	UninitPlayer();

	// �G�̏I������
	UninitEnemy();

	// �e�̏I������
	UninitShadow();

	//�t�B�[���h�̏I������
	UninitMeshField();

	// �O�Ղ̏I������
	UninitOrbit();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	// �O���f�[�V�����̏I������
	UninitGradation();

	// flower�̏I������
	UninitFlower();

	//hitpoint�̏I������
	UninitHitPoint();

}

//========================================================================================
// �X�V����
//========================================================================================
void UpdateGame(void)
{
	if (g_gameState == GAMESTATE_PAUSE)
	{
		UpdatePause();

		if (g_gameState == GAMESTATE_BREAK)
		{
			SetFade(MODE_TITLE);
		}
	}
	else
	{
		if (g_gameState == GAMESTATE_TUTORIAL)
		{
			// �`���[�g���A���̍X�V����
			UpdateTutorial();
		}

		//�e���b�v�̍X�V����
		UpdateTelop();

		// �|���S���̍X�V����
		UpdatePolygon();

		//�t�B�[���h�̍X�V����
		UpdateMeshField();

		//�O�Ղ̕`�揈��
		UpdateOrbit();

		// �ǂ̍X�V����
		UpdateWall();

		// ���f���̍X�V����
		UpdatePlayer();

		// �G�̍X�V����
		UpdateEnemy();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �e�̍X�V����
		UpdateShadow();

		// �O���f�[�V�����̍X�V����
		UpdateGradation();

		// Flower�̍X�V����
		UpdateFlower();

		//hitpoint�̍X�V����
		UpdateHitPoint();
	}

	//�|�[�Y�؂�ւ�
	if (GetTriggerKeyboard(DIK_P) && g_gameState == GAMESTATE_PAUSE || GetControllerTrigger(0, JOYPADKEY_B) && g_gameState == GAMESTATE_PAUSE)
	{
		SetPause(false);
		SetGameState(GetPauseMode());

	}
	else if (GetTriggerKeyboard(DIK_P) && g_gameState != GAMESTATE_PAUSE || GetControllerTrigger(0, JOYPADKEY_B) && g_gameState == GAMESTATE_PAUSE)
	{
		SetPause(true);
		SetGameState(GAMESTATE_PAUSE);
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;
			if (GetFade() == FADE_NONE)
			{
				//��ʃ��[�h�̐ݒ�
				SetFade(MODE_RESULT);
				//SetIsFinish(false);		//�`���[�g���A��
			}
		}
		break;
	}

#ifdef _DEBUG
	if (GetTriggerKeyboard(DIK_F1))
	{
		if (g_bUseUI == true)
		{
			g_bUseUI = false;
		}
		else if (g_bUseUI == false)
		{
			g_bUseUI = true;
		}
	}
#endif // _DEBUG
}

//========================================================================================
// �`�揈��
//========================================================================================
void DrawGame(void)
{
	PLAYER *pPlayer = GetPlayer();

	// �G�̕`�揈��
	DrawEnemy();

	// �|���S���̕`�揈��
	DrawPolygon();

	// �t�B�[���h�̕`�揈��
	DrawMeshField();

	// �ǂ̕`�揈��
	DrawWall();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawShadow();

	//�`���[�g���A���̕`�揈��
	DrawTutorial();

	//�O�Ղ̕`�揈��
	DrawOrbit();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	/*=======UI========*/
	if (g_bUseUI == true)
	{
		// �_���[�W�̕`�揈��
		DrawGradation();

	}

	// ���[�^�[�\��
	DrawFlower();

	//hitpoint�̕`�揈��
	DrawHitPoint();

	//�e���b�v�̕`�揈��
	DrawTelop();

	//�|�[�Y
	DrawPause();
}

//========================================================================================
// �Q�[����Ԃ̐ݒ�
//========================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//========================================================================================
// �Q�[����Ԃ̐ݒ�
//========================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
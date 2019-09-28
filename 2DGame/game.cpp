// ==========================================================
//
// �Q�[��[game.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "game.h"
#include "scene2D.h"
#include "player.h"
#include "bg.h"
#include "explosion.h"
#include "effect.h"
#include "score.h"
#include "number.h"
#include "life.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "time.h"
#include "stock.h"
#include "logo.h"
#include "pause.h"
#include "record.h"
#include "block.h"
#include "item.h"
#include "playerState.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CPlayer *CGame::m_pPlayer = NULL;
CRecord *CGame::m_pRecord = NULL;
CBlock *CGame::m_pBlock = NULL;
CWall *CGame::m_pWall = NULL;
CGame *CGame::m_pGame = NULL;
CEgg *CGame::m_pEgg = NULL;
CItem *CGame::m_pItem = NULL;
D3DXVECTOR3 CGame::m_move = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 CGame::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CGame::m_nCounterGameState = 0;
int CGame::m_nNumBlock = 0;
int CGame::m_nNumItem = 0;
float CGame::m_fposEgg = 0;
int CGame::m_nCntMove = 0;
float CGame::m_fpos = 0;
int CGame::m_nLife = 0;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CGame::CGame()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CGame::~CGame()
{

}

// ==========================================================
// ������
// ==========================================================
void CGame::InitGame(void)
{
	m_nCntMove = 0;
	m_fpos = 0;
	m_fposEgg = 0;

	CBg *pBg;

	// �w�i���[�h
	CBg::Load();

	// �v���C���[���[�h
	CPlayer::Load();

	// �������[�h
	CExplosion::Load();

	// �G�t�F�N�g���[�h
	CEffect::Load();

	// �X�e�[�g���[�h
	CPlayerState::Load();

	// �i���o�[���[�h
	CNumber::Load();

	//�u���b�N���[�h
	CBlock::Load();

	//�A�C�e���̃��[�h
	CItem::Load();

	CPause::Load();

	// �w�i�̐���
	pBg = CBg::Create();
	pBg->SetPos(D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0), PLAYERSIZE_X, PLAYERSIZE_Y, CENTERPOS_NORMAL);

	//�u���b�N�̃��[�h
	LoadBlock(0);

	//�A�C�e���̃��[�h
	LoadItem(0);

	////�A�C�e���̐���
	//m_pItem = CItem::Create(CItem::ITEMTYPE_DASH);
	//m_pItem->SetPos(D3DXVECTOR3(2000, 0, 0), 60, 60, CENTERPOS_NORMAL);

	// ���̐���
	//m_pEgg = CEgg::Create();
	//m_pEgg->SetPos(D3DXVECTOR3(EGG_POS_X, EGG_POS_Y, 0), EGGSIZE_X, EGGSIZE_Y, CENTERPOS_NORMAL);

	CEffect *pEffect;

	CEffect::Load();

	pEffect = CEffect::Create();

	pEffect->SetPos(D3DXVECTOR3(EFFECT_POS_X, EFFECT_POS_Y, 0), EFFECTSIZE_X, EFFECTSIZE_X, CENTERPOS_NORMAL);

	// �X�R�A�̐���
	CScore::Create();

	// �^�C���̐���
	CTime::Create();

	// �X�g�b�N�̐���
	CStock::Create();
	CLogo *pLogo;
	pLogo->Load();
	pLogo = CLogo::Create();

	// ���C�t�Q�[�W�̐���
	CLife *pLife;
	pLife = CLife::Create();
	pLife->SetPos(D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, 0), LIFE_SIZE_X, LIFE_SIZE_Y, CENTERPOS_NORMAL);
}

// ==========================================================
// �I��
// ==========================================================
void CGame::UninitGame(void)
{
	// �w�i�A�����[�h
	CBg::Unload();

	// �v���C���[�A�����[�h
	CPlayer::Unload();

	// �����A�����[�h
	CExplosion::Unload();

	// �X�e�[�g���[�h
	CPlayerState::Unload();

	// �G�t�F�N�g�A�����[�h
	CEffect::Unload();

	// �i���o�[�A�����[�h
	CNumber::Unload();

	//�u���b�N�A�����[�h
	CBlock::Unload();

	//�A�C�e���̃A�����[�h
	CItem::Unload();

	CPause::Unload();

	int nScore = CScore::GetNowScore();

	m_pRecord->SaveData(nScore);

	m_pRecord->LoadData();

	CScene2D::ReleaseAll();
}

// ==========================================================
// �X�V
// ==========================================================
void CGame::UpdateGame(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	//// �L�[�{�[�hP�������ꂽ��
	//if (pInputKeyboard->GetKeyboardPress(DIK_P))
	//{
	//	// �|�[�Y����
	//	CPause *pPause = CPause::Create();
	//	//pPause->SetPause(false);
	//}

	// �v���C���[�̗͎擾
	int nPlayerLife = m_pPlayer->GetLife();

	// �v���C���[���͗��̗̑͂��O�ɂȂ�����
	if (nPlayerLife <= 0)
	{
		// �J�E���^�[���Z
		m_nCounterGameState++;
		// �J�E���g��90�𒴂�����
		if (m_nCounterGameState >= STATE_COUNT)
		{
			// �t�F�[�h
			if (Fade == CFade::FADE_NONE)
			{
				CBg::SetResultState(CBg::RESULTSTATE_FAILD);
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
	}
}

// ==========================================================
// �`��
// ==========================================================
void CGame::DrawGame(void)
{

}

// ==========================================================
// �v���C���[���擾
// ==========================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

// ==========================================================
// �G�̏��擾
// ==========================================================
CBlock *CGame::GetBlock(void)
{
	return m_pBlock;
}

// ==========================================================
// ���̏��擾
// ==========================================================
CEgg *CGame::GetEgg(void)
{
	return m_pEgg;
}

// ==========================================================
// �ǂ̏��擾
// ==========================================================
CWall *CGame::GetWall(void)
{
	return m_pWall;
}

// ==========================================================
// �A�C�e���̏��擾
// ==========================================================
CItem * CGame::GetItem(void)
{
	return m_pItem;
}

// ==========================================================
// �Q�[����Ԃ̐ݒ�
// ==========================================================
void CGame::SetGameState(GAMESTATE state)
{
	m_gameState = state;
	m_nCounterGameState = 0;
}

// ==========================================================
// �Q�[����Ԃ̎擾
// ==========================================================
CGame::GAMESTATE CGame::GetGameState(void)
{
	return m_gameState;
}

// ==========================================================
// ����
// ==========================================================
CGame *CGame::Create(void)
{
	m_pGame = new CGame;
	m_pGame->InitGame();
	return m_pGame;
}

// ==========================================================
// �u���b�N�̃t�@�C���ǂݍ���
// ==========================================================
bool CGame::LoadBlock(int nStageNo)
{
	FILE *pFile;
	CBlock *pBlock;
	char aTextBuff[128];

	pFile = 0;

	// �t�@�C���ǂݍ���
	if (nStageNo == 0)
	{
		pFile = fopen(BLOCK_FILE, "r");
	}
	else if (nStageNo == 1)
	{
		pFile = fopen(BLOCK_FILE, "r");
	}

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aTextBuff[0]);
			// �t�@�C���͈�
			if (strcmp(&aTextBuff[0], "SCRIPT") == 0)
			{
				while (strcmp(&aTextBuff[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aTextBuff[0]);
					if (strcmp(&aTextBuff[0], "numBlock") == 0)
					{
						fscanf(pFile, " = %d", &m_nNumBlock);
					}
					// �G�t�@�C���ǂݍ���
					if (strcmp(&aTextBuff[0], "BLOCKSET") == 0)
					{
						while (strcmp(&aTextBuff[0], "END_BLOCKSET") != 0)
						{
							fscanf(pFile, "%s", &aTextBuff[0]);
							if (strcmp(&aTextBuff[0], "pos") == 0)
							{// �|�W�V����
								fscanf(pFile, " = %f,%f,%f", &m_pos.x, &m_pos.y, &m_pos.z);
							}
						}
						// �u���b�N����
						pBlock = CBlock::Create();
						pBlock->SetPos(m_pos, BLOCKSIZE_X, BLOCKSIZE_Y, CENTERPOS_NORMAL);
						pBlock->SetBlockNum(m_nNumBlock);
					}
				}
				break;
			}
		}
		fclose(pFile);
		return true;
	}
	return false;
}

// ==========================================================
// �A�C�e���̃t�@�C���ǂݍ���
// ==========================================================
bool CGame::LoadItem(int nStageNo)
{
	FILE *pFile;
	CItem *pItem;
	char aTextBuff[128];

	CItem::ITEMTYPE m_type;

	pFile = 0;

	// �t�@�C���ǂݍ���
	if (nStageNo == 0)
	{
		pFile = fopen(ITEM_FILE, "r");
	}
	else if (nStageNo == 1)
	{
		pFile = fopen(ITEM_FILE, "r");
	}

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aTextBuff[0]);
			// �t�@�C���͈�
			if (strcmp(&aTextBuff[0], "SCRIPT") == 0)
			{
				while (strcmp(&aTextBuff[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aTextBuff[0]);
					if (strcmp(&aTextBuff[0], "numItem") == 0)
					{
						fscanf(pFile, " = %d", &m_nNumItem);
					}
					// �G�t�@�C���ǂݍ���
					if (strcmp(&aTextBuff[0], "ITEMSET") == 0)
					{
						while (strcmp(&aTextBuff[0], "END_ITEMSET") != 0)
						{
							fscanf(pFile, "%s", &aTextBuff[0]);
							if (strcmp(&aTextBuff[0], "pos") == 0)
							{// �|�W�V����
								fscanf(pFile, " = %f,%f,%f", &m_pos.x, &m_pos.y, &m_pos.z);
							}
							if (strcmp(&aTextBuff[0], "nType") == 0)
							{// �^�C�v
								fscanf(pFile, " = %d", &m_type);
							}
						}
						// �u���b�N����
						pItem = CItem::Create(m_type);
						pItem->SetPos(m_pos, ITEMSIZE_X, ITEMSIZE_Y, CENTERPOS_NORMAL);
						pItem->SetItemNum(m_nNumItem);
					}
				}
				break;
			}
		}
		fclose(pFile);
		return true;
	}
	return false;
}
// ==========================================================
//
// �Ǘ�[manager.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "game.h"
#include "title.h"
#include "fade.h"
#include "polygon.h"
#include "result.h"
#include "ranking.h"
#include "record.h"
#include "sound.h"
#include "tutorial.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CGame *CManager::m_pGame = NULL;
CTitle *CManager::m_pTitle = NULL;
CSound *CManager::m_pSound = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;
CRenderer *CManager::m_pRenderer = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputController *CManager::m_pInputController = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CManager::CManager()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CManager::~CManager()
{

}

// ==========================================================
// ������
// ==========================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	// �����_���[���I�Ɋm��
	m_pRenderer = new CRenderer;
	// �Q�[�����I�Ɋm��
	m_pGame = new CGame;
	// �L�[�{�[�h���I�Ɋm��
	m_pInputKeyboard = new CInputKeyboard;
	// �R���g���[���[���I�Ɋm��
	m_pInputController = new CInputController;

	// ����������
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return-1;
	}

	// �L�[�{�[�h������
	m_pInputKeyboard->Init(hInstance, hWnd);

	// �R���g���[���[������
	m_pInputController->Init(hInstance, hWnd);

	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

	m_pRecord->InitLoad();

	m_pRecord->InitSave();

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

		break;
	default:
		break;
	}
	
	return S_OK;
}

// ==========================================================
// �I��
// ==========================================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();

	m_pSound->UninitSound();

	if (m_pRenderer != NULL)
	{
		// �����_���[�I��
		m_pRenderer->Uninit();
		// �����_���[�j��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{
		// �I��
		m_pInputKeyboard->Uninit();
		// �j��
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pInputController != NULL)
	{
		// �I��
		m_pInputController->Uninit();
		// �j��
		delete m_pInputController;
		m_pInputController = NULL;
	}
}

// ==========================================================
// �X�V
// ==========================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{
		// �����_���[�X�V
		m_pRenderer->Update();
	}
	if (m_pInputKeyboard != NULL)
	{
		// �L�[�{�[�h�X�V
		m_pInputKeyboard->Update();
	}
	// �R���g���[���[�X�V
	m_pInputController->Update();

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;
	case MODE_GAME:
		m_pGame->UpdateGame();
		break;
	case MODE_RESULT:
		m_pResult->Update();
		break;
	case MODE_RANKING:
		m_pRanking->Update();
		break;
	}
}

// ==========================================================
// �`�揈��
// ==========================================================
void CManager::Draw(void)
{
	// �����_���[�`��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Draw();
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Draw();
		break;
	case MODE_GAME:
		m_pGame->DrawGame();
		break;
	case MODE_RESULT:
		m_pResult->Draw();
		break;
	case MODE_RANKING:
		m_pRanking->Draw();
		break;
	}
}

// ==========================================================
// �����_���[���擾
// ==========================================================
CRenderer *CManager:: GetRenderer(void)
{
	return m_pRenderer;
}

// ==========================================================
// �L�[�{�[�h���擾
// ==========================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

// ==========================================================
// �R���g���[���[���擾
// ==========================================================
CInputController *CManager::GetInputController(void)
{
	return m_pInputController;
}

// ==========================================================
// �T�E���h���擾
// ==========================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

// ==========================================================
// �Q�[�����擾
// ==========================================================
CGame *CManager::GetGame(void)
{
	return m_pGame;
}

// ==========================================================
// ���[�h�擾
// ==========================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

// ==========================================================
// ���[�h�̐ݒ�
// ==========================================================
void CManager::SetMode(MODE mode)
{
	m_pSound->StopSound();

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Uninit();
		// �j��
		delete m_pTitle;
		m_pTitle = NULL;
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
		break;
	case MODE_GAME:
		m_pGame->UninitGame();
		// �j��
		delete m_pGame;
		m_pGame = NULL;
		break;
	case MODE_RESULT:
		m_pResult->Uninit();
		// �j��
		delete m_pResult;
		m_pResult = NULL;
		break;
	case MODE_RANKING:
		m_pRanking->Uninit();
		m_pRanking = NULL;
		break;
	}

	m_mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = CTitle::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		break;
	case MODE_TUTORIAL:
		m_pTutorial->CTutorial::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		break;
	case MODE_GAME:
		m_pGame->CGame::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM001);
		break;
	case MODE_RESULT:
		m_pResult = CResult::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		break;
	case MODE_RANKING:
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		m_pRanking = CRanking::Create();
		break;
	}
}
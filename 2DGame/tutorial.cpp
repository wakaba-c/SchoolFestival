// ==========================================================
//
// �`���[�g���A��[tutorial.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "tutorial.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "logo.h"
#include "inputKeyboard.h"
#include "sound.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CBg *CTutorial::m_pBg = NULL;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CTutorial::CTutorial()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CTutorial::~CTutorial()
{

}

// ==========================================================
// ������
// ==========================================================
void CTutorial::Init(void)
{
	CLogo *pLogo;
	pLogo->Load();

	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(TUTORIAL_BG_X, TUTORIAL_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	pLogo = CLogo::Create();
}

// ==========================================================
// �I��
// ==========================================================
void CTutorial::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// �X�V
// ==========================================================
void CTutorial::Update(void)
{
	CSound *pSound = CManager::GetSound();
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	// �L�[���͂��ꂽ�Ƃ�
	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN))
	{
		if (Fade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ENTER);
			CLogo::SetEnter(true);
			CFade::SetFade(CManager::MODE_GAME);
		}
	}
}

// ==========================================================
// �`�揈��
// ==========================================================
void CTutorial::Draw(void)
{

}

// ==========================================================
// ����
// ==========================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial;
	// �V�[�����I�Ɋm��
	pTutorial = new CTutorial;

	// �V�[��������
	pTutorial->Init();
	// �l��Ԃ�
	return pTutorial;
}
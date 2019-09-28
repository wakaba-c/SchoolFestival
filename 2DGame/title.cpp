// ==========================================================
//
// �^�C�g��[title.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "title.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "inputKeyboard.h"
#include "bg.h"
#include "logo.h"
#include "effect.h"
#include "sound.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
int CTitle::m_nTime = 0;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CTitle::CTitle()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CTitle::~CTitle()
{

}

// ==========================================================
// ������
// ==========================================================
void CTitle::Init(void)
{
	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(TITLE_BG_X, TITLE_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y,CENTERPOS_NORMAL);
	
	m_nTime = 0;
}

// ==========================================================
// �I��
// ==========================================================
void CTitle::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// �X�V
// ==========================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	m_nTime++;

	if (m_nTime > FADE_COUNT)
	{
		if (Fade == CFade::FADE_NONE)
		{
			CFade::SetFade(CManager::MODE_RANKING);
		}
	}

	// �L�[���͂��ꂽ�Ƃ�
	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN))
	{

		if (Fade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ENTER);
			CLogo::SetEnter(true);
			CFade::SetFade(CManager::MODE_TUTORIAL);
		}
	}
}

// ==========================================================
// �`�揈��
// ==========================================================
void CTitle::Draw(void)
{
	
}

// ==========================================================
// ����
// ==========================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle;
	pTitle = new CTitle;
	pTitle->Init();
	return pTitle;
}
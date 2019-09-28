// ==========================================================
//
// ���U���g[result.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "result.h"
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
CBg *CResult::m_pBg = NULL;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CResult::CResult()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CResult::~CResult()
{

}

// ==========================================================
// ������
// ==========================================================
void CResult::Init(void)
{
	CLogo *pLogo;
	pLogo->Load();

	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(RESULT_BG_X, RESULT_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	pLogo = CLogo::Create();
}

// ==========================================================
// �I��
// ==========================================================
void CResult::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// �X�V
// ==========================================================
void CResult::Update(void)
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
			CFade::SetFade(CManager::MODE_RANKING);
		}
	}
}

// ==========================================================
// �`�揈��
// ==========================================================
void CResult::Draw(void)
{
}

// ==========================================================
// ����
// ==========================================================
CResult *CResult::Create(void)
{
	CResult *pResult;
	// �V�[�����I�Ɋm��
	pResult = new CResult;

	// �V�[��������
	pResult->Init();
	// �l��Ԃ�
	return pResult;
}
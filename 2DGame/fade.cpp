// ==========================================================
//
// �t�F�[�h[fade.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "fade.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "player.h"
#include "game.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CFade *CFade::m_pFade;
CFade::FADE CFade::m_fade = FADE_IN;
CManager::MODE CFade::m_modeNext;
D3DXCOLOR CFade::m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CFade::CFade()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CFade::~CFade()
{

}

// ==========================================================
// ������
// ==========================================================
void CFade::Init(CManager::MODE modeNext)
{
	m_modeNext = modeNext;

	// �|���S������
	m_pPolygon = CPolygon::Create();
	m_pPolygon->SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), FADESIZE_X, FADESIZE_Y);
	m_pPolygon->SetColPolygon(m_colorFade);
}

// ==========================================================
// �I��
// ==========================================================
void CFade::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Uninit();
	}
}

// ==========================================================
// �X�V
// ==========================================================
void CFade::Update(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Update();
	}

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_colorFade.a -= 0.01f;
			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += 0.01f;
			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_fade = FADE_IN;
				CManager::SetMode(m_modeNext);
			}
		}
	}

	//���l�ύX 
	m_pPolygon->SetColPolygon(m_colorFade);
}

// ==========================================================
// �`��
// ==========================================================
void CFade::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

// ==========================================================
// ����
// ==========================================================
CFade *CFade::Create(CManager::MODE mode)
{
	// �V�[�����I�Ɋm��
	m_pFade = new CFade();

	// �V�[��������
	m_pFade->Init(mode);
	// �l��Ԃ�
	return m_pFade;
}

// ==========================================================
// ���擾
// ==========================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

// ==========================================================
// ���擾
// ==========================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
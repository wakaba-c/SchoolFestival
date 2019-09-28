// ==========================================================
//
// �̗�[life.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "life.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "player.h"
#include "game.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CLife *CLife::m_pLife;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CLife::CLife(PRIORITY type = CScene::PRIORITY_UI):CScene(type)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_nLife = 0;
	m_fWidth = WIDTH_SIZE;

	// �v���C���[���擾
	CPlayer *pPlayer = CGame::GetPlayer();

	m_fLife = (float)pPlayer->GetLife();
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CLife::~CLife()
{

}

// ==========================================================
// �̗͂̏�����
// ==========================================================
void CLife::Init(void)
{

}

// ==========================================================
// �̗͂̏I��
// ==========================================================
void CLife::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_LIFE; nCntScore++)
	{
		if (m_apPolygon[nCntScore] != NULL)
		{
			m_apPolygon[nCntScore]->Uninit();
		}
	}
}

// ==========================================================
// �̗͂̍X�V
// ==========================================================
void CLife::Update(void)
{
	for (int nCntScore = 0; nCntScore < MAX_LIFE; nCntScore++)
	{
		if (m_apPolygon[nCntScore] != NULL)
		{
			m_apPolygon[nCntScore]->Update();
		}
	}
}

// ==========================================================
// �̗͂̕`��
// ==========================================================
void CLife::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_LIFE; nCntScore++)
	{
		if (m_apPolygon[nCntScore] != NULL)
		{
			m_apPolygon[nCntScore]->Draw();
		}
	}
}

// ==========================================================
// �̗͂̐���
// ==========================================================
CLife *CLife::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pLife = new CLife(CScene::PRIORITY_UI);

	// �V�[��������
	m_pLife->Init();
	// �l��Ԃ�
	return m_pLife;
}

// ==========================================================
// �̗͏��擾
// ==========================================================
CLife *CLife::GetLife(void)
{
	return m_pLife;
}

// ==========================================================
// �̗͏��擾
// ==========================================================
void CLife::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
	m_fWidth = fSizeX;
	m_fSize = fSizeY;

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		m_apPolygon[nCntLife] = CPolygon::Create();
		m_apPolygon[nCntLife]->SetPolygon(m_pos, m_fWidth, (int)m_fSize);
		m_apPolygon[nCntLife]->SetColPolygon(D3DXCOLOR(1.0f - 1.0f * nCntLife, 1.0f, 1.0f - 0.75f * nCntLife, 1.0f));
	}
}

// ==========================================================
// �̗͏��擾
// ==========================================================
void CLife::SetLife(int nLife)
{
	m_nLife = nLife;
}

// ==========================================================
// �̗͏��擾
// ==========================================================
void CLife::AddLife(int nLife)
{
	float fLifeMax = (float)nLife;
	float fLifeNow;
	float fWidth;
	float fResidue;

	fLifeNow = fLifeMax / m_fLife;

	fWidth = fLifeNow * m_fWidth;

	fResidue = 1 - fLifeNow;

	if (fLifeNow > 0.5)
	{
		m_apPolygon[1]->SetPolygon(m_pos, m_fWidth, (int)m_fSize);
		m_apPolygon[1]->SetColPolygon(D3DXCOLOR(0.0f + fResidue * 2, 1.0f, 0.25f - (fResidue / 2), 1.0f));
	}
	else
	{
		m_apPolygon[1]->SetPolygon(m_pos, m_fWidth,(int)m_fSize);
		m_apPolygon[1]->SetColPolygon(D3DXCOLOR(1.0f, fLifeNow * 2, 0.0f, 1.0f));
	}

	m_apPolygon[1]->SetLife(fWidth);
}
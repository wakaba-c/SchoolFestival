// ==========================================================
//
// �X�R�A[score.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CScore *CScore::m_pScore;
int CScore::m_nScore = 0;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CScore::CScore(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CScore::~CScore()
{

}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
void CScore::Init(void)
{
	m_nScore = 0;
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create();
		m_apNumber[nCntScore]->SetPosNum(D3DXVECTOR3(SCORE_POS_X + SCORE_INTERVAL_X * nCntScore, SCORE_POS_Y, 0.0f), NUMBERSIZE_X, NUMBERSIZE_Y);
	}
}

// ==========================================================
// �X�R�A�̏I��
// ==========================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
		}
	}
}

// ==========================================================
// �X�R�A�̍X�V
// ==========================================================
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Update();
		}
	}
}

// ==========================================================
// �X�R�A�̕`��
// ==========================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

// ==========================================================
// �X�R�A�̐���
// ==========================================================
CScore *CScore::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pScore = new CScore(CScene::PRIORITY_UI);

	// �V�[��������
	m_pScore->Init();

	return m_pScore;
}

// ==========================================================
// �X�R�A�ݒ�
// ==========================================================
void CScore::AddScore(int nValue)
{
	int nCount;
	int nNumber;

	m_nScore += nValue;

	for (nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		nNumber = m_nScore % (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - nCount) / (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - 1.0f - nCount);

		if (m_nScore <= 0)
		{
			m_nScore = 0;
		}
		m_apNumber[nCount]->SetNumber(nNumber);
	}
}

// ==========================================================
// �X�R�A���擾
// ==========================================================
CScore *CScore::GetScore(void)
{
	return m_pScore;
}

// ==========================================================
// �X�R�A���擾
// ==========================================================
void CScore::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}

// ==========================================================
// �X�R�A�ݒ�
// ==========================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

// ==========================================================
// �X�R�A���擾
// ==========================================================
int CScore::GetNowScore(void)
{
	return m_nScore;
}
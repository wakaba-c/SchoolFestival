// ==========================================================
//
// �����L���O����[rankingNum.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "rankingNum.h"
#include "renderer.h"
#include "manager.h"
#include "record.h"
#include "number.h"
#include "score.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CRankingNum *CRankingNum::m_pRankingNum;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CRankingNum::CRankingNum(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CRankingNum::~CRankingNum()
{

}

// ==========================================================
// �����L���O�����̏�����
// ==========================================================
void CRankingNum::Init(void)
{
	for (int nCntScore = 0; nCntScore < RANK_NUM; nCntScore++)
	{
		for (int nCnt = 0; nCnt < RANK_SCORE; nCnt++)
		{
			m_apNumber[nCntScore][nCnt] = CNumber::Create();
			m_apNumber[nCntScore][nCnt]->SetPosNum(D3DXVECTOR3(NUM_POS_X + NUM_INTERVAL_X * nCnt, NUM_POS_Y + NUM_INTERVAL_Y * nCntScore, 0.0f), NUM_SIZE_X, NUM_SIZE_Y);
		}
	}

	m_nScore = CScore::GetNowScore();
}

// ==========================================================
// �����L���O�����̏I��
// ==========================================================
void CRankingNum::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < RANK_NUM; nCntScore++)
	{
		for (int nCnt = 0; nCnt < RANK_SCORE; nCnt++)
		{
			if (m_apNumber[nCntScore][nCnt] != NULL)
			{
				m_apNumber[nCntScore][nCnt]->Uninit();
			}
		}
	}
}

// ==========================================================
// �����L���O�����̍X�V
// ==========================================================
void CRankingNum::Update(void)
{
	for (int nCntScore = 0; nCntScore < RANK_NUM; nCntScore++)
	{
		for (int nCnt = 0; nCnt < RANK_SCORE; nCnt++)
		{
			if (m_apNumber[nCntScore][nCnt] != NULL)
			{
				m_apNumber[nCntScore][nCnt]->Update();
			}
		}
	}
}

// ==========================================================
// �����L���O�����̕`��
// ==========================================================
void CRankingNum::Draw(void)
{
	for (int nCntScore = 0; nCntScore < RANK_NUM; nCntScore++)
	{
		for (int nCnt = 0; nCnt < RANK_SCORE; nCnt++)
		{
			if (m_apNumber[nCntScore][nCnt] != NULL)
			{
				m_apNumber[nCntScore][nCnt]->Draw();
			}
		}
	}
}

// ==========================================================
// �����L���O�����̐���
// ==========================================================
CRankingNum *CRankingNum::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pRankingNum = new CRankingNum(CScene::PRIORITY_UI);

	// �V�[��������
	m_pRankingNum->Init();

	return m_pRankingNum;
}

// ==========================================================
// �����L���O�����ݒ�
// ==========================================================
void CRankingNum::AddRankingNum(int nValue)
{
	int nCount;
	int nNumber;

	for (nCount = 0; nCount < RANK_NUM; nCount++)
	{
		m_nRanking[nCount] = *CRecord::GetLoad(nCount);

		for (int nCnt = 0; nCnt < RANK_SCORE; nCnt++)
		{
			nNumber = m_nRanking[nCount] % (int)powf(10.0f, 8.0f - nCnt) / (int)powf(10.0f, 8.0f - 1.0f - nCnt);

			m_apNumber[nCount][nCnt]->SetNumber(nNumber);
		}
	}
}

// ==========================================================
// �����L���O�������擾
// ==========================================================
CRankingNum *CRankingNum::GetRank(void)
{
	return m_pRankingNum;
}

// ==========================================================
// �����L���O�������擾
// ==========================================================
void CRankingNum::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}
// ==========================================================
//
// ランキング数字[rankingNum.cpp]
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
// 静的メンバー変数の初期化
// ==========================================================
CRankingNum *CRankingNum::m_pRankingNum;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CRankingNum::CRankingNum(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CRankingNum::~CRankingNum()
{

}

// ==========================================================
// ランキング数字の初期化
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
// ランキング数字の終了
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
// ランキング数字の更新
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
// ランキング数字の描画
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
// ランキング数字の生成
// ==========================================================
CRankingNum *CRankingNum::Create(void)
{
	// シーン動的に確保
	m_pRankingNum = new CRankingNum(CScene::PRIORITY_UI);

	// シーン初期化
	m_pRankingNum->Init();

	return m_pRankingNum;
}

// ==========================================================
// ランキング数字設定
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
// ランキング数字情報取得
// ==========================================================
CRankingNum *CRankingNum::GetRank(void)
{
	return m_pRankingNum;
}

// ==========================================================
// ランキング数字情報取得
// ==========================================================
void CRankingNum::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}
// ==========================================================
//
// スコア[score.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CScore *CScore::m_pScore;
int CScore::m_nScore = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CScore::CScore(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CScore::~CScore()
{

}

// ==========================================================
// スコアの初期化
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
// スコアの終了
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
// スコアの更新
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
// スコアの描画
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
// スコアの生成
// ==========================================================
CScore *CScore::Create(void)
{
	// シーン動的に確保
	m_pScore = new CScore(CScene::PRIORITY_UI);

	// シーン初期化
	m_pScore->Init();

	return m_pScore;
}

// ==========================================================
// スコア設定
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
// スコア情報取得
// ==========================================================
CScore *CScore::GetScore(void)
{
	return m_pScore;
}

// ==========================================================
// スコア情報取得
// ==========================================================
void CScore::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}

// ==========================================================
// スコア設定
// ==========================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

// ==========================================================
// スコア情報取得
// ==========================================================
int CScore::GetNowScore(void)
{
	return m_nScore;
}
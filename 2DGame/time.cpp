// ==========================================================
//
// タイム[time.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "time.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"
#include "fade.h"
#include "manager.h"
#include "bg.h"
#include "player.h"
#include "score.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CTime::CTime(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
	m_nTime = 0;
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CTime::~CTime()
{

}

// ==========================================================
// タイムの初期化
// ==========================================================
void CTime::Init(void)
{
	m_dStartTime = START_TIMU * FRAME_TIMU;
	for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create();
		m_apNumber[nCntScore]->SetPosNum(D3DXVECTOR3(TIME_POS_X + TIME_INTERVAL_X * nCntScore, TIME_POS_Y, 0.0f), NUMBERSIZE_X, NUMBERSIZE_Y);
	}
}

// ==========================================================
// タイムの終了
// ==========================================================
void CTime::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
		}
	}
}

// ==========================================================
// タイムの更新
// ==========================================================
void CTime::Update(void)
{
	CScore *pScore = CScore::GetScore();
	int nPlayerLife = CPlayer::GetLife();

	if (nPlayerLife > 0)
	{
		for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
		{
			if (m_apNumber[nCntScore] != NULL)
			{
				m_apNumber[nCntScore]->Update();
			}
		}
		m_dStartTime++;
		if (m_dStartTime >= 0)
		{
			SetTime(m_dStartTime);
		}

		if (m_dStartTime == -1)
		{
			CBg::SetResultState(CBg::RESULTSTATE_CLEAR);
			// 画面遷移
			CFade::SetFade(CManager::MODE_RESULT);
		}
	}
	// スコア
	pScore->AddScore(1);

}

// ==========================================================
// タイムの描画
// ==========================================================
void CTime::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

// ==========================================================
// タイムの生成
// ==========================================================
CTime *CTime::Create(void)
{
	// シーン動的に確保
	CTime *pTime = new CTime(CScene::PRIORITY_UI);

	// シーン初期化
	pTime->Init();

	return pTime;
}

// ----------------------------------------------------------------------------
// タイムの変更
// ----------------------------------------------------------------------------
void CTime::SetTime(int nTime)
{
	int nCount;
	int nNumber;

	nTime = nTime / FRAME_TIMU + 1;

	for (nCount = 0; nCount < MAX_TIME; nCount++)
	{
		nNumber = nTime % (int)powf(TIME_INTERVAL, (float)MAX_TIME - nCount) / (int)powf(TIME_INTERVAL, MAX_TIME - 1.0f - nCount);

		m_apNumber[nCount]->SetNumber(nNumber);
	}

}

// ==========================================================
// 情報取得
// ==========================================================
void CTime::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}
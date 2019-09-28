// ==========================================================
//
// ランキング[ranking.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "inputKeyboard.h"
#include "rankingNum.h"
#include "renderer.h"
#include "manager.h"
#include "ranking.h"
#include "effect.h"
#include "number.h"
#include "logo.h"
#include "fade.h"
#include "bg.h"
#include "sound.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
int CRanking::m_nTime = NULL;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CRanking::CRanking()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CRanking::~CRanking()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CRanking::Init(void)
{
	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(RANKING_BG_X, RANKING_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	CNumber::Load();
	CRankingNum *pRankingNuhm = CRankingNum::Create();
	pRankingNuhm->AddRankingNum(0);

	CLogo *pLogo;
	pLogo->Load();
	pLogo = CLogo::Create();

	m_nTime = 0;
}

// ==========================================================
// 終了
// ==========================================================
void CRanking::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// 更新
// ==========================================================
void CRanking::Update(void)
{
	CSound *pSound = CManager::GetSound();
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	m_nTime++;

	// キー入力されたとき
	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN) || m_nTime > TIME_COUNT)
	{
		if (Fade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ENTER);
			CLogo::SetEnter(true);
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}
}

// ==========================================================
// 描画処理
// ==========================================================
void CRanking::Draw(void)
{

}

// ==========================================================
// 生成
// ==========================================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking;
	pRanking = new CRanking;
	pRanking->Init();
	return pRanking;
}
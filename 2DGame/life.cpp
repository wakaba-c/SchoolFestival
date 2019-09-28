// ==========================================================
//
// 体力[life.cpp]
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
// 静的メンバー変数の初期化
// ==========================================================
CLife *CLife::m_pLife;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CLife::CLife(PRIORITY type = CScene::PRIORITY_UI):CScene(type)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_nLife = 0;
	m_fWidth = WIDTH_SIZE;

	// プレイヤー情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	m_fLife = (float)pPlayer->GetLife();
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CLife::~CLife()
{

}

// ==========================================================
// 体力の初期化
// ==========================================================
void CLife::Init(void)
{

}

// ==========================================================
// 体力の終了
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
// 体力の更新
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
// 体力の描画
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
// 体力の生成
// ==========================================================
CLife *CLife::Create(void)
{
	// シーン動的に確保
	m_pLife = new CLife(CScene::PRIORITY_UI);

	// シーン初期化
	m_pLife->Init();
	// 値を返す
	return m_pLife;
}

// ==========================================================
// 体力情報取得
// ==========================================================
CLife *CLife::GetLife(void)
{
	return m_pLife;
}

// ==========================================================
// 体力情報取得
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
// 体力情報取得
// ==========================================================
void CLife::SetLife(int nLife)
{
	m_nLife = nLife;
}

// ==========================================================
// 体力情報取得
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
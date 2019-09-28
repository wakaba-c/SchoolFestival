// ==========================================================
//
// ストック[stock.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "stock.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CStock *CStock::m_pStock;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CStock::CStock(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
	m_nStock = NUM_STOCK;
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CStock::~CStock()
{

}

// ==========================================================
// スコアの初期化
// ==========================================================
void CStock::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create();
		m_apNumber[nCnt]->SetPosNum(D3DXVECTOR3(STOCK_POS_X + STOCK_INTERVAL_X * nCnt, STOCK_POS_Y, 0.0f), NUMBERSIZE_X, NUMBERSIZE_Y);
	}
	m_apNumber[1]->SetNumber(m_nStock);
}

// ==========================================================
// スコアの終了
// ==========================================================
void CStock::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
		}
	}
}

// ==========================================================
// スコアの更新
// ==========================================================
void CStock::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Update();
		}
	}
}

// ==========================================================
// スコアの描画
// ==========================================================
void CStock::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

// ==========================================================
// スコアの生成
// ==========================================================
CStock *CStock::Create(void)
{
	// シーン動的に確保
	m_pStock = new CStock(CScene::PRIORITY_UI);

	// シーン初期化
	m_pStock->Init();

	return m_pStock;
}

// ==========================================================
// スコア設定
// ==========================================================
void CStock::AddStock(int nValue)
{
	int nCount;
	int nNumber;

	m_nStock += nValue;

	for (nCount = 0; nCount < MAX_STOCK; nCount++)
	{
		nNumber = m_nStock % (int)powf(STOCK_INTERVAL, (float)MAX_STOCK - nCount) / (int)powf(STOCK_INTERVAL, (float)MAX_STOCK - 1.0f - nCount);
		if (m_nStock <= 0)
		{
			m_nStock = 0;
		}
		m_apNumber[nCount]->SetNumber(nNumber);
	}
}

// ==========================================================
// スコア情報取得
// ==========================================================
CStock *CStock::GetStock(void)
{
	return m_pStock;
}

// ==========================================================
// スコア情報取得
// ==========================================================
void CStock::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}

// ==========================================================
// スコア情報取得
// ==========================================================
void CStock::SetStock(int nStock)
{
	m_nStock = nStock;
}
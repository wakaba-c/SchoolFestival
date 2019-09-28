// ==========================================================
//
// フェード[fade.cpp]
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
// 静的メンバー変数の初期化
// ==========================================================
CFade *CFade::m_pFade;
CFade::FADE CFade::m_fade = FADE_IN;
CManager::MODE CFade::m_modeNext;
D3DXCOLOR CFade::m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CFade::CFade()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CFade::~CFade()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CFade::Init(CManager::MODE modeNext)
{
	m_modeNext = modeNext;

	// ポリゴン生成
	m_pPolygon = CPolygon::Create();
	m_pPolygon->SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), FADESIZE_X, FADESIZE_Y);
	m_pPolygon->SetColPolygon(m_colorFade);
}

// ==========================================================
// 終了
// ==========================================================
void CFade::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Uninit();
	}
}

// ==========================================================
// 更新
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

	//α値変更 
	m_pPolygon->SetColPolygon(m_colorFade);
}

// ==========================================================
// 描画
// ==========================================================
void CFade::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

// ==========================================================
// 生成
// ==========================================================
CFade *CFade::Create(CManager::MODE mode)
{
	// シーン動的に確保
	m_pFade = new CFade();

	// シーン初期化
	m_pFade->Init(mode);
	// 値を返す
	return m_pFade;
}

// ==========================================================
// 情報取得
// ==========================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

// ==========================================================
// 情報取得
// ==========================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
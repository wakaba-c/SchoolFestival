// ==========================================================
//
// 背景[bg.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "bg.h"
#include "renderer.h"
#include "manager.h"
#include "logo.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CBg *CBg::m_pBg;
LPDIRECT3DTEXTURE9 CBg::m_pTex = NULL;
CBg::RESULTSTATE CBg::m_ResultState = RESULTSTATE_CLEAR;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CBg::CBg(PRIORITY type = CScene::PRIORITY_BG):CScene2D(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CBg::~CBg()
{

}

// ==========================================================
// 背景の初期化
// ==========================================================
void CBg::Init(void)
{
	m_move = D3DXVECTOR3(0, 0, 0);

	CScene2D::Init();
}

// ==========================================================
// 背景の終了
// ==========================================================
void CBg::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// 背景の更新
// ==========================================================
void CBg::Update(void)
{
	D3DXVECTOR3 posBG;

	CScene2D::Update();

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		//シーン2D更新処理
		CScene2D::Update();

		//位置取得
		posBG = GetPos();

		m_move.x += 0.005f;

		if (m_move.y > 1.0f)
		{
			m_move.y = 0.0f;
		}

		if (m_move.x > 1.0f)
		{
			m_move.x = 0.0f;
		}

		//位置の設定
		SetPos(posBG, SCREEN_WIDTH, SCREEN_HEIGHT, CENTERPOS_NORMAL);

		//テクスチャ動かす
		SetMove(1.0f, 1.0f, m_move);
	}
}

// ==========================================================
// 背景の描画
// ==========================================================
void CBg::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// 背景の生成
// ==========================================================
CBg *CBg::Create(void)
{
	// シーン動的に確保
	m_pBg = new CBg(CScene::PRIORITY_BG);

	// シーン初期化
	m_pBg->Init();

	m_pBg->BindTex(m_pTex);

	// 値を返す
	return m_pBg;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CBg::Load(void)
{
	CLogo *pLogo;
	pLogo->Load();

	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (CManager::GetMode() == CManager::MODE_TITLE)
	{
		pLogo = CLogo::Create();
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTex);
	}
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTex);
	}
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, TEXTURE_BG, &m_pTex);
	}
	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		if (m_ResultState == RESULTSTATE_CLEAR)
		{

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT, &m_pTex);
		}
		else if (m_ResultState == RESULTSTATE_FAILD)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, TEXTURE_FAILD, &m_pTex);
		}
	}
	if (CManager::GetMode() == CManager::MODE_RANKING)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTex);
	}

	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CBg::Unload(void)
{
	// テクスチャ解放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// 背景移動
// ==========================================================
void CBg::MoveBg(D3DXVECTOR3 fMove)
{
	m_pBg->SetMove(1.0f, 1.0f, fMove);
}

// ==========================================================
// ステート
// ==========================================================
void CBg::SetResultState(RESULTSTATE state)
{
	m_ResultState = state;
}

// ==========================================================
// ステート取得
// ==========================================================
CBg::RESULTSTATE CBg::GetResultState(void)
{
	return m_ResultState;
}
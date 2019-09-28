// ==========================================================
//
// ポーズ[pause.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "bg.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "game.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CPause::PAUSETYPE CPause::m_type = PAUSETYPE_PAUSE;
LPDIRECT3DTEXTURE9 CPause::m_pTex[PAUSETYPE_MAX] = {};
bool CPause::m_bEnter = false;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CPause::CPause(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CPause::~CPause()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CPause::Init(void)
{
	m_bEnter = false;
	CBg::RESULTSTATE state = CBg::GetResultState();
	for (int nCnt = 0; nCnt < PAUSETYPE_MAX; nCnt++)
	{
		m_pScene2D[nCnt] = CScene2D::Create();
		m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);

		switch (nCnt)
		{
		case PAUSETYPE_PAUSE:
			SetLogo(D3DXVECTOR3(PAUSE_POSX, PAUSE_POSY, 0.0f), PAUSE_X, PAUSE_Y, PAUSETYPE_PAUSE);
			break;

		case PAUSETYPE_TITLE:
			SetLogo(D3DXVECTOR3(TITlE_POSX, TITlE_POSY, 0.0f), TITlE_X, TITlE_Y, PAUSETYPE_TITLE);
			break;

		case PAUSETYPE_TUDUKERU:
			SetLogo(D3DXVECTOR3(TUDUKERU_POSX, TUDUKERU_POSY, 0.0f), TUDUKERU_X, TUDUKERU_Y, PAUSETYPE_TUDUKERU);
			break;

		case PAUSETYPE_MARK:
			SetLogo(D3DXVECTOR3(SAKANA_POSX, SAKANA_POSY, 0.0f), SAKANA_X, SAKANA_Y, PAUSETYPE_MARK);
			break;
		}
	}

}

// ==========================================================
// 終了
// ==========================================================
void CPause::Uninit(void)
{

}

// ==========================================================
// 更新
// ==========================================================
void CPause::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	//選択肢の戻し処理
	if (m_nSelect < 0)
	{
		m_nSelect = 1;
	}
	if (m_nSelect > 1)
	{
		m_nSelect = 0;
	}

	//選択肢の処理
	if (pInputKeyboard->GetKeyboardTngger(DIK_DOWNARROW))
	{
		m_nSelect += 1;
	}
	if (pInputKeyboard->GetKeyboardTngger(DIK_UPARROW))
	{
		m_nSelect -= 1;
	}

	//選択肢の表示処理
	if (m_nSelect == 0)
	{
		m_pScene2D[PAUSETYPE_MARK]->SetPos(D3DXVECTOR3(SAKANA_POSX, SAKANA_POSY, 0), SAKANA_X, SAKANA_Y, 2);
	}
	else if (m_nSelect == 1)
	{
		m_pScene2D[PAUSETYPE_MARK]->SetPos(D3DXVECTOR3(SAKANA_POSX_2, SAKANA_POSY_2, 0), SAKANA_X, SAKANA_Y, 2);
	}

	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN))
	{
		if (m_nSelect == 0)
		{
			SetPause(false);
			CGame::SetGameState(CGame::GAMESTATE_NORMAL);
		}
		else if (m_nSelect == 1)
		{
			CFade::SetFade(CManager::MODE_TITLE);
			SetPause(false);
		}
	}

	if (m_bPause == false)
	{
		Release();
	}
}

// ==========================================================
// 描画
// ==========================================================
void CPause::Draw(void)
{

}

// ==========================================================
// 生成
// ==========================================================
CPause *CPause::Create(void)
{
	// シーン動的に確保
	CPause *pPause = new CPause(CScene::PRIORITY_UI);

	// シーン初期化
	pPause->Init();

	// 値を返す
	return pPause;
}

// ==========================================================
// 情報
// ==========================================================
void CPause::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{

}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CPause::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE, &m_pTex[PAUSETYPE_PAUSE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE_TITLE, &m_pTex[PAUSETYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TUDUKERU, &m_pTex[PAUSETYPE_TUDUKERU]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MARK, &m_pTex[PAUSETYPE_MARK]);

	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CPause::Unload(void)
{
	for (int nCnt = 0; nCnt < PAUSETYPE_MAX; nCnt++)
	{
		// テクスチャ解放
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ==========================================================
// タイプ
// ==========================================================
CPause::PAUSETYPE CPause::GetType(void)
{
	return m_type;
}

// ==========================================================
// 作成
// ==========================================================
void CPause::SetLogo(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt)
{
	m_pScene2D[nCnt]->SetPos(pos, fSizeX, fSizeY, CENTERPOS_NORMAL);
}

//========================================================================================
// ポーズ設定
//========================================================================================
void CPause::SetPause(bool bPause)
{
	m_bPause = bPause;
}
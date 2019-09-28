// ==========================================================
//
// ロゴ[logo.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "bg.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CLogo::LOGOTYPE CLogo::m_type = LOGOTYPE_TITLE;
LPDIRECT3DTEXTURE9 CLogo::m_pTex[LOGOTYPE_MAX] = {};
bool CLogo::m_bEnter = false;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CLogo::CLogo(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CLogo::~CLogo()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CLogo::Init(void)
{
	m_bEnter = false;
	m_nCounterAnim = 0;

	CBg::RESULTSTATE state = CBg::GetResultState();

	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		if (CManager::GetMode() == CManager::MODE_TITLE)
		{
			if (nCnt <= LOGOTYPE_ENTER)
			{
				m_pScene2D[nCnt] = CScene2D::Create();
				m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);
			}
			switch (nCnt)
			{
			//case LOGOTYPE_GRIFFON:
			//	SetLogo(D3DXVECTOR3(GRIFFON_POSX, GRIFFON_POSY, 0.0f), GRIFFONSIZE_X, GRIFFONSIZE_X, LOGOTYPE_GRIFFON);
			//	break;

			case LOGOTYPE_TITLE:
				SetLogo(D3DXVECTOR3(640, 180, 0.0f), LOGOSIZE_X, LOGOSIZE_Y, LOGOTYPE_TITLE);
				break;

			case LOGOTYPE_ENTER:
				m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y,0.0f, 0);
				SetLogo(D3DXVECTOR3(ENTER_POSX, ENTER_POSY, 0.0f), ENTERSIZE_X, ENTERSIZE_Y, LOGOTYPE_ENTER);
				break;
			}
		}
		if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			if (nCnt == LOGOTYPE_ENTER || nCnt == LOGOTYPE_TUTORIAL)
			{
				m_pScene2D[nCnt] = CScene2D::Create();
				m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);
			}
			switch (nCnt)
			{
			case LOGOTYPE_TUTORIAL:
				SetLogo(D3DXVECTOR3(640, 180, 0.0f), 1280, LOGOSIZE_Y, LOGOTYPE_TUTORIAL);
				break;

			case LOGOTYPE_ENTER:
				m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y, 0.0f, 0);
				SetLogo(D3DXVECTOR3(TUTORIAL_ENTER_POSX, TUTORIAL_ENTER_POSY, 0.0f), ENTERSIZE_X, ENTERSIZE_Y, LOGOTYPE_ENTER);
				break;
			}
		}
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			if (nCnt == LOGOTYPE_UI )
			{
				m_pScene2D[nCnt] = CScene2D::Create();
				m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);
			}
			switch (nCnt)
			{

			case LOGOTYPE_UI:
				m_pScene2D[LOGOTYPE_UI]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetLogo(D3DXVECTOR3(UI_POSX, UI_POSY, 0.0f), SCREEN_WIDTH, UISIZE_Y, LOGOTYPE_UI);
				break;
			}
		}
		if (CManager::GetMode() == CManager::MODE_RESULT && state == CBg::RESULTSTATE_CLEAR)
		{
			if (nCnt == LOGOTYPE_ENTER || nCnt == LOGOTYPE_CLEAR)
			{
				m_pScene2D[nCnt] = CScene2D::Create();
				m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);
			}
			switch (nCnt)
			{
			case LOGOTYPE_CLEAR:
				SetLogo(D3DXVECTOR3(CLEAR_POSX, CLEAR_POSY, 0.0f), CLEARSIZE_X, CLEARSIZE_Y, LOGOTYPE_CLEAR);
				break;

			case LOGOTYPE_ENTER:
				m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y, 0.0f, 0);
				SetLogo(D3DXVECTOR3(ENTER_POSX, ENTER_POSY, 0.0f), ENTERSIZE_X, ENTERSIZE_Y, LOGOTYPE_ENTER);
				break;
			}
		}
		if (CManager::GetMode() == CManager::MODE_RESULT && state == CBg::RESULTSTATE_FAILD)
		{
			if (nCnt == LOGOTYPE_ENTER || nCnt == LOGOTYPE_FAILD)
			{
				m_pScene2D[nCnt] = CScene2D::Create();
				m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);
			}
			switch (nCnt)
			{
			case LOGOTYPE_FAILD:
				SetLogo(D3DXVECTOR3(FAILD_POSX, FAILD_POSY, 0.0f), FAILDSIZE_X, FAILDSIZE_Y, LOGOTYPE_FAILD);
				break;

			case LOGOTYPE_ENTER:
				m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y, 0.0f, 0);
				SetLogo(D3DXVECTOR3(ENTER_POSX, ENTER_POSY, 0.0f), ENTERSIZE_X, ENTERSIZE_Y, LOGOTYPE_ENTER);
				break;
			}
		}
		if (CManager::GetMode() == CManager::MODE_RANKING)
		{
			if (nCnt == LOGOTYPE_ENTER || nCnt > LOGOTYPE_UI)
			{
				m_pScene2D[nCnt] = CScene2D::Create();
				m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);
			}
			switch (nCnt)
			{
			case LOGOTYPE_RANKING:
				SetLogo(D3DXVECTOR3(RANK_POSX, RANK_POSY, 0.0f), CLEARSIZE_X, CLEARSIZE_Y, LOGOTYPE_RANKING);
				break;

			case LOGOTYPE_RANKNUM:
				SetLogo(D3DXVECTOR3(RANKNUM_POSX, RANKNUM_POSY, 0.0f), RANKNUMSIZE_X, RANKNUMSIZE_Y, LOGOTYPE_RANKNUM);
				break;

			case LOGOTYPE_ENTER:
				m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y, 0.0f, 0);
				SetLogo(D3DXVECTOR3(ENTER_POSX, RANKENTER_POS, 0.0f), ENTERSIZE_X, ENTERSIZE_Y, LOGOTYPE_ENTER);
				break;
			}
		}
	}
}

// ==========================================================
// 終了
// ==========================================================
void CLogo::Uninit(void)
{

}

// ==========================================================
// 更新
// ==========================================================
void CLogo::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	// パターン切り替え
	m_nCounterAnim++;
	if (CManager::GetMode() != CManager::MODE_GAME)
	{
		if (m_nCounterAnim % LOGOANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % LOGOANIM_COUNT;

			m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y, 0.0f, m_nPatternAnim);
		}
		if (m_bEnter == true)
		{
			if (m_nCounterAnim % LOGOANIM_INTERVAL_2 == 0)
			{
				m_nPatternAnim = (m_nPatternAnim + 1) % LOGOANIM_COUNT;

				m_pScene2D[LOGOTYPE_ENTER]->SetAnimation(LOGOTEX_X, LOGOTEX_Y, 0.0f, m_nPatternAnim);
			}
		}
	}
}

// ==========================================================
// 描画
// ==========================================================
void CLogo::Draw(void)
{

}

// ==========================================================
// 生成
// ==========================================================
CLogo *CLogo::Create(void)
{
	// シーン動的に確保
	CLogo *pLogo = new CLogo(CScene::PRIORITY_UI);

	// シーン初期化
	pLogo->Init();

	// 値を返す
	return pLogo;
}

// ==========================================================
// 情報
// ==========================================================
void CLogo::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{

}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CLogo::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, LOGO_TITLE, &m_pTex[LOGOTYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENTER, &m_pTex[LOGOTYPE_ENTER]);
	D3DXCreateTextureFromFile(pDevice, LOGO_TUTORIAL, &m_pTex[LOGOTYPE_TUTORIAL]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_CLEAR, &m_pTex[LOGOTYPE_CLEAR]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_FAILDLOGO, &m_pTex[LOGOTYPE_FAILD]);

	//D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTex[LOGOTYPE_BULLET]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_UI, &m_pTex[LOGOTYPE_UI]);
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_PLAYER, &m_pTex[LOGOTYPE_PLAYER]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &m_pTex[LOGOTYPE_RANKING]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKNUM, &m_pTex[LOGOTYPE_RANKNUM]);

	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
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
CLogo::LOGOTYPE CLogo::GetType(void)
{
	return m_type;
}

// ==========================================================
// 作成
// ==========================================================
void CLogo::SetLogo(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt)
{
	m_pScene2D[nCnt]->SetPos(pos, fSizeX, fSizeY,CENTERPOS_NORMAL);
}

// ==========================================================
// 状態管理
// ==========================================================
void CLogo::SetEnter(bool bEnter)
{
	m_bEnter = bEnter;
}
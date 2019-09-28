// ==========================================================
//
// タイトル[title.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "title.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "inputKeyboard.h"
#include "bg.h"
#include "logo.h"
#include "effect.h"
#include "sound.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
int CTitle::m_nTime = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CTitle::CTitle()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CTitle::~CTitle()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CTitle::Init(void)
{
	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(TITLE_BG_X, TITLE_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y,CENTERPOS_NORMAL);
	
	m_nTime = 0;
}

// ==========================================================
// 終了
// ==========================================================
void CTitle::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// 更新
// ==========================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	m_nTime++;

	if (m_nTime > FADE_COUNT)
	{
		if (Fade == CFade::FADE_NONE)
		{
			CFade::SetFade(CManager::MODE_RANKING);
		}
	}

	// キー入力されたとき
	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN))
	{

		if (Fade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ENTER);
			CLogo::SetEnter(true);
			CFade::SetFade(CManager::MODE_TUTORIAL);
		}
	}
}

// ==========================================================
// 描画処理
// ==========================================================
void CTitle::Draw(void)
{
	
}

// ==========================================================
// 生成
// ==========================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle;
	pTitle = new CTitle;
	pTitle->Init();
	return pTitle;
}
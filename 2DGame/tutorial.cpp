// ==========================================================
//
// チュートリアル[tutorial.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "tutorial.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "logo.h"
#include "inputKeyboard.h"
#include "sound.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CBg *CTutorial::m_pBg = NULL;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CTutorial::CTutorial()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CTutorial::~CTutorial()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CTutorial::Init(void)
{
	CLogo *pLogo;
	pLogo->Load();

	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(TUTORIAL_BG_X, TUTORIAL_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	pLogo = CLogo::Create();
}

// ==========================================================
// 終了
// ==========================================================
void CTutorial::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// 更新
// ==========================================================
void CTutorial::Update(void)
{
	CSound *pSound = CManager::GetSound();
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	// キー入力されたとき
	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN))
	{
		if (Fade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ENTER);
			CLogo::SetEnter(true);
			CFade::SetFade(CManager::MODE_GAME);
		}
	}
}

// ==========================================================
// 描画処理
// ==========================================================
void CTutorial::Draw(void)
{

}

// ==========================================================
// 生成
// ==========================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial;
	// シーン動的に確保
	pTutorial = new CTutorial;

	// シーン初期化
	pTutorial->Init();
	// 値を返す
	return pTutorial;
}
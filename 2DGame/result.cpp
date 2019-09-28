// ==========================================================
//
// リザルト[result.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "result.h"
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
CBg *CResult::m_pBg = NULL;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CResult::CResult()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CResult::~CResult()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CResult::Init(void)
{
	CLogo *pLogo;
	pLogo->Load();

	m_pBg->Load();
	m_pBg = CBg::Create();
	m_pBg->SetPos(D3DXVECTOR3(RESULT_BG_X, RESULT_BG_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	pLogo = CLogo::Create();
}

// ==========================================================
// 終了
// ==========================================================
void CResult::Uninit(void)
{
	m_pBg->Unload();
	CScene2D::ReleaseAll();
}

// ==========================================================
// 更新
// ==========================================================
void CResult::Update(void)
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
			CFade::SetFade(CManager::MODE_RANKING);
		}
	}
}

// ==========================================================
// 描画処理
// ==========================================================
void CResult::Draw(void)
{
}

// ==========================================================
// 生成
// ==========================================================
CResult *CResult::Create(void)
{
	CResult *pResult;
	// シーン動的に確保
	pResult = new CResult;

	// シーン初期化
	pResult->Init();
	// 値を返す
	return pResult;
}
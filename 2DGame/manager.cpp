// ==========================================================
//
// 管理[manager.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "game.h"
#include "title.h"
#include "fade.h"
#include "polygon.h"
#include "result.h"
#include "ranking.h"
#include "record.h"
#include "sound.h"
#include "tutorial.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CGame *CManager::m_pGame = NULL;
CTitle *CManager::m_pTitle = NULL;
CSound *CManager::m_pSound = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;
CRenderer *CManager::m_pRenderer = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputController *CManager::m_pInputController = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CManager::CManager()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CManager::~CManager()
{

}

// ==========================================================
// 初期化
// ==========================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	// レンダラー動的に確保
	m_pRenderer = new CRenderer;
	// ゲーム動的に確保
	m_pGame = new CGame;
	// キーボード動的に確保
	m_pInputKeyboard = new CInputKeyboard;
	// コントローラー動的に確保
	m_pInputController = new CInputController;

	// 初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return-1;
	}

	// キーボード初期化
	m_pInputKeyboard->Init(hInstance, hWnd);

	// コントローラー初期化
	m_pInputController->Init(hInstance, hWnd);

	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

	m_pRecord->InitLoad();

	m_pRecord->InitSave();

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

		break;
	default:
		break;
	}
	
	return S_OK;
}

// ==========================================================
// 終了
// ==========================================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();

	m_pSound->UninitSound();

	if (m_pRenderer != NULL)
	{
		// レンダラー終了
		m_pRenderer->Uninit();
		// レンダラー破棄
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{
		// 終了
		m_pInputKeyboard->Uninit();
		// 破棄
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pInputController != NULL)
	{
		// 終了
		m_pInputController->Uninit();
		// 破棄
		delete m_pInputController;
		m_pInputController = NULL;
	}
}

// ==========================================================
// 更新
// ==========================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{
		// レンダラー更新
		m_pRenderer->Update();
	}
	if (m_pInputKeyboard != NULL)
	{
		// キーボード更新
		m_pInputKeyboard->Update();
	}
	// コントローラー更新
	m_pInputController->Update();

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;
	case MODE_GAME:
		m_pGame->UpdateGame();
		break;
	case MODE_RESULT:
		m_pResult->Update();
		break;
	case MODE_RANKING:
		m_pRanking->Update();
		break;
	}
}

// ==========================================================
// 描画処理
// ==========================================================
void CManager::Draw(void)
{
	// レンダラー描画
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Draw();
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Draw();
		break;
	case MODE_GAME:
		m_pGame->DrawGame();
		break;
	case MODE_RESULT:
		m_pResult->Draw();
		break;
	case MODE_RANKING:
		m_pRanking->Draw();
		break;
	}
}

// ==========================================================
// レンダラー情報取得
// ==========================================================
CRenderer *CManager:: GetRenderer(void)
{
	return m_pRenderer;
}

// ==========================================================
// キーボード情報取得
// ==========================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

// ==========================================================
// コントローラー情報取得
// ==========================================================
CInputController *CManager::GetInputController(void)
{
	return m_pInputController;
}

// ==========================================================
// サウンド情報取得
// ==========================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

// ==========================================================
// ゲーム情報取得
// ==========================================================
CGame *CManager::GetGame(void)
{
	return m_pGame;
}

// ==========================================================
// モード取得
// ==========================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

// ==========================================================
// モードの設定
// ==========================================================
void CManager::SetMode(MODE mode)
{
	m_pSound->StopSound();

	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Uninit();
		// 破棄
		delete m_pTitle;
		m_pTitle = NULL;
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
		break;
	case MODE_GAME:
		m_pGame->UninitGame();
		// 破棄
		delete m_pGame;
		m_pGame = NULL;
		break;
	case MODE_RESULT:
		m_pResult->Uninit();
		// 破棄
		delete m_pResult;
		m_pResult = NULL;
		break;
	case MODE_RANKING:
		m_pRanking->Uninit();
		m_pRanking = NULL;
		break;
	}

	m_mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = CTitle::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		break;
	case MODE_TUTORIAL:
		m_pTutorial->CTutorial::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		break;
	case MODE_GAME:
		m_pGame->CGame::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM001);
		break;
	case MODE_RESULT:
		m_pResult = CResult::Create();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		break;
	case MODE_RANKING:
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM000);
		m_pRanking = CRanking::Create();
		break;
	}
}
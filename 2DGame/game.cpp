// ==========================================================
//
// ゲーム[game.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "game.h"
#include "scene2D.h"
#include "player.h"
#include "bg.h"
#include "explosion.h"
#include "effect.h"
#include "score.h"
#include "number.h"
#include "life.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "time.h"
#include "stock.h"
#include "logo.h"
#include "pause.h"
#include "record.h"
#include "block.h"
#include "item.h"
#include "playerState.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CPlayer *CGame::m_pPlayer = NULL;
CRecord *CGame::m_pRecord = NULL;
CBlock *CGame::m_pBlock = NULL;
CWall *CGame::m_pWall = NULL;
CGame *CGame::m_pGame = NULL;
CEgg *CGame::m_pEgg = NULL;
CItem *CGame::m_pItem = NULL;
D3DXVECTOR3 CGame::m_move = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 CGame::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CGame::m_nCounterGameState = 0;
int CGame::m_nNumBlock = 0;
int CGame::m_nNumItem = 0;
float CGame::m_fposEgg = 0;
int CGame::m_nCntMove = 0;
float CGame::m_fpos = 0;
int CGame::m_nLife = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CGame::CGame()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CGame::~CGame()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CGame::InitGame(void)
{
	m_nCntMove = 0;
	m_fpos = 0;
	m_fposEgg = 0;

	CBg *pBg;

	// 背景ロード
	CBg::Load();

	// プレイヤーロード
	CPlayer::Load();

	// 爆発ロード
	CExplosion::Load();

	// エフェクトロード
	CEffect::Load();

	// ステートロード
	CPlayerState::Load();

	// ナンバーロード
	CNumber::Load();

	//ブロックロード
	CBlock::Load();

	//アイテムのロード
	CItem::Load();

	CPause::Load();

	// 背景の生成
	pBg = CBg::Create();
	pBg->SetPos(D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f), BGSIZE_X, BGSIZE_Y, CENTERPOS_NORMAL);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0), PLAYERSIZE_X, PLAYERSIZE_Y, CENTERPOS_NORMAL);

	//ブロックのロード
	LoadBlock(0);

	//アイテムのロード
	LoadItem(0);

	////アイテムの生成
	//m_pItem = CItem::Create(CItem::ITEMTYPE_DASH);
	//m_pItem->SetPos(D3DXVECTOR3(2000, 0, 0), 60, 60, CENTERPOS_NORMAL);

	// 卵の生成
	//m_pEgg = CEgg::Create();
	//m_pEgg->SetPos(D3DXVECTOR3(EGG_POS_X, EGG_POS_Y, 0), EGGSIZE_X, EGGSIZE_Y, CENTERPOS_NORMAL);

	CEffect *pEffect;

	CEffect::Load();

	pEffect = CEffect::Create();

	pEffect->SetPos(D3DXVECTOR3(EFFECT_POS_X, EFFECT_POS_Y, 0), EFFECTSIZE_X, EFFECTSIZE_X, CENTERPOS_NORMAL);

	// スコアの生成
	CScore::Create();

	// タイムの生成
	CTime::Create();

	// ストックの生成
	CStock::Create();
	CLogo *pLogo;
	pLogo->Load();
	pLogo = CLogo::Create();

	// ライフゲージの生成
	CLife *pLife;
	pLife = CLife::Create();
	pLife->SetPos(D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, 0), LIFE_SIZE_X, LIFE_SIZE_Y, CENTERPOS_NORMAL);
}

// ==========================================================
// 終了
// ==========================================================
void CGame::UninitGame(void)
{
	// 背景アンロード
	CBg::Unload();

	// プレイヤーアンロード
	CPlayer::Unload();

	// 爆発アンロード
	CExplosion::Unload();

	// ステートロード
	CPlayerState::Unload();

	// エフェクトアンロード
	CEffect::Unload();

	// ナンバーアンロード
	CNumber::Unload();

	//ブロックアンロード
	CBlock::Unload();

	//アイテムのアンロード
	CItem::Unload();

	CPause::Unload();

	int nScore = CScore::GetNowScore();

	m_pRecord->SaveData(nScore);

	m_pRecord->LoadData();

	CScene2D::ReleaseAll();
}

// ==========================================================
// 更新
// ==========================================================
void CGame::UpdateGame(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	//// キーボードPを押されたら
	//if (pInputKeyboard->GetKeyboardPress(DIK_P))
	//{
	//	// ポーズ生成
	//	CPause *pPause = CPause::Create();
	//	//pPause->SetPause(false);
	//}

	// プレイヤー体力取得
	int nPlayerLife = m_pPlayer->GetLife();

	// プレイヤー又は卵の体力が０になったら
	if (nPlayerLife <= 0)
	{
		// カウンター加算
		m_nCounterGameState++;
		// カウントが90を超えたら
		if (m_nCounterGameState >= STATE_COUNT)
		{
			// フェード
			if (Fade == CFade::FADE_NONE)
			{
				CBg::SetResultState(CBg::RESULTSTATE_FAILD);
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
	}
}

// ==========================================================
// 描画
// ==========================================================
void CGame::DrawGame(void)
{

}

// ==========================================================
// プレイヤー情報取得
// ==========================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

// ==========================================================
// 敵の情報取得
// ==========================================================
CBlock *CGame::GetBlock(void)
{
	return m_pBlock;
}

// ==========================================================
// 卵の情報取得
// ==========================================================
CEgg *CGame::GetEgg(void)
{
	return m_pEgg;
}

// ==========================================================
// 壁の情報取得
// ==========================================================
CWall *CGame::GetWall(void)
{
	return m_pWall;
}

// ==========================================================
// アイテムの情報取得
// ==========================================================
CItem * CGame::GetItem(void)
{
	return m_pItem;
}

// ==========================================================
// ゲーム状態の設定
// ==========================================================
void CGame::SetGameState(GAMESTATE state)
{
	m_gameState = state;
	m_nCounterGameState = 0;
}

// ==========================================================
// ゲーム状態の取得
// ==========================================================
CGame::GAMESTATE CGame::GetGameState(void)
{
	return m_gameState;
}

// ==========================================================
// 生成
// ==========================================================
CGame *CGame::Create(void)
{
	m_pGame = new CGame;
	m_pGame->InitGame();
	return m_pGame;
}

// ==========================================================
// ブロックのファイル読み込み
// ==========================================================
bool CGame::LoadBlock(int nStageNo)
{
	FILE *pFile;
	CBlock *pBlock;
	char aTextBuff[128];

	pFile = 0;

	// ファイル読み込み
	if (nStageNo == 0)
	{
		pFile = fopen(BLOCK_FILE, "r");
	}
	else if (nStageNo == 1)
	{
		pFile = fopen(BLOCK_FILE, "r");
	}

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aTextBuff[0]);
			// ファイル範囲
			if (strcmp(&aTextBuff[0], "SCRIPT") == 0)
			{
				while (strcmp(&aTextBuff[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aTextBuff[0]);
					if (strcmp(&aTextBuff[0], "numBlock") == 0)
					{
						fscanf(pFile, " = %d", &m_nNumBlock);
					}
					// 敵ファイル読み込み
					if (strcmp(&aTextBuff[0], "BLOCKSET") == 0)
					{
						while (strcmp(&aTextBuff[0], "END_BLOCKSET") != 0)
						{
							fscanf(pFile, "%s", &aTextBuff[0]);
							if (strcmp(&aTextBuff[0], "pos") == 0)
							{// ポジション
								fscanf(pFile, " = %f,%f,%f", &m_pos.x, &m_pos.y, &m_pos.z);
							}
						}
						// ブロック生成
						pBlock = CBlock::Create();
						pBlock->SetPos(m_pos, BLOCKSIZE_X, BLOCKSIZE_Y, CENTERPOS_NORMAL);
						pBlock->SetBlockNum(m_nNumBlock);
					}
				}
				break;
			}
		}
		fclose(pFile);
		return true;
	}
	return false;
}

// ==========================================================
// アイテムのファイル読み込み
// ==========================================================
bool CGame::LoadItem(int nStageNo)
{
	FILE *pFile;
	CItem *pItem;
	char aTextBuff[128];

	CItem::ITEMTYPE m_type;

	pFile = 0;

	// ファイル読み込み
	if (nStageNo == 0)
	{
		pFile = fopen(ITEM_FILE, "r");
	}
	else if (nStageNo == 1)
	{
		pFile = fopen(ITEM_FILE, "r");
	}

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aTextBuff[0]);
			// ファイル範囲
			if (strcmp(&aTextBuff[0], "SCRIPT") == 0)
			{
				while (strcmp(&aTextBuff[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aTextBuff[0]);
					if (strcmp(&aTextBuff[0], "numItem") == 0)
					{
						fscanf(pFile, " = %d", &m_nNumItem);
					}
					// 敵ファイル読み込み
					if (strcmp(&aTextBuff[0], "ITEMSET") == 0)
					{
						while (strcmp(&aTextBuff[0], "END_ITEMSET") != 0)
						{
							fscanf(pFile, "%s", &aTextBuff[0]);
							if (strcmp(&aTextBuff[0], "pos") == 0)
							{// ポジション
								fscanf(pFile, " = %f,%f,%f", &m_pos.x, &m_pos.y, &m_pos.z);
							}
							if (strcmp(&aTextBuff[0], "nType") == 0)
							{// タイプ
								fscanf(pFile, " = %d", &m_type);
							}
						}
						// ブロック生成
						pItem = CItem::Create(m_type);
						pItem->SetPos(m_pos, ITEMSIZE_X, ITEMSIZE_Y, CENTERPOS_NORMAL);
						pItem->SetItemNum(m_nNumItem);
					}
				}
				break;
			}
		}
		fclose(pFile);
		return true;
	}
	return false;
}
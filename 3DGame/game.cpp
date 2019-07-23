//========================================================================================
//
// ゲーム本編[game.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "time.h"
#include "effect.h"
#include "pause.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "effect.h"
#include "gradation.h"
#include "flower.h"
#include "meshField.h"
#include "enemy.h"
#include "meshorbit.h"
#include "tutorial.h"
#include "model.h"
#include "telop.h"
#include "sound.h"
#include "hitpoint.h"
#include "EXP.h"

//========================================================================================
// マクロ定義
//========================================================================================

//========================================================================================
// プロトタイプ宣言
//========================================================================================

//========================================================================================
// グローバル変数
//========================================================================================
GAMESTATE g_gameState = GAMESTATE_NORMAL;				//ゲーム状態
int g_gameStateLog = GAMESTATE_NONE;
int g_nCounterGameState = 0;						//ゲーム状態管理カウンター

bool g_bUseUI = true;										//UIの表示するか

//========================================================================================
// 初期化処理
//========================================================================================
void InitGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// チュートリアルが終わっているかどうか
	if (IsFinish())
	{
		g_gameState = GAMESTATE_NORMAL;
	}
	else
	{
		g_gameState = GAMESTATE_TUTORIAL;
	}

	//チュートリアルの初期化処理
	InitTutorial();

	//テロップの初期化処理
	InitTelop();

	//エフェクト
	InitEffect();

	//ポーズ
	InitPause();

	//影の初期化処理
	InitShadow();

	// ポリゴンの初期化処理
	InitPolygon();

	// 軌跡の初期化
	InitOrbit();

	// 壁の初期化処理
	InitWall();

	//プレイヤーの初期化処理
	InitPlayer();

	// 敵の初期化処理
	InitEnemy();

	//フィールドの初期化処理
	InitMeshField();

	//エフェクトの初期化処理
	InitEffect();

	//グラデーションの初期化処理
	InitGradation();

	//flowerの初期化処理
	InitFlower();

	//hitpointの初期化処理
	InitHitPoint();

	//経験値の初期化処理
	InitEXP();

	PlaySound(SOUND_LABEL_BGM001, false);

	g_nCounterGameState = 0;
}

//========================================================================================
// 終了処理
//========================================================================================
void UninitGame(void)
{
	// チュートリアルの終了処理
	UninitTutorial();

	//テロップの終了処理
	UninitTelop();

	// 3Dポリゴンの終了処理
	UninitPolygon();

	// 壁の終了処理
	UninitWall();

	// モデルの終了処理
	UninitPlayer();

	// 敵の終了処理
	UninitEnemy();

	// 影の終了処理
	UninitShadow();

	//フィールドの終了処理
	UninitMeshField();

	// 軌跡の終了処理
	UninitOrbit();

	//エフェクトの終了処理
	UninitEffect();

	// グラデーションの終了処理
	UninitGradation();

	// flowerの終了処理
	UninitFlower();

	//hitpointの終了処理
	UninitHitPoint();

	//経験値の終了処理
	UninitEXP();

}

//========================================================================================
// 更新処理
//========================================================================================
void UpdateGame(void)
{
	if (g_gameState == GAMESTATE_PAUSE)
	{
		UpdatePause();

		if (g_gameState == GAMESTATE_BREAK)
		{
			SetFade(MODE_TITLE);
		}
	}
	else
	{
		if (g_gameState == GAMESTATE_TUTORIAL)
		{
			// チュートリアルの更新処理
			UpdateTutorial();
		}

		//テロップの更新処理
		UpdateTelop();

		// ポリゴンの更新処理
		UpdatePolygon();

		//フィールドの更新処理
		UpdateMeshField();

		//軌跡の描画処理
		UpdateOrbit();

		// 壁の更新処理
		UpdateWall();

		// モデルの更新処理
		UpdatePlayer();

		// 敵の更新処理
		UpdateEnemy();

		//エフェクトの更新処理
		UpdateEffect();

		// 影の更新処理
		UpdateShadow();

		// グラデーションの更新処理
		UpdateGradation();

		// Flowerの更新処理
		UpdateFlower();

		//hitpointの更新処理
		UpdateHitPoint();

		//経験値の湖心処理
		UpdateEXP();
	}

	//ポーズ切り替え
	if (GetTriggerKeyboard(DIK_P) && g_gameState == GAMESTATE_PAUSE || GetControllerTrigger(0, JOYPADKEY_B) && g_gameState == GAMESTATE_PAUSE)
	{
		SetPause(false);
		SetGameState(GetPauseMode());

	}
	else if (GetTriggerKeyboard(DIK_P) && g_gameState != GAMESTATE_PAUSE || GetControllerTrigger(0, JOYPADKEY_B) && g_gameState == GAMESTATE_PAUSE)
	{
		SetPause(true);
		SetGameState(GAMESTATE_PAUSE);
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;
			if (GetFade() == FADE_NONE)
			{
				//画面モードの設定
				SetFade(MODE_RESULT);
				//SetIsFinish(false);		//チュートリアル
			}
		}
		break;
	}

#ifdef _DEBUG
	if (GetTriggerKeyboard(DIK_F1))
	{
		if (g_bUseUI == true)
		{
			g_bUseUI = false;
		}
		else if (g_bUseUI == false)
		{
			g_bUseUI = true;
		}
	}
#endif // _DEBUG
}

//========================================================================================
// 描画処理
//========================================================================================
void DrawGame(void)
{
	PLAYER *pPlayer = GetPlayer();

	// 敵の描画処理
	DrawEnemy();

	// ポリゴンの描画処理
	DrawPolygon();

	// フィールドの描画処理
	DrawMeshField();

	// 壁の描画処理
	DrawWall();

	// プレイヤーの描画処理
	DrawPlayer();

	// 影の描画処理
	DrawShadow();

	//チュートリアルの描画処理
	DrawTutorial();

	//軌跡の描画処理
	DrawOrbit();

	//エフェクトの描画処理
	DrawEffect();

	//hitpointの描画処理
	DrawHitPoint();

	//経験値の描画処理
	DrawEXP();

	/*=======UI========*/
	if (g_bUseUI == true)
	{
		// ダメージの描画処理
		DrawGradation();

	}

	// メーター表示
	DrawFlower();

	//テロップの描画処理
	DrawTelop();

	//ポーズ
	DrawPause();
}

//========================================================================================
// ゲーム状態の設定
//========================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//========================================================================================
// ゲーム状態の設定
//========================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//========================================================================================
//
// ゲーム本編[game.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "game.h"
#include "magician.h"
#include "input.h"
#include "pointer.h"
#include "fade.h"
#include "time.h"
#include "effect.h"
#include "gamefade.h"
#include "pause.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "trajectory.h"
#include "gradation.h"
#include "meshField.h"
#include "meshSphere.h"
#include "enemy.h"
#include "meshorbit.h"
#include "tutorial.h"
#include "model.h"
#include "telop.h"
#include "sound.h"
#include "magic.h"
#include "hitpoint.h"
#include "time.h"

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

	// 魔法陣の初期化処理
	InitMagician();

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

	//ポインタの初期化処理
	InitPointer();

	//バレットの初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//フィールドの初期化処理
	InitMeshField();

	////フィールドの初期化処理
	//InitMeshSphere();

	//エフェクトの初期化処理
	InitEffect();

	//グラデーションの初期化処理
	InitGradation();

	//魔法メーターの初期化処理
	InitMagic();

	//体力の初期化処理
	InitHitPoint();

	//タイムの初期化処理
	InitTime();

	PlaySound(SOUND_LABEL_BGM000);

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

	//バレットの終了処理
	UninitBullet();

	//魔法陣の終了処理
	UninitMagician();

	//爆発の終了処理
	UninitExplosion();

	//フィールドの終了処理
	UninitMeshField();

	//フィールドの終了処理
	UninitMeshSphere();

	// ポインタの終了処理
	UninitPointer();

	// 軌跡の終了処理
	UninitOrbit();

	//エフェクトの終了処理
	UninitEffect();

	// グラデーションの終了処理
	UninitGradation();

	//魔法メーターの終了処理
	UninitMagic();

	//体力の終了処理
	UninitHitPoint();

	//タイムの終了処理
	UninitTime();

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

		// 魔法陣の更新処理
		UpdateMagician();

		// 敵の更新処理
		UpdateEnemy();
		
		// ポインタの更新処理
		UpdatePointer();

		//エフェクトの更新処理
		UpdateEffect();

		//バレットの更新処理
		UpdateBullet();

		//爆発の更新処理
		UpdateExplosion();

		// 影の更新処理
		UpdateShadow();

		// グラデーションの更新処理
		UpdateGradation();

		// 球体の更新処理
		UpdateMeshSphere();

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
		//魔法メーターの更新処理
		UpdateMagic();

		//体力の更新処理
		UpdateHitPoint();

		//タイムの更新処理
		UpdateTime();

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
	case GAMESTATE_STAGECHANGE:
		if (GetGameFade() == GAMEFADE_NONE)
		{
			SetGameFade();
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

	//バレットの描画処理
	DrawBullet();

	// ポリゴンの描画処理
	DrawPolygon();

	// フィールドの描画処理
	DrawMeshField();

	//フィールドの描画処理
	DrawMeshSphere();

	// 壁の描画処理
	DrawWall();

	// プレイヤーの描画処理
	DrawPlayer();

	//魔法陣の描画処理
	DrawMagician();

	// 影の描画処理
	DrawShadow();

	//チュートリアルの描画処理
	DrawTutorial();

	if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_1 || pPlayer->nAnimationType == MOTIONTYPE_ATTACK_2)
	{
	}

	//軌跡の描画処理
	DrawOrbit();

	//エフェクトの描画処理
	DrawEffect();

	//爆発の描画処理
	DrawExplosion();

	// ポインタの描画処理
	DrawPointer();

	/*=======UI========*/
	if (g_bUseUI == true)
	{
		//// ダメージの描画処理
		//DrawGradation();

	}

	//魔法メーターの描画処理
	DrawMagic();

	//体力の描画処理
	DrawHitPoint();

	//タイムの描画処理
	DrawTime();

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
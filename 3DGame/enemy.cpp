//=============================================================================
//
// モデル処理 [enemy.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "load.h"
#include "trajectory.h"
#include "model.h"
#include "player.h"
#include "meshorbit.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "telop.h"
#include "bullet.h"
#include "time.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_GRAVITY 2
#define MAX_ENEMY	1
#define MAX_LIFE 30
#define ENEMY_GRAVITY 0.5

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void AnimationEnemy(float fDifference);
bool CollisionExtinction(int nNum);
void AnimationSwitchEnemy(MOTIONENEMY nType);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;		//テクスチャへのポインタ
//float g_fVelocity = 30;
ENEMY g_aEnemy[MAX_ENEMY];										//プレイヤーの構造体
ENEMYPARTS g_aEnemyParts[30];						//partsの構造体
int g_nMaxEnemy = 15;								//パーツの最大数
D3DXVECTOR3				g_EffectPos;
D3DXVECTOR3				g_EffectPos1;
D3DXVECTOR3				g_EffectPos2;
D3DXVECTOR3				g_EffectPos3;
D3DXVECTOR3				g_EffectPos4;
D3DXVECTOR3				g_EffectPos5;
D3DXVECTOR3				g_EffectPos6;
D3DXVECTOR3				g_EffectPos7;
D3DXVECTOR3				g_EffectPos8;
int g_Number = 3;									//敵の人数
float g_fVelocityEnemy = 10;
int g_bAnimationEnemy = true;							//アニメーションのプレイ


//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_EffectPos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_EffectPos1 = D3DXVECTOR3(10.0f, 20.0f, 0.0f);
	g_EffectPos2 = D3DXVECTOR3(-10.0f, 20.0f, 0.0f);
	g_EffectPos3 = D3DXVECTOR3(-15.0f, 20.0f, 0.0f);
	g_EffectPos4 = D3DXVECTOR3(15.0f, 20.0f, 0.0f);
	g_EffectPos5 = D3DXVECTOR3(0.0f, 20.0f, 10.0f);
	g_EffectPos6 = D3DXVECTOR3(0.0f, 20.0f, 15.0f);
	g_EffectPos7 = D3DXVECTOR3(0.0f, 20.0f, -15.0f);
	g_EffectPos8 = D3DXVECTOR3(0.0f, 20.0f, -10.0f);
	for (int nCntModel = 0; nCntModel < MAX_ENEMY; nCntModel++)
	{
		// 位置・向きの初期設定
		for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
		{
			g_aEnemy[nCntModel].nLife = MAX_LIFE;
			g_aEnemy[nCntModel].aModel[nCntEnemy].posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].originPos - D3DXVECTOR3(0, 0, 0);
			g_aEnemy[nCntModel].aModel[nCntEnemy].originRot - D3DXVECTOR3(0, 0, 0);
			g_aEnemy[nCntModel].bJump = false;
			g_aEnemy[nCntModel].bUse = false;

			g_aEnemy[nCntModel].CurrentEnemyFrame = 0;								//現在のフレーム数
			g_aEnemy[nCntModel].CurrentEnemyKey = 0;								//現在のキー
			g_aEnemy[nCntModel].nAnimationEnemy = 0;								//現在のアニメーション
			g_aEnemy[nCntModel].nValueH = 0;										//コントローラー
			g_aEnemy[nCntModel].nValueV = 0;										//コントローラー
			g_aEnemy[nCntModel].nCntAttack = 0;
		}
	}

	//スクリプトの読み込み
	g_nMaxEnemy = LoadEnemy();

	for (int nCntEnemy = 0; nCntEnemy < g_nMaxEnemy; nCntEnemy++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(g_aEnemy[0].aModel[nCntEnemy].sAdd, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aEnemyParts[nCntEnemy].pBuffMatEnemy, NULL, &g_aEnemyParts[nCntEnemy].nNumMatEnemy, &g_aEnemyParts[nCntEnemy].pMeshEnemy);
	}

	// 位置・向きの初期設定
	for (int nCntModel = 0; nCntModel < MAX_ENEMY; nCntModel++)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
		{
			g_aEnemy[nCntModel].aModel[nCntEnemy].posEnemy = g_aEnemy[0].aModel[nCntEnemy].originPos;
			g_aEnemy[nCntModel].aModel[nCntEnemy].rotEnemy = g_aEnemy[0].aModel[nCntEnemy].originRot;
		}
		g_aEnemy[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (GetMode() == MODE_GAME && IsFinish())
	{
		SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 25);

		//SetEnemy(D3DXVECTOR3(00.0f, 0.0f, 100.0f), 25);

		//SetEnemy(D3DXVECTOR3(-100.0f, 0.0f, 100.0f), 25);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < g_nMaxEnemy; nCntEnemy++)
	{
		// メッシュの開放
		if (g_aEnemyParts[nCntEnemy].pMeshEnemy != NULL)
		{
			g_aEnemyParts[nCntEnemy].pMeshEnemy->Release();
			g_aEnemyParts[nCntEnemy].pMeshEnemy = NULL;
		}

		// マテリアルの開放
		if (g_aEnemyParts[nCntEnemy].pBuffMatEnemy != NULL)
		{
			g_aEnemyParts[nCntEnemy].pBuffMatEnemy->Release();
			g_aEnemyParts[nCntEnemy].pBuffMatEnemy = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	float fAngle = 0;
	float fAngle1 = 0;
	float fSpeed = 2.5;

	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();
	BULLET *pBullet = GetBullet();

	float fDifference_x;
	float fDifference_z;
	float fDifference;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (GetTriggerKeyboard(DIK_7))
		{
			g_aEnemy[nCntEnemy].nLife += 1;
		}
		if (GetTriggerKeyboard(DIK_U))
		{
			g_aEnemy[nCntEnemy].nLife += -1;
		}

		//敵が使われていた時
		if (g_aEnemy[nCntEnemy].bUse)
		{
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			fDifference_x = g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x;
			fDifference_z = g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z;
			fDifference = (float)sqrt(fDifference_x * fDifference_x + fDifference_z * fDifference_z);

			g_aEnemy[nCntEnemy].nCntAttack++;

			g_aEnemy[nCntEnemy].nWait += 1;

			AnimationEnemy(fDifference);

			//テロップがない時
			if (GetTelopState() == false)
			{
				if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_1 && g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_2)
				{
					//================コントローラー===================//
					GetJoypadStickLeft(1, &g_aEnemy[nCntEnemy].nValueH, &g_aEnemy[nCntEnemy].nValueV);

					//角度
					g_aEnemy[nCntEnemy].dest.y = atan2f(-g_aEnemy[nCntEnemy].move.x, -g_aEnemy[nCntEnemy].move.z);

					//移動
					g_aEnemy[nCntEnemy].move -= D3DXVECTOR3(sinf(D3DX_PI * 1.0f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueV), 0, cosf(D3DX_PI * 1.0f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueV));
					g_aEnemy[nCntEnemy].move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueH), 0, cosf(D3DX_PI * 0.5f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueH));

					if (fabs(g_aEnemy[nCntEnemy].move.x) > 1 || fabs(g_aEnemy[nCntEnemy].move.z) > 1)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_RUN)
						{
							AnimationSwitchEnemy(MOTIONENEMY_RUN);
						}
					}

					//左移動
					if (GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].dest.y = D3DX_PI * 0.5f + pCamera->rot.y;
						pCamera->nCount = 0;
					}
					//右移動
					if (GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].dest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
						pCamera->nCount = 0;
					}
					//攻撃
					if (GetTriggerKeyboard(DIK_LSHIFT) || GetControllerTrigger(1, JOYPADKEY_X))
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_1 && g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_2)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_1;
							g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
							g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
						}
					}
				}

				//ジャンプ
				if (GetTriggerKeyboard(DIK_W) || GetControllerPress(1, JOYPADKEY_A))
				{
					if (g_aEnemy[nCntEnemy].bJump == false)
					{
						g_aEnemy[nCntEnemy].bJump = true;
						g_aEnemy[nCntEnemy].move.y += g_fVelocityEnemy;
					}
				}
			}

			//座標の移動
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//減少
			g_aEnemy[nCntEnemy].move.x += (0 - g_aEnemy[nCntEnemy].move.x) * 0.2f;
			g_aEnemy[nCntEnemy].move.z += (0 - g_aEnemy[nCntEnemy].move.z) * 0.2f;

			//キャラクターの振り向き
			g_aEnemy[nCntEnemy].Difference.y = g_aEnemy[nCntEnemy].rot.y - g_aEnemy[nCntEnemy].dest.y;

			if (g_aEnemy[nCntEnemy].Difference.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].Difference.y -= D3DX_PI * 2;
			}
			else if (g_aEnemy[nCntEnemy].Difference.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].Difference.y += D3DX_PI * 2;
			}

			g_aEnemy[nCntEnemy].rot.y -= g_aEnemy[nCntEnemy].Difference.y * 0.1f;

			if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}
			else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}

			if (GetTriggerKeyboard(DIK_F6))
			{
				if (g_bAnimationEnemy)
				{
					g_bAnimationEnemy = false;
				}
				else if (!g_bAnimationEnemy)
				{
					g_bAnimationEnemy = true;
				}
			}

			//重力
			g_aEnemy[nCntEnemy].move.y -= ENEMY_GRAVITY;

			//モデルの上にいる時
			if (CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move) == true)
			{
				g_aEnemy[nCntEnemy].bJump = false;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
			}
			else
			{
				g_aEnemy[nCntEnemy].bJump = true;
			}

			//プレイヤーが床に着いた時
			if (g_aEnemy[nCntEnemy].pos.y <= 0)
			{
				g_aEnemy[nCntEnemy].pos.y = 0.0f;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
				g_aEnemy[nCntEnemy].bJump = false;
			}

			//敵の当たり判定
			if (pBullet->pos.x < g_aEnemy[nCntEnemy].pos.x - 10.0f &&
				pBullet->pos.x > g_aEnemy[nCntEnemy].pos.x + 10.0f &&
				pBullet->pos.z < g_aEnemy[nCntEnemy].pos.z + 40.0f &&
				pBullet->pos.z > g_aEnemy[nCntEnemy].pos.z)
			{
				g_aEnemy[nCntEnemy].nLife -= 5;

				if (g_aEnemy[nCntEnemy].nLife <= 0)
				{
					g_aEnemy[nCntEnemy].bUse = false;
					DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadow);
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize1 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize1 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize = rand() % 10 + 10;
						float fSize2 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize1 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize = rand() % 10 + 10;
						float fSize3 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize = rand() % 10 + 10;
						float fSize4 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}

				}
			}
			//モデルの当たり判定
			CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move);

			//当たり判定
			//if (SphereModel(COLLISIONTYPE_WEAPON, &pBullet->pos, &g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].move, &pPlayer->move, 10, 10))
			//{
			//	g_aEnemy[nCntEnemy].nLife -= 1;
			//}

			//敵が攻撃を受けているかどうか
			if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_DAMAGE)
			{
				//プレイヤーの攻撃方法が通常攻撃だった時
				if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_1)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 5, 5))
					{
						g_aEnemy[nCntEnemy].nLife -= 3;
						g_aEnemy[nCntEnemy].nCntAttack = 0;
						g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_DAMAGE;
					}
				}

				//プレイヤーの攻撃方法が通常攻撃だった時
				if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_2)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 5, 5))
					{
						g_aEnemy[nCntEnemy].nLife -= 5;
						g_aEnemy[nCntEnemy].nCntAttack = 0;
						g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_DAMAGE;
					}
				}

				//敵のHPが0になった時
				if (g_aEnemy[nCntEnemy].nLife <= 0)
				{
					g_aEnemy[nCntEnemy].bUse = false;
					DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadow);

					if (CollisionExtinction(-1))
					{
						//SetModel(ITEMTYPE_CLOUD_3, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);
					}
				}
			}

			//プレイヤーがダメージを受けているかどうか
			if (pPlayer->nAnimationType != MOTIONTYPE_DAMAGE)
			{
				//敵の攻撃が攻撃だった時
				if (g_aEnemy[nCntEnemy].nAnimationEnemy == MOTIONENEMY_ATTACK_1)
				{
					//当たり判定
					if (SphereModel(COLLISIONTYPE_WEAPON, &g_aEnemy[nCntEnemy].pos, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &pPlayer->move, &g_aEnemy[nCntEnemy].move, 50, 50))
					{
						pPlayer->nLife -= 3;
						pPlayer->nAnimationType = MOTIONTYPE_DAMAGE;

						//プレイヤーの体力が0になった時
						if (pPlayer->nLife <= 0)
						{
							SetGameState(GAMESTATE_END);
							SetResultState(RESULTSTATE_FAILD);
						}
					}
				}
			}

			//影の位置を設定
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);
		}
	}

	//当たり判定
	if (SphereModel(COLLISIONTYPE_WEAPON, &pBullet->pos, &pPlayer->pos, &pPlayer->move, &pPlayer->move, 10, 10))
	{
		//pPlayer->nLife -= 1;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用
	D3DMATERIAL9	matDef;							//マテリアルデータへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);			// ワールドマトリックスの初期化

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			for (int nCntModel = 0; nCntModel < g_nMaxEnemy; nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy);

				// スケールを反映


				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aModel[nCntModel].rotEnemy.y, g_aEnemy[nCntEnemy].aModel[nCntModel].rotEnemy.x, g_aEnemy[nCntEnemy].aModel[nCntModel].rotEnemy.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aModel[nCntModel].posEnemy.x, g_aEnemy[nCntEnemy].aModel[nCntModel].posEnemy.y, g_aEnemy[nCntEnemy].aModel[nCntModel].posEnemy.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);					//すべてのデータ収録

																						//親子関係が成立している場合
				if (g_aEnemy[nCntEnemy].aModel[nCntModel].nParent == -1)
				{
					//マトリックスの合成
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].mtxWorld);
				}
				else
				{
					//マトリックスの合成
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntModel].nParent].mtxWorldEnemy);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy);					//すべてのデータ収録

																													// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_aEnemyParts[nCntModel].pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aEnemyParts[nCntModel].nNumMatEnemy; nCntMat++)
				{
					//色
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// 描画
					g_aEnemyParts[nCntModel].pMeshEnemy->DrawSubset(nCntMat);
				}
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// プレイヤーアニメーション
//=============================================================================
void AnimationEnemy(float fDifference)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			if (GetTriggerKeyboard(DIK_5))
			{
				g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_1;
				g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
				g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
			}

			//モーションの最大値処理
			if (g_aEnemy[nCntEnemy].nAnimationEnemy < 0)
			{
				g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_MAX;
			}
			else if (g_aEnemy[nCntEnemy].nAnimationEnemy > MOTIONENEMY_MAX)
			{
				g_aEnemy[nCntEnemy].nAnimationEnemy = 0;
			}

			//最初のフレームの時
			if (g_aEnemy[nCntEnemy].CurrentEnemyFrame == 0)
			{
				for (int nCntMotion = 0; nCntMotion < g_nMaxEnemy; nCntMotion++)
				{

					//posの計算((目標のkey + プリセットの配置) - 現在のキー)
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos.x = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].pos.x + g_aEnemy[nCntEnemy].aModel[nCntMotion].originPos.x) - g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy.x) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos.y = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].pos.y + g_aEnemy[nCntEnemy].aModel[nCntMotion].originPos.y) - g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy.y) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos.z = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].pos.z + g_aEnemy[nCntEnemy].aModel[nCntMotion].originPos.z) - g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy.z) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;

					//rotの計算((目標のkey + プリセットの配置) - 現在のキー)
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot.x = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].rot.x + g_aEnemy[nCntEnemy].aModel[nCntMotion].originRot.x) - g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy.x) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot.y = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].rot.y + g_aEnemy[nCntEnemy].aModel[nCntMotion].originRot.y) - g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy.y) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot.z = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].rot.z + g_aEnemy[nCntEnemy].aModel[nCntMotion].originRot.z) - g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy.z) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
				}
			}

			//それ以外のフレーム
			if (g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].nMaxKey > g_aEnemy[nCntEnemy].CurrentEnemyKey)
			{
				for (int nCntMotion = 0; nCntMotion < g_nMaxEnemy; nCntMotion++)
				{
					//rotの移動
					g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy += g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot;

					//posの移動
					g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy += g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos;
				}
			}

			//フレームの最大数に満たない場合
			if (g_aEnemy[nCntEnemy].CurrentEnemyFrame < g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame)
			{
				g_aEnemy[nCntEnemy].CurrentEnemyFrame++;
			}

			//フレーム数の最大値に達した場合
			if (g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame <= g_aEnemy[nCntEnemy].CurrentEnemyFrame)
			{
				g_aEnemy[nCntEnemy].CurrentEnemyKey++;
				g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
			}

			//キー数の最大値に達した場合
			if (g_aEnemy[nCntEnemy].CurrentEnemyKey == g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].nMaxKey)
			{
				//アニメーションがアタックモーションだった時
				if (g_aEnemy[nCntEnemy].nAnimationEnemy == MOTIONENEMY_ATTACK_1)
				{
					if (fDifference < 150)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_2)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_2;
							g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
							g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
							g_aEnemy[nCntEnemy].nWait = 0;
						}
					}
				}

				//ループするかどうか
				else if (!g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].nLoop)
				{
					g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_NEUTRAL;
					g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
				}

				//キーのリセット
				g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
			}
		}
	}
}

//=============================================================================
// プレイヤー情報の取得
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nLife)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, D3DXVECTOR3(30.0f, 10.0f, 30.0f));
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
}

//=============================================================================
// プレイヤー情報の取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// プレイヤー情報の取得
//=============================================================================
bool CollisionExtinction(int nNum)
{
	bool bDispersion = false;
	g_Number += nNum;

	if (g_Number <= 0)
	{
		SetStageTelop(TELOPTYPE_MESSAGEWINDOW, TELOPPAGE_END);
		bDispersion = true;
	}
	else
	{
		if (g_Number == 2)
		{
			SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 25);
		}
		else if (g_Number == 1)
		{
			SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 25);
		}
	}

	return bDispersion;
}

//=============================================================================
// アニメーションの切り替え
//=============================================================================
void AnimationSwitchEnemy(MOTIONENEMY nType)
{
	g_aEnemy[0].nAnimationEnemy = nType;
	g_aEnemy[0].CurrentEnemyKey = 0;
	g_aEnemy[0].CurrentEnemyFrame = 0;

	//switch (nType)
	//{
	//case MOTIONTYPE_NEUTRAL:
	//	g_nAnimationType = MOTIONTYPE_NEUTRAL;
	//	g_CurrentKey = 0;
	//	g_CurrentFrame = 0;
	//	break;
	//case MOTIONTYPE_ATTACK:
	//	break;
	//case MOTIONTYPE_RUN:
	//	break;
	//}
}

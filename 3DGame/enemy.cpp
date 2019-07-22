//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "load.h"
#include "model.h"
#include "player.h"
#include "meshorbit.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "telop.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_GRAVITY 2
#define MAX_ENEMY	5

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void AnimationEnemy(float fDifference);
bool CollisionExtinction(int nNum);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;		//テクスチャへのポインタ
//float g_fVelocity = 30;
ENEMY g_aEnemy[MAX_ENEMY];										//プレイヤーの構造体
ENEMYPARTS g_aEnemyParts[30];						//partsの構造体
int g_nMaxEnemy = 15;								//パーツの最大数

int g_nNumber;									//敵の人数

//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModel = 0; nCntModel < MAX_ENEMY; nCntModel++)
	{
		// 位置・向きの初期設定
		for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
		{
			g_aEnemy[nCntModel].nLife = 0;
			g_aEnemy[nCntModel].aModel[nCntEnemy].posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].bJump = false;
			g_aEnemy[nCntModel].bUse = false;

			g_aEnemy[nCntModel].CurrentEnemyFrame = 0;								//現在のフレーム数
			g_aEnemy[nCntModel].CurrentEnemyKey = 0;								//現在のキー
			g_aEnemy[nCntModel].nAnimationEnemy = 0;							//現在のアニメーション
			g_aEnemy[nCntModel].nCntAttack = 0;
		}
	}

	g_nNumber = 3;

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
		SetEnemy(D3DXVECTOR3(100.0f, 0.0f, 100.0f), 25);

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
	PLAYER *pPlayer = GetPlayer();

	float fDifference_x;
	float fDifference_z;
	float fDifference;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
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

			//敵が攻撃中かどうか
			if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_1)
			{
				//敵が範囲内に入ってきたかどうか
				if (fDifference < 200.0f && fDifference > 50)
				{
					if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 1.0f)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONTYPE_RUN)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = (MOTIONTYPE_RUN);
						}

						g_aEnemy[nCntEnemy].pos.x = g_aEnemy[nCntEnemy].pos.x + 1.0f;
						g_aEnemy[nCntEnemy].rot.y = atan2f(fDifference_x, fDifference_z);
					}
					else if (pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 1.0f)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONTYPE_RUN)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = (MOTIONTYPE_RUN);
						}
						g_aEnemy[nCntEnemy].pos.x = g_aEnemy[nCntEnemy].pos.x - 1.0f;
						g_aEnemy[nCntEnemy].rot.y = atan2f(fDifference_x, fDifference_z);
					}
					if (pPlayer->pos.z <= g_aEnemy[nCntEnemy].pos.z + 1.0f)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONTYPE_RUN)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = (MOTIONTYPE_RUN);
						}
						g_aEnemy[nCntEnemy].pos.z = g_aEnemy[nCntEnemy].pos.z - 1.0f;
						g_aEnemy[nCntEnemy].rot.y = atan2f(fDifference_x, fDifference_z);
					}
					else if (pPlayer->pos.z >= g_aEnemy[nCntEnemy].pos.z - 1.0f)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONTYPE_RUN)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = (MOTIONTYPE_RUN);
						}
						g_aEnemy[nCntEnemy].pos.z = g_aEnemy[nCntEnemy].pos.z + 1.0f;
						g_aEnemy[nCntEnemy].rot.y = atan2f(fDifference_x, fDifference_z);
					}
				}

				//クールタイムを終えているかどうか
				if (g_aEnemy[nCntEnemy].nWait >= 100)
				{
					if (fDifference < 50)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_1)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_1;
							g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
							g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
							g_aEnemy[nCntEnemy].nWait = 0;
						}
					}
				}
			}

			//モデルの当たり判定
			CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move);

			//当たり判定
			SphereModel(COLLISIONTYPE_NORMAL , &g_aEnemy[nCntEnemy].pos, &pPlayer->pos, &g_aEnemy[nCntEnemy].move, &pPlayer->move, 20, 20);
			
			//敵が攻撃を受けているかどうか
			if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_DAMAGE)
			{
				//プレイヤーの攻撃方法が飛び込み攻撃だった時
				if (pPlayer->nAnimationType == MOTIONTYPE_RUNATTACK)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 20, 20))
					{
						g_aEnemy[nCntEnemy].nLife -= 3;
						g_aEnemy[nCntEnemy].nCntAttack = 0;
						g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_DAMAGE;

					}
				}
				
				//プレイヤーの攻撃方法が通常攻撃だった時
				if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_1)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 20, 20))
					{
						g_aEnemy[nCntEnemy].nLife -= 3;
						g_aEnemy[nCntEnemy].nCntAttack = 0;
						g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_DAMAGE;
					}
				}
				
				//プレイヤーの攻撃方法が通常攻撃だった時
				if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_2)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 20, 20))
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
						SetModel(ITEMTYPE_FLOWER, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);
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
					if (fDifference < 50)
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
	g_nNumber += nNum;

	if (g_nNumber <= 0)
	{
		SetStageTelop(TELOPTYPE_MESSAGEWINDOW, TELOPPAGE_END);
		bDispersion = true;
	}
	else
	{
		if (g_nNumber == 2)
		{
			SetEnemy(D3DXVECTOR3(100.0f, 0.0f, 100.0f), 25);
		}
		else if (g_nNumber == 1)
		{
			SetEnemy(D3DXVECTOR3(-100.0f, 0.0f, 100.0f), 25);
		}
	}

	return bDispersion;
}
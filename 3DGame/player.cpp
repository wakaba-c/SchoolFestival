//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "load.h"
#include "model.h"
#include "controller.h"
#include "tutorial.h"
#include "sound.h"
#include "telop.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_GRAVITY	2
#define CHANGE_MOTIONFRAME	5

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void AnimationPlayer(void);
void AnimationSwitch(MOTIONTYPE nType);
float g_fVelocity = 20;

#ifdef _DEBUG
void DrawPlayerData(void);
#endif

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;		//テクスチャへのポインタ
PLAYER g_player;									//プレイヤーの構造体
int g_nMaxPlayer = 15;								//パーツの最大数
int g_bAnimationPlayer = true;						//アニメーションのプレイ

int g_nCntAttacCombo = 0;							//アタックモーションの派生

#ifdef _DEBUG
LPD3DXFONT			g_pPlayerFont = NULL;			// フォントへのポインタ
#endif

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	g_player.bJump = false;
	g_player.CurrentFrame = 0;									//現在のフレーム数
	g_player.CurrentKey = 1;									//現在のキー
	g_player.nAnimationType = 0;								//現在のアニメーション
	g_player.nValueH = 0;									//コントローラー
	g_player.nValueV = 0;									//コントローラー

	// 位置・向きの初期設定
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_player.aModel[nCntPlayer].posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[nCntPlayer].movePlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[nCntPlayer].rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	D3DXCreateFont(pDevice, 13, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &g_pPlayerFont);
#endif

	//スクリプトの読み込み
	g_nMaxPlayer = LoadModel();

	//LoadMaya();

	for (int nCntPlayer = 0; nCntPlayer < g_nMaxPlayer; nCntPlayer++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(g_player.aModel[nCntPlayer].sAdd, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[nCntPlayer].pBuffMatPlayer, NULL, &g_player.aModel[nCntPlayer].nNumMatPlayer, &g_player.aModel[nCntPlayer].pMeshPlayer);
	}

	//// 位置・向きの初期設定
	//for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	//{
	//	g_player.aModel[nCntPlayer].posPlayer = g_player.aModel[nCntPlayer].originPos + g_player.aModel[nCntPlayer].aMotion[g_nAnimationType].aKey[0].pos;
	//	g_player.aModel[nCntPlayer].rotPlayer = g_player.aModel[nCntPlayer].originRot + g_player.aModel[nCntPlayer].aMotion[g_nAnimationType].aKey[0].rot;
	//}

	// 位置・向きの初期設定
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_player.aModel[nCntPlayer].posPlayer = g_player.aModel[nCntPlayer].originPos;
		g_player.aModel[nCntPlayer].rotPlayer = g_player.aModel[nCntPlayer].originRot;
	}

	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);

	if (IsFinish())
	{
		g_player.fLife = PLAYER_LIFE;
	}
	else
	{
		g_player.fLife = 999;
	}
	
	//影の作成
	g_player.nIdxShadow = SetShadow(D3DXVECTOR3(1.0f,0.0f,10.0f), g_player.rot, D3DXVECTOR3(30.0f, 10.0f, 30.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
#ifdef _DEBUG
	if (g_pPlayerFont != NULL)
	{// デバッグ表示用フォントの開放
		g_pPlayerFont->Release();
		g_pPlayerFont = NULL;
	}
#endif
	for (int nCntPlayer = 0; nCntPlayer < g_nMaxPlayer; nCntPlayer++)
	{
		// メッシュの開放
		if (g_player.aModel[nCntPlayer].pMeshPlayer != NULL)
		{
			g_player.aModel[nCntPlayer].pMeshPlayer->Release();
			g_player.aModel[nCntPlayer].pMeshPlayer = NULL;
		}

		// マテリアルの開放
		if (g_player.aModel[nCntPlayer].pBuffMatPlayer != NULL)
		{
			g_player.aModel[nCntPlayer].pBuffMatPlayer->Release();
			g_player.aModel[nCntPlayer].pBuffMatPlayer = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *pCamera = GetCamera();

	// リスナーの更新
	UpdateListener(g_player.pos, g_player.move, g_player.rot, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	if (GetTriggerKeyboard(DIK_8))
	{
		g_player.fLife += 1;
	}
	if (GetTriggerKeyboard(DIK_I))
	{
		g_player.fLife += -1;
	}

	g_player.posOld = g_player.pos;


	if (GetTelopState() == false)
	{
		if (g_player.nAnimationType != MOTIONTYPE_RUNATTACK && g_player.nAnimationType != MOTIONTYPE_ATTACK_1 && g_player.nAnimationType != MOTIONTYPE_ATTACK_2)
		{

			if (fabs(g_player.move.x) > 2 || fabs(g_player.move.z) > 2)
			{
				if (g_player.nAnimationType != MOTIONTYPE_RUN)
				{
					AnimationSwitch(MOTIONTYPE_RUN);
				}
			}

			//================コントローラー===================//
			GetJoypadStickLeft(0, &g_player.nValueH, &g_player.nValueV);

			//角度
			g_player.dest.y = atan2f(-g_player.move.x, -g_player.move.z);

			//移動
			g_player.move -= D3DXVECTOR3(sinf(D3DX_PI * 1.0f + pCamera->rot.y) * (1.5f * g_player.nValueV), 0, cosf(D3DX_PI * 1.0f + pCamera->rot.y) * (1.5f * g_player.nValueV));
			g_player.move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f + pCamera->rot.y) * (1.5f * g_player.nValueH), 0, cosf(D3DX_PI * 0.5f + pCamera->rot.y) * (1.5f * g_player.nValueH));

			//左右操作
			if (GetKeyboardPress(DIK_LEFTARROW))
			{
				//if (g_player.nAnimationType != MOTIONTYPE_RUN)
				//{
				//	AnimationSwitch(MOTIONTYPE_RUN);
				//}

				if (GetKeyboardPress(DIK_UPARROW))
				{
					g_player.move.x += sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * 1.0f;
					g_player.move.z += cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * 1.0f;
					g_player.dest.y = D3DX_PI * 0.75f + pCamera->rot.y;
					pCamera->nCount = 0;
				}
				else if (GetKeyboardPress(DIK_DOWNARROW))
				{
					g_player.move.x += sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * 1.0f;
					g_player.move.z += cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * 1.0f;
					g_player.dest.y = D3DX_PI * 0.25f + pCamera->rot.y;
					pCamera->nCount = 0;
				}
				else
				{
					g_player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
					g_player.move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
					g_player.dest.y = D3DX_PI * 0.5f + pCamera->rot.y;
					pCamera->nCount = 0;
				}
			}

			else if (GetKeyboardPress(DIK_RIGHTARROW))
			{
				//if (g_player.nAnimationType != MOTIONTYPE_RUN)
				//{
				//	AnimationSwitch(MOTIONTYPE_RUN);
				//}

				//上下操作
				if (GetKeyboardPress(DIK_UPARROW))
				{
					g_player.move.x += sinf(D3DX_PI * 0.25f + pCamera->rot.y) * 1.0f;
					g_player.move.z += cosf(D3DX_PI * 0.25f + pCamera->rot.y) * 1.0f;
					g_player.dest.y = -D3DX_PI * 0.75f + pCamera->rot.y;
					pCamera->nCount = 0;
				}
				else if (GetKeyboardPress(DIK_DOWNARROW))
				{
					g_player.move.x += sinf(D3DX_PI * 0.75f + pCamera->rot.y) * 1.0f;
					g_player.move.z += cosf(D3DX_PI * 0.75f + pCamera->rot.y) * 1.0f;
					g_player.dest.y = -D3DX_PI * 0.25f + pCamera->rot.y;
					pCamera->nCount = 0;
				}
				else
				{
					g_player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
					g_player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
					g_player.dest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
					pCamera->nCount = 0;
				}
			}

			//上下操作
			else if (GetKeyboardPress(DIK_UPARROW))
			{
				//if (g_player.nAnimationType != MOTIONTYPE_RUN)
				//{
				//	AnimationSwitch(MOTIONTYPE_RUN);
				//}

				g_player.move.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * 1.0f;
				g_player.move.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * 1.0f;
				g_player.dest.y = D3DX_PI * 1.0f + pCamera->rot.y;
				pCamera->nCount = 0;
			}
			else if (GetKeyboardPress(DIK_DOWNARROW))
			{
				//if (g_player.nAnimationType != MOTIONTYPE_RUN)
				//{
				//	AnimationSwitch(MOTIONTYPE_RUN);
				//}

				g_player.move.x += sinf(D3DX_PI * 1.0f + pCamera->rot.y) * 1.0f;
				g_player.move.z += cosf(D3DX_PI * 1.0f + pCamera->rot.y) * 1.0f;
				g_player.dest.y = D3DX_PI * 0.0f + pCamera->rot.y;
				pCamera->nCount = 0;
			}

			if (GetTriggerKeyboard(DIK_C) || GetControllerTrigger(0, JOYPADKEY_RIGHT_SHOULDER))
			{
				g_player.nAnimationType = MOTIONTYPE_RUNATTACK;
				g_player.CurrentKey = 0;
				g_player.CurrentFrame = 0;

				g_player.move.x -= sinf(g_player.rot.y) * 25.0f;
				g_player.move.z -= cosf(g_player.rot.y) * 25.0f;
				pCamera->nCount = 0;
			}
			if (GetTriggerKeyboard(DIK_X) || GetControllerTrigger(0, JOYPADKEY_X))
			{
				PlaySound(SOUND_LABEL_SE_SLASH, false);
				//g_player.nAnimationType = MOTIONTYPE_ATTACK_1;
				//g_player.CurrentKey = 0;
				//g_player.CurrentFrame = 0;

				AnimationSwitch(MOTIONTYPE_ATTACK_1);
			}
		}

		if (GetTriggerKeyboard(DIK_SPACE) || GetControllerPress(0, JOYPADKEY_A))
		{
			if (g_player.bJump == false)
			{
				PlaySound(SOUND_LABEL_SE_JUMP, false);
				g_player.bJump = true;
				g_player.move.y += g_fVelocity;
			}
		}
	}

	if (g_bAnimationPlayer)
	{
		AnimationPlayer();
	}

	//座標の移動
	g_player.pos += g_player.move;

	g_player.move.x += (0 - g_player.move.x) * 0.2f;
	g_player.move.z += (0 - g_player.move.z) * 0.2f;

	//キャラクターの振り向き
	g_player.Difference.y = g_player.rot.y - g_player.dest.y;

	if (g_player.Difference.y > D3DX_PI)
	{
		g_player.Difference.y -= D3DX_PI * 2;
	}
	else if (g_player.Difference.y < -D3DX_PI)
	{
		g_player.Difference.y += D3DX_PI * 2;
	}

	g_player.rot.y -= g_player.Difference.y * 0.1f;

	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2;
	}


	if (g_player.pos.z < -500)
	{
		g_player.pos.z = -500;
		g_player.move.z = 0;
	}
	else if (g_player.pos.z > 500)
	{
		g_player.pos.z = 500;
		g_player.move.z = 0;
	}
	if (g_player.pos.x < -500)
	{
		g_player.pos.x = -500;
		g_player.move.x = 0;
	}
	else if (g_player.pos.x > 500)
	{
		g_player.pos.x = 500;
		g_player.move.x = 0;
	}

	if (GetTriggerKeyboard(DIK_F5))
	{
		if (g_bAnimationPlayer)
		{
			g_bAnimationPlayer = false;
		}
		else if (!g_bAnimationPlayer)
		{
			g_bAnimationPlayer = true;
		}
	}

	//重力
	g_player.move.y -= PLAYER_GRAVITY;

	//モデルの上にいる時
	if (CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move) == true)
	{
		g_player.bJump = false;
		g_player.move.y = 0.0f;
	}
	else
	{
		g_player.bJump = true;
	}

	//プレイヤーが床に着いた時
	if (g_player.pos.y <= 0)
	{
		g_player.pos.y = 0.0f;
		g_player.move.y = 0.0f;
		g_player.bJump = false;
	}

	//影の位置を設定
	SetPositionShadow(g_player.nIdxShadow, g_player.pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用
	D3DMATERIAL9	matDef;							//マテリアルデータへのポインタ
	

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	for (int nCntPlayer = 0; nCntPlayer < g_nMaxPlayer; nCntPlayer++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntPlayer].mtxWorldPlayer);

		// スケールを反映


		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.aModel[nCntPlayer].rotPlayer.y, g_player.aModel[nCntPlayer].rotPlayer.x, g_player.aModel[nCntPlayer].rotPlayer.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntPlayer].mtxWorldPlayer, &g_player.aModel[nCntPlayer].mtxWorldPlayer, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.aModel[nCntPlayer].posPlayer.x,g_player.aModel[nCntPlayer].posPlayer.y, g_player.aModel[nCntPlayer].posPlayer.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntPlayer].mtxWorldPlayer, &g_player.aModel[nCntPlayer].mtxWorldPlayer, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);					//すべてのデータ収録

		//親子関係が成立している場合
		if (g_player.aModel[nCntPlayer].nParent == -1)
		{
			//マトリックスの合成
			D3DXMatrixMultiply(&g_player.aModel[nCntPlayer].mtxWorldPlayer, &g_player.aModel[nCntPlayer].mtxWorldPlayer, &g_player.mtxWorld);
		}
		else
		{
			//マトリックスの合成
			D3DXMatrixMultiply(&g_player.aModel[nCntPlayer].mtxWorldPlayer, &g_player.aModel[nCntPlayer].mtxWorldPlayer, &g_player.aModel[g_player.aModel[nCntPlayer].nParent].mtxWorldPlayer);
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntPlayer].mtxWorldPlayer);					//すべてのデータ収録

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntPlayer].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntPlayer].nNumMatPlayer; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// 描画
			g_player.aModel[nCntPlayer].pMeshPlayer->DrawSubset(nCntMat);
		}
	}
#ifdef _DEBUG
	// FPS表示
	DrawPlayerData();
#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// プレイヤーアニメーション
//=============================================================================
void AnimationPlayer(void)
{	
	//モーションの最大値処理
	if (g_player.nAnimationType < 0)
	{
		g_player.nAnimationType = MOTIONTYPE_MAX;
	}
	else if (g_player.nAnimationType > MOTIONTYPE_MAX)
	{
		g_player.nAnimationType = 0;
	}

	if (GetTriggerKeyboard(DIK_3))
	{
		g_player.nAnimationType = MOTIONTYPE_DAMAGE;
	}

	//最初のフレームの時
	if (g_player.CurrentFrame == 0)
	{
		for (int nCntMotion = 0; nCntMotion < g_nMaxPlayer; nCntMotion++)
		{
			//posの計算((目標のkey + プリセットの配置) - 現在のキー)
			g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentPos.x = ((g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].pos.x + g_player.aModel[nCntMotion].originPos.x) - g_player.aModel[nCntMotion].posPlayer.x) / g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame;
			g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentPos.y = ((g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].pos.y + g_player.aModel[nCntMotion].originPos.y) - g_player.aModel[nCntMotion].posPlayer.y) / g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame;
			g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentPos.z = ((g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].pos.z + g_player.aModel[nCntMotion].originPos.z) - g_player.aModel[nCntMotion].posPlayer.z) / g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame;

			//rotの計算((目標のkey + プリセットの配置) - 現在のキー)
			g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentRot.x = ((g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].rot.x + g_player.aModel[nCntMotion].originRot.x) - g_player.aModel[nCntMotion].rotPlayer.x) / g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame;
			g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentRot.y = ((g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].rot.y + g_player.aModel[nCntMotion].originRot.y) - g_player.aModel[nCntMotion].rotPlayer.y) / g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame;
			g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentRot.z = ((g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].rot.z + g_player.aModel[nCntMotion].originRot.z) - g_player.aModel[nCntMotion].rotPlayer.z) / g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame;
		}
	}

	//それ以外のフレーム
	if (g_player.aModel[0].aMotion[g_player.nAnimationType].nMaxKey > g_player.CurrentKey)
	{
		for (int nCntMotion = 0; nCntMotion < g_nMaxPlayer; nCntMotion++)
		{
			//rotの移動
			g_player.aModel[nCntMotion].rotPlayer += g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentRot;

			//posの移動
			g_player.aModel[nCntMotion].posPlayer += g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].CurrentPos;
		}
	}

	//フレームの最大数に満たない場合
	if (g_player.CurrentFrame < g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame)
	{
		g_player.CurrentFrame++;
	}

	//フレーム数の最大値に達した場合
	if (g_player.aModel[0].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].nFrame <= g_player.CurrentFrame)
	{
		g_player.CurrentKey++;
		g_player.CurrentFrame = 0;
	}

	//キー数の最大値に達した場合
	if (g_player.CurrentKey >= g_player.aModel[0].aMotion[g_player.nAnimationType].nMaxKey)
	{
		//攻撃の派生
		if (g_player.nAnimationType == MOTIONTYPE_ATTACK_1)
		{
			g_nCntAttacCombo++;

			if (GetTriggerKeyboard(DIK_X) || GetControllerTrigger(0, JOYPADKEY_X))
			{
				PlaySound(SOUND_LABEL_SE_SLASH, false);
				g_player.nAnimationType = MOTIONTYPE_ATTACK_2;
				g_player.CurrentKey = 0;
				g_player.CurrentFrame = 0;
				g_nCntAttacCombo = 0;
			}
			else if (g_nCntAttacCombo > 15)
			{
				g_player.nAnimationType = MOTIONTYPE_NEUTRAL;
				g_player.CurrentFrame = 0;
				g_nCntAttacCombo = 0;

				//キーのリセット
				g_player.CurrentKey = 0;
			}
		}
		else
		{
			//ループするかどうか
			if (g_player.aModel[0].aMotion[g_player.nAnimationType].nLoop)
			{
				//キーのリセット
				g_player.CurrentKey = 0;
				g_player.CurrentFrame = 0;
			}
			//まだ歩いている場合
			else if (fabs(g_player.move.x) > 2 || fabs(g_player.move.z) > 2)
			{
				//キーのリセット
				g_player.CurrentKey = 0;
				g_player.CurrentFrame = 0;
			}
			else
			{
				//ニュートラルモーション
				AnimationSwitch(MOTIONTYPE_NEUTRAL);
				g_player.CurrentFrame = 0;

				//キーのリセット
				g_player.CurrentKey = 0;
			}
		}
	}
	//else
	//{
	//	if (g_player.nAnimationType != MOTIONTYPE_ATTACK_1 || g_player.nAnimationType != MOTIONTYPE_ATTACK_2)
	//	{

	//	}
	//	else if (fabs(g_player.move.x) > 2 || fabs(g_player.move.z) > 2)
	//	{

	//	}
	//	else
	//	{
	//		if (!g_player.nAnimationType == MOTIONTYPE_NEUTRAL)
	//		{
	//			//ニュートラルモーション
	//			AnimationSwitch(MOTIONTYPE_NEUTRAL);
	//			g_player.CurrentFrame = 0;

	//			//キーのリセット
	//			g_player.CurrentKey = 0;
	//		}
	//	}
	//}
}

//=============================================================================
// アニメーションの切り替え
//=============================================================================
void AnimationSwitch(MOTIONTYPE nType)
{
	if (g_player.nAnimationType != MOTIONTYPE_ATTACK_1 || g_player.nAnimationType != MOTIONTYPE_ATTACK_2)
	{
		if (g_player.nAnimationType != nType)
		{
			g_player.nAnimationType = nType;
			g_player.CurrentKey = 0;
			g_player.CurrentFrame = 0;

			for (int nCntMotion = 0; nCntMotion < g_nMaxPlayer; nCntMotion++)
			{
				// 位置・向きの初期設定
				g_player.aModel[nCntMotion].posPlayer = g_player.aModel[nCntMotion].originPos;
				g_player.aModel[nCntMotion].rotPlayer = g_player.aModel[nCntMotion].originRot;

				//rotの移動
				g_player.aModel[nCntMotion].rotPlayer = g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[1].rot;

				//posの移動
				g_player.aModel[nCntMotion].posPlayer += (g_player.aModel[nCntMotion].aMotion[g_player.nAnimationType].aKey[g_player.CurrentKey].pos + g_player.aModel[nCntMotion].originPos) - g_player.aModel[nCntMotion].posPlayer;

			}
		}
	}
}


//=============================================================================
// プレイヤー情報の取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

#ifdef _DEBUG
//=============================================================================
// データ表示
//=============================================================================
void DrawPlayerData(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	char sAnimation[2000];
	char sData[128];

	strcpy(&sAnimation[0], "==============パーツ情報===============\n");

	sprintf(&sData[0], "アニメーションの種類 : %d\n", g_player.nAnimationType);
	strcat(&sAnimation[0], &sData[0]);

	sprintf(&sData[0], "パーツ数 : %d\n", g_nMaxPlayer);
	strcat(&sAnimation[0], &sData[0]);

	sprintf(&sData[0], "プレイヤー体力 : %d\n", g_player.fLife);
	strcat(&sAnimation[0], &sData[0]);

	for (int nCntAnimation = 0; nCntAnimation < g_nMaxPlayer; nCntAnimation++)
	{
		//モデル情報(pos)
		sprintf(&sData[0], "[%d] : (%.6f, %.6f, %.6f)\n", nCntAnimation, g_player.aModel[nCntAnimation].posPlayer.x, g_player.aModel[nCntAnimation].posPlayer.y, g_player.aModel[nCntAnimation].posPlayer.z);
		strcat(&sAnimation[0], &sData[0]);

		//モデル情報(rot)
		sprintf(&sData[0], "rot : (%.6f, %.6f, %.6f)\n\n", g_player.aModel[nCntAnimation].rotPlayer.x, g_player.aModel[nCntAnimation].rotPlayer.y, g_player.aModel[nCntAnimation].rotPlayer.z);
		strcat(&sAnimation[0], &sData[0]);
	}

	sprintf(&sData[0], "フレーム数 : %d\n", g_player.CurrentFrame);
	strcat(&sAnimation[0], &sData[0]);
	sprintf(&sData[0], "キー数 : %d\n", g_player.CurrentKey);
	strcat(&sAnimation[0], &sData[0]);
	

	strcat(&sAnimation[0], "==============親情報===============\n");
	sprintf(&sData[0], "[%d] : (%.6f, %.6f, %.6f)\n", 0, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	strcat(&sAnimation[0], &sData[0]);


	// テキスト描画
	g_pPlayerFont->DrawText(NULL, &sAnimation[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif
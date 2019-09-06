//=============================================================================
//
// 体力処理 [hitpoint.cpp]
// Author : Seiya Takahashi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "hitpoint.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_ADD_HPbar	"data/tex/HPバー白.png"			//読み込むテクスチャファイル名
#define TEXTURE_ADD_HPframe "data/tex/HPバー黒.png"			//読み込むテクスチャファイル名
#define TEXTURE_ADD_HP		"data/tex/HP.png"				//読み込むテクスチャファイル名
#define MAX_HITPOINT (6)									//テクスチャ枚数
#define MAX_WIDTH (500.0f)									//画像の幅の最大値
#define MAX_HEIGHT (27.0f)									//画像の高さの最大値
#define PLAYER_POS D3DXVECTOR3(50, 20, 0)					//Playerバーの位置
#define ENEMY_POS D3DXVECTOR3(700, 20, 0)					//Enemyバーの位置

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureHITPOINT[MAX_HITPOINT] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHITPOINT = NULL;

HITPOINT g_HitPoint;
//=============================================================================
// 初期化処理
//=============================================================================
void InitHitPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *pPlayer = GetPlayer();
	ENEMY *pEnemy = GetEnemy();
	
	g_HitPoint.fMaxHp = (float)pPlayer->nLife;
	g_HitPoint.fNowHp = g_HitPoint.fMaxHp;
	g_HitPoint.fWidth = MAX_WIDTH;
	g_HitPoint.fMaxHpEnemy = (float)pEnemy->nLife;
	g_HitPoint.fNowHpEnemy = g_HitPoint.fMaxHpEnemy;
	g_HitPoint.fWidthEnemy = MAX_WIDTH;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HPbar, &g_pTextureHITPOINT[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HPframe, &g_pTextureHITPOINT[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HP, &g_pTextureHITPOINT[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HPbar, &g_pTextureHITPOINT[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HPframe, &g_pTextureHITPOINT[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HP, &g_pTextureHITPOINT[5]);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HITPOINT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHITPOINT,
		NULL);

	//テクスチャ設定
	//Player1の体力
	SetVertexHitPoint(0, PLAYER_POS, D3DXCOLOR(0.0f, 1.0f, 0.25f, 1.0f), MAX_WIDTH, MAX_HEIGHT, 0);
	SetVertexHitPoint(1, D3DXVECTOR3(50, 15, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH + 5.0f, 35.0f, 0);
	SetVertexHitPoint(2, D3DXVECTOR3(20, 15, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 30, 0);

	//Player2の体力
	SetVertexHitPoint(3, ENEMY_POS, D3DXCOLOR(0.0f, 1.0f, 0.25f, 1.0f), MAX_WIDTH, MAX_HEIGHT, 0.25f);
	SetVertexHitPoint(4, D3DXVECTOR3(700, 15, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH + 5.0f, 35.0f, 0);
	SetVertexHitPoint(5, D3DXVECTOR3(1180, 15, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 30, 0);
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitHitPoint(void)
{
	int nCntTex;

	for (nCntTex = 0; nCntTex < MAX_HITPOINT; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureHITPOINT[nCntTex] != NULL)
		{
			g_pTextureHITPOINT[nCntTex]->Release();
			g_pTextureHITPOINT[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffHITPOINT != NULL)
	{
		g_pVtxBuffHITPOINT->Release();
		g_pVtxBuffHITPOINT = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateHitPoint(void)
{
	PLAYER *pPlayer = GetPlayer();
	ENEMY *pEnemy = GetEnemy();
	float NowHP;
	float fWidth;																						//現在の画像の幅
	float fResidue;																						//画像の残りの長さ
	float NowHpEnemy;																					//敵の今の体力
	float fWidthEnemy;																					//現在の画像の幅
	float fResidueEnemy;																				//画像の残りの長さ

	//Playerの計算
	g_HitPoint.fNowHp = (float)pPlayer->nLife;															//現在のHP
	NowHP = g_HitPoint.fNowHp / g_HitPoint.fMaxHp;														//HPの比率計算
	fWidth = NowHP * g_HitPoint.fWidth;																	//描画すべき画像幅がいくらなのか
	fResidue = (g_HitPoint.fMaxHp - g_HitPoint.fNowHp) / g_HitPoint.fMaxHp;								//HPの残りの比率計算

	//Enemyの計算
	g_HitPoint.fNowHpEnemy = (float)pEnemy->nLife;														//現在のHP
	NowHpEnemy = g_HitPoint.fNowHpEnemy / g_HitPoint.fMaxHpEnemy;										//HPの比率計算
	fWidthEnemy = NowHpEnemy * g_HitPoint.fWidthEnemy;													//描画すべき画像幅がいくらなのか
	fResidueEnemy = (g_HitPoint.fMaxHpEnemy - g_HitPoint.fNowHpEnemy) / g_HitPoint.fMaxHpEnemy;			//HPの残りの比率計算

	//Playerの体力半分以上
	if (NowHP > 0.5f)
	{
		SetVertexHitPoint(0, PLAYER_POS, D3DXCOLOR(0.0f + fResidue * 2, 1.0f, 0.25f - (fResidue / 2), 1.0f), fWidth, MAX_HEIGHT, 0);
	}
	else
	{
		SetVertexHitPoint(0, PLAYER_POS, D3DXCOLOR(1.0f, NowHP * 2, 0.0f, 1.0f), fWidth, MAX_HEIGHT, 0);
	}

	//Enemyの体力半分以上
	if (NowHpEnemy > 0.5f)
	{
		SetVertexHitPoint(3, ENEMY_POS, D3DXCOLOR(0.0f + fResidueEnemy * 2, 1.0f, 0.25f - (fResidueEnemy / 2), 1.0f), fWidthEnemy, MAX_HEIGHT, 0.25f);
	}
	else
	{
		SetVertexHitPoint(3, ENEMY_POS, D3DXCOLOR(1.0f, NowHpEnemy * 2, 0.0f, 1.0f), fWidthEnemy, MAX_HEIGHT, 0.25f);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawHitPoint(void)
{
	int nCntTex;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffHITPOINT, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTex = 0; nCntTex < MAX_HITPOINT; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHITPOINT[nCntTex]);

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);

	}

}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexHitPoint(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float rot)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffHITPOINT->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += index * 4;					//頂点を4つずつ加算

	// 頂点情報の設定
	//頂点座標の設定(基準のx座標 + 間隔 * nCntScore (+ 幅), 基準のy座標)
	pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.75f + rot), pos.y + cosf(-D3DX_PI * 0.75f + rot), 0);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth + sinf(D3DX_PI * 0.75f + rot), pos.y + cosf(D3DX_PI * 0.75f + rot), 0);
	pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.25f + rot), pos.y + fHeight + cosf(-D3DX_PI * 0.25f + rot), 0);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth + sinf(D3DX_PI * 0.25f + rot), pos.y + fHeight + cosf(D3DX_PI * 0.25f + rot), 0);

	//1.0で固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーチャートの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//テクスチャ描写の位置
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データのアンロック
	g_pVtxBuffHITPOINT->Unlock();
}

//=============================================================================
// 体力の取得
//=============================================================================
HITPOINT *GetHitPoint(void)
{
	return &g_HitPoint;
}

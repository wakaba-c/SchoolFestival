//=============================================================================
//
// 経験値処理 [EXP.cpp]
// Author : Seiya Takahashi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "EXP.h"
#include "fade.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_ADD_EXPbar	"data/tex/HPbar.png"		//読み込むテクスチャファイル名
#define TEXTURE_ADD_EXPframe "data/tex/HPFrame.png"		//読み込むテクスチャファイル名
#define MAX_EXP (2)									//テクスチャ枚数
#define MAX_WIDTH (50.0f)									//画像の幅の最大値
#define MAX_HEIGHT (50.0f)									//画像の高さの最大値

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEXP[MAX_EXP] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEXP = NULL;

EXP g_EXP;
//=============================================================================
// 初期化処理
//=============================================================================
void InitEXP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *pPlayer = GetPlayer();

	g_EXP.fMaxEXP = 10;
	g_EXP.fNowEXP = g_EXP.fMaxEXP;
	g_EXP.fHeight = MAX_WIDTH;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_EXPbar, &g_pTextureEXP[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_EXPframe, &g_pTextureEXP[1]);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEXP,
		NULL);

	//テクスチャ設定
	SetVertexEXP(0, D3DXVECTOR3(100, 100, 0), D3DXCOLOR(1.0f, 0.27f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

	SetVertexEXP(1, D3DXVECTOR3(100, 100, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitEXP(void)
{
	int nCntTex;

	for (nCntTex = 0; nCntTex < MAX_EXP; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureEXP[nCntTex] != NULL)
		{
			g_pTextureEXP[nCntTex]->Release();
			g_pTextureEXP[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffEXP != NULL)
	{
		g_pVtxBuffEXP->Release();
		g_pVtxBuffEXP = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateEXP(void)
{
	PLAYER *pPlayer = GetPlayer();
	float NowEXP;
	float fHeight;																	//現在の画像の幅

	g_EXP.fNowEXP = pPlayer->fEXP;												//現在のHP

	NowEXP = g_EXP.fNowEXP / g_EXP.fMaxEXP;									//HPの比率計算
	fHeight = NowEXP * g_EXP.fHeight;												//描画すべき画像幅がいくらなのか

	SetVertexEXP(0, D3DXVECTOR3(100, 100, 0), D3DXCOLOR(1.0f, 0.27f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEXP(void)
{
	int nCntTex;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEXP, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTex = 0; nCntTex < MAX_EXP; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEXP[nCntTex]);

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexEXP(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEXP->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += index * 4;					//頂点を4つずつ加算

	// 頂点情報の設定
	//頂点座標の設定(基準のx座標 + 間隔 * nCntScore (+ 幅), 基準のy座標)
	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0);
	pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0);
	pVtx[2].pos = D3DXVECTOR3(pos.x + fWidth, pos.y, 0);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0);

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
	g_pVtxBuffEXP->Unlock();

}

//=============================================================================
// 体力の取得
//=============================================================================
EXP GetEXP(void)
{
	return g_EXP;
}

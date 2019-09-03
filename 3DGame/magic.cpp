//=============================================================================
//
// 魔法処理 [magic.cpp]
// Author : Seiya Takahashi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "magic.h"
#include "fade.h"
#include "input.h"
#include "mouse.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_ADD_MagicBar	"data/tex/HPバー白.png"		//読み込むテクスチャファイル名
#define TEXTURE_ADD_MagicFrame "data/tex/HPバー黒.png"		//読み込むテクスチャファイル名
#define TEXTURE_ADD_MP			"data/tex/MP.png"			//読み込むテクスチャファイル名
#define MAX_MAGIC (3)											//テクスチャ枚数
#define MAX_WIDTH (500.0f)										//画像の幅の最大値
#define MAX_HEIGHT (27.0f)										//画像の高さの最大値

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMagic[MAX_MAGIC] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagic = NULL;

MAGIC g_Magic;
//=============================================================================
// 初期化処理
//=============================================================================
void InitMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	g_Magic.fMaxMagic = 100.0f;
	g_Magic.fNowMagic = g_Magic.fMaxMagic;
	g_Magic.fWidth = MAX_WIDTH;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_MagicBar, &g_pTextureMagic[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_MagicFrame, &g_pTextureMagic[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_MP, &g_pTextureMagic[2]);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MAGIC,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMagic,
		NULL);

	//テクスチャ設定
	SetVertexMagic(0, D3DXVECTOR3(55, 55, 0), D3DXCOLOR(0.059f, 0.32f, 0.73f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

	SetVertexMagic(1, D3DXVECTOR3(50, 50, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH + 10.0f, 35.0f);

	SetVertexMagic(2, D3DXVECTOR3(20, 50, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 30);

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMagic(void)
{
	int nCntTex;

	for (nCntTex = 0; nCntTex < MAX_MAGIC; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureMagic[nCntTex] != NULL)
		{
			g_pTextureMagic[nCntTex]->Release();
			g_pTextureMagic[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagic(void)
{
	float NowMagic;
	float fWidth;																	//現在の画像の幅

	if (GetMousePressTrigger(0))
	{
		g_Magic.fNowMagic -= 10.0f;
	}
	else
	{
		g_Magic.fNowMagic += 0.75f;

		if (g_Magic.fNowMagic >= g_Magic.fMaxMagic)
		{
			g_Magic.fNowMagic = g_Magic.fMaxMagic;
		}
	}

	NowMagic = g_Magic.fNowMagic / g_Magic.fMaxMagic;									//魔力の比率計算
	fWidth = NowMagic * g_Magic.fWidth;													//描画すべき画像幅がいくらなのか

	SetVertexMagic(0, D3DXVECTOR3(55, 50, 0), D3DXCOLOR(0.059f, 0.32f, 0.73f, 1.0f), fWidth, MAX_HEIGHT);
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawMagic(void)
{
	int nCntTex;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTex = 0; nCntTex < MAX_MAGIC; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMagic[nCntTex]);

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);

	}

}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexMagic(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += index * 4;					//頂点を4つずつ加算

	// 頂点情報の設定
	//頂点座標の設定(基準のx座標 + 間隔 * nCntScore (+ 幅), 基準のy座標)
	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y, 0);
	pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0);

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
	g_pVtxBuffMagic->Unlock();

}

//=============================================================================
// 体力の取得
//=============================================================================
MAGIC *GetMagic(void)
{
	return &g_Magic;
}

//=============================================================================
//
// レベル処理 [level.cpp]
// Author : HIROYUKI KAWAHIGASHI
//
//=============================================================================
#include "level.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_LEVEL		"data/tex/number3D.png"	// 読み込むテクスチャファイル名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexLevel(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureLevel = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevel = NULL;	// 頂点バッファへのポインタ

int						g_nLevel;				// スコア

//=============================================================================
// 初期化処理
//=============================================================================
void InitLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nLevel = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_LEVEL,		// ファイルの名前
		&g_pTextureLevel);	// 読み込むメモリー

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevel,
		NULL);

	// 頂点情報の作成
	MakeVertexLevel(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLevel(void)
{
	//テクスチャの開放
	if (g_pTextureLevel != NULL)
	{
		g_pTextureLevel->Release();
		g_pTextureLevel = NULL;
	}
	//頂点バッファの開放
	if (g_pVtxBuffLevel != NULL)
	{
		g_pVtxBuffLevel->Release();
		g_pVtxBuffLevel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLevel(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffLevel, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLevel);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexLevel(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx; //頂点情報へのポインタ

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevel,
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタ取得
	g_pVtxBuffLevel->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(右回りで設定する)
	pVtx[0].pos = D3DXVECTOR3(90.0f, 10.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(120.0f, 10.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(90.0f, 70.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(120.0f, 70.0f, 0.0f);

	//同次座標(1.0で固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffLevel->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void AddLevel(int nValue)
{
	VERTEX_2D *pVtx; //頂点情報へのポインタ
	int nNumber;

	if (nValue >= 3)
	{
		nValue = 3;
	}
	g_nLevel = nValue;

	//頂点データの範囲をロックし、頂点バッファへのポインタ取得
	g_pVtxBuffLevel->Lock(0, 0, (void**)&pVtx, 0);

	nNumber = g_nLevel % (int)pow(10.0f, 1) / (int)pow(10.0f, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((float)nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)nNumber * 0.1f + 0.1f, 1.0f);

	//頂点データをアンロック
	g_pVtxBuffLevel->Unlock();
}


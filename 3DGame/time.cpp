//================================================================================================
//
// スコア処理 [time.cpp]
// Author : Seiya takahashi
//
//================================================================================================
#include "time.h"
#include "fade.h"

//================================================================================================
// マクロ定義
//================================================================================================
#define	TEXTURE_TIME		"data/tex/number000.png"	// 読み込むテクスチャファイル名
#define MAX_TIME (2)


//================================================================================================
// プロトタイプ宣言
//================================================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//================================================================================================
// グローバル変数宣言
//================================================================================================
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;				// 頂点バッファへのポインタ
DWORD					g_dStartTime;
int g_nTime;
int g_nSeconds;
int g_nMaxSeconds;

//================================================================================================
// タイムの初期化処理
//================================================================================================
void InitTime(void)
{
	g_nSeconds = 0;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_dStartTime = 0;
	SetSeconds(99);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_TIME,								// ファイルの名前
		&g_pTextureTime);							// 読み込むメモリー
						
	// 頂点情報の作成
	MakeVertexTime(pDevice);
}

//================================================================================================
// タイム終了処理
//================================================================================================
void UninitTime(void)
{
	// テクスチャ解放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	// 頂点バッファの解放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//================================================================================================
// タイムの更新処理
//================================================================================================
void UpdateTime(void)
{
	g_dStartTime--;
	if (g_dStartTime >= 0)
	{
		SetTime(g_dStartTime);
	}
	if (g_dStartTime == 0)
	{
		// 画面遷移
		SetFade(MODE_RESULT);
	}

}

//================================================================================================
// タイムの描画処理
//================================================================================================
void DrawTime(void)
{
	int nCount;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	// ポリゴンの描画
	for (nCount = 0; nCount < MAX_TIME; nCount++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}

//================================================================================================
// タイムの作成
//================================================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	int nCount;
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	for (nCount = 0; nCount < 2; nCount++)
	{
		// 頂点座標
		//頂点座標の設定(基準のx座標 + 間隔 * nCnt (+ 幅), 基準のy座標)
		pVtx[0].pos = D3DXVECTOR3(587.0f + 53 * nCount, 10.0f, 0);
		pVtx[1].pos = D3DXVECTOR3(587.0f + 53 * nCount + 50.0f, 10.0f, 0);
		pVtx[2].pos = D3DXVECTOR3(587.0f + 53 * nCount, 80.0f, 0);
		pVtx[3].pos = D3DXVECTOR3(587.0f + 53 * nCount + 50.0f, 80.0f, 0);

		// 1.0で固定、同次座標
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// 頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	// 頂点データをアンロック
	g_pVtxBuffTime->Unlock();
}

//================================================================================================
// タイムの変更
//================================================================================================
void SetTime(int nTime)
{
	int nCount;
	int nNumber;
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	g_nSeconds = nTime / 60 + 1;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_TIME; nCount++)
	{
		nNumber = (int)g_nSeconds % (int)pow(10, MAX_TIME - nCount) / (int)pow(10, MAX_TIME - 1 - nCount);

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(nNumber*0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber*0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber*0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber*0.1f + 0.1f, 1.0f);

		// 頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	// 頂点データをアンロック
	g_pVtxBuffTime->Unlock();
}

//================================================================================================
// タイムの変更
//================================================================================================
void SetSeconds(int nTime)
{
	g_dStartTime += nTime * 60;
	g_nMaxSeconds = g_dStartTime / 60;
}

//================================================================================================
// タイムの取得
//================================================================================================
int GetTime(void)
{
	return 	g_nSeconds;
}

//================================================================================================
// タイムの最大値の取得
//================================================================================================
int GetMaxTime(void)
{
	return g_nMaxSeconds;
}

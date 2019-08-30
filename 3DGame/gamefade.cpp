//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "gamefade.h"
#include "game.h"
#include "result.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_FADE	"data/TEXTURE/fade000.jpg"	// 読み込むテクスチャファイル名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexGameFade(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureGameFade = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameFade = NULL;		// 頂点バッファへのポインタ
GAMEFADE					g_Gamefade;						// フェード状態
MODE					g_modeGameNext;					// 次の画面（モード）
D3DXCOLOR				g_colorGameFade;				// フェード色

//=============================================================================
// 初期化処理
//=============================================================================
void InitGameFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Gamefade = GAMEFADE_IN;
	g_colorGameFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.5f);	// 黒い画面（不透明）

														// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_FADE,		// ファイルの名前
		&g_pTextureGameFade);	// 読み込むメモリー

							// 頂点情報の設定
	MakeVertexGameFade(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGameFade(void)
{
	// テクスチャの開放
	if (g_pTextureGameFade != NULL)
	{
		g_pTextureGameFade->Release();
		g_pTextureGameFade = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffGameFade != NULL)
	{
		g_pVtxBuffGameFade->Release();
		g_pVtxBuffGameFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameFade(void)
{
	VERTEX_2D *pVtx;

	if (g_Gamefade != GAMEFADE_NONE)
	{
		if (g_Gamefade == GAMEFADE_IN)
		{
			g_colorGameFade.a -= 0.05f;				//画面を透明にしていく
			if (g_colorGameFade.a <= 0.0f)
			{
				g_colorGameFade.a = 0.0f;
				g_Gamefade = GAMEFADE_NONE;
			}
		}
		else if (g_Gamefade == GAMEFADE_OUT)
		{
			g_colorGameFade.a += 0.05f;
			if (g_colorGameFade.a >= 1.0f)
			{
				//フェードイン処理に切り替え
				g_colorGameFade.a = 1.0f;
				g_Gamefade = GAMEFADE_IN;
			}
		}
	}
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffGameFade->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = g_colorGameFade;
	pVtx[1].col = g_colorGameFade;
	pVtx[2].col = g_colorGameFade;
	pVtx[3].col = g_colorGameFade;

	// 頂点データをアンロックする
	g_pVtxBuffGameFade->Unlock();
}

//=============================================================================
// ゲームフェード描画
//=============================================================================
void DrawGameFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffGameFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGameFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexGameFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameFade, NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffGameFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_colorGameFade;
	pVtx[1].col = g_colorGameFade;
	pVtx[2].col = g_colorGameFade;
	pVtx[3].col = g_colorGameFade;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffGameFade->Unlock();
}

//=============================================================================
// フェードの状態設定
//=============================================================================
void SetGameFade()
{
	g_Gamefade = GAMEFADE_OUT;
	//g_colorGameFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//黒い画面(透明)
}

//=============================================================================
// フェードの状態取得
//=============================================================================
GAMEFADE GetGameFade(void)
{
	return g_Gamefade;
}
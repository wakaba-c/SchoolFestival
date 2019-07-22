//========================================================================================
//
// ダメージ[gradation.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "gradation.h"
#include "player.h"

//========================================================================================
// マクロ定義
//========================================================================================
#define TEXTURE_GRADATION_ADD		"data/tex/Gradation.png"
#define TEXTURE_BLACKOUT			"data/tex/blood.png"
#define	GRADATION_MAX				2

//========================================================================================
// グローバル変数
//========================================================================================
LPDIRECT3DTEXTURE9 g_apTextureGradation[GRADATION_MAX] = {};						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGradation = NULL;				//頂点バッファへのポインタ

//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitGradation(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GRADATION_ADD, &g_apTextureGradation[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BLACKOUT, &g_apTextureGradation[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * GRADATION_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGradation, NULL);

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffGradation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < GRADATION_MAX; nCntTex++)
	{
		//座標の位置
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//1.0で固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntTex == 0)
		{
			if (GetMode() == MODE_GAME)
			{
				//カラーチャートの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			}
			else
			{
				//カラーチャートの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
			}
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点データのアンロック
	g_pVtxBuffGradation->Unlock();
}

//========================================================================================
// ポリゴンの開放処理
//========================================================================================
void UninitGradation(void)
{
		//テクスチャの開放

	for (int nCntTex = 0; nCntTex < GRADATION_MAX; nCntTex++)
	{
		if (g_apTextureGradation[nCntTex] != NULL)
		{
			g_apTextureGradation[nCntTex]->Release();
			g_apTextureGradation[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffGradation != NULL)
	{
		g_pVtxBuffGradation->Release();
		g_pVtxBuffGradation = NULL;
	}
}

//========================================================================================
// ポリゴンの更新処理
//========================================================================================
void UpdateGradation(void)
{
	VERTEX_2D *pVtx;
	PLAYER *pPlayer = GetPlayer();

	if (GetMode() == MODE_GAME)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffGradation->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;

		//カラーチャートの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - 0.7f * (pPlayer->nLife / (float)PLAYER_LIFE));
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - 0.7f * (pPlayer->nLife / (float)PLAYER_LIFE));
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - 0.7f * (pPlayer->nLife / (float)PLAYER_LIFE));
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - 0.7f * (pPlayer->nLife / (float)PLAYER_LIFE));

		//頂点データのアンロック
		g_pVtxBuffGradation->Unlock();
	}
}

//========================================================================================
// ポリゴンの描画処理
//========================================================================================
void DrawGradation(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	for (int nCntTex = 0; nCntTex < GRADATION_MAX; nCntTex++)
	{
		//頂点バッファをデバイスのデータにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffGradation, 0, sizeof(VERTEX_2D));

		//テクスチャの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//頂点フォーマットの設定
		pDevice->SetTexture(0, g_apTextureGradation[nCntTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTex, 2);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexGradation(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffGradation->Lock(0, 0, (void**)&pVtx, 0);

	//座標の位置
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

	//1.0で固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーチャートの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.5f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.5f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.5f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.5f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データのアンロック
	g_pVtxBuffGradation->Unlock();
}
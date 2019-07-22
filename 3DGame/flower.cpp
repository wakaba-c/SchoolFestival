//========================================================================================
//
// 飾り[flower.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "flower.h"
#include "player.h"

//========================================================================================
// マクロ定義
//========================================================================================
#define TEXTURE_FLOWER		"data/tex/leaf.png"
#define TEXTURE_CONTOUR		"data/tex/Crystal contour.png"
#define TEXTURE_CORE		"data/tex/Crystal.png"
#define TEXTURE_WAVE		"data/tex/wave.png"

//========================================================================================
// グローバル変数
//========================================================================================
LPDIRECT3DTEXTURE9 g_apTextureFlower[TEXTURETYPE_MAX] = {};						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlower = NULL;				//頂点バッファへのポインタ

FLOWER	g_aFlower[TEXTURETYPE_MAX];

float g_nCntPage = 0;
int g_nAnim = 0;

//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitFlower(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FLOWER, &g_apTextureFlower[TEXTURETYPE_FLOWER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_CONTOUR, &g_apTextureFlower[TEXTURETYPE_CONTOUR]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_CORE, &g_apTextureFlower[TEXTURETYPE_CORE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_WAVE, &g_apTextureFlower[TEXTURETYPE_WAVE]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TEXTURETYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFlower, NULL);

	//蔦
	MakeVertexFlower(TEXTURETYPE_FLOWER ,D3DXVECTOR3(1080, 100, 0.0f), D3DXVECTOR3(400, 200, 0.0f));

	//華の輪郭
	MakeVertexFlower(TEXTURETYPE_CONTOUR, D3DXVECTOR3(1200, 50, 0.0f), D3DXVECTOR3(150, 150, 0.0f));

	//華
	MakeVertexFlower(TEXTURETYPE_CORE, D3DXVECTOR3(1200, 50, 0.0f), D3DXVECTOR3(150, 150, 0.0f));

}

//========================================================================================
// ポリゴンの開放処理
//========================================================================================
void UninitFlower(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURETYPE_MAX; nCntTex++)
	{
		//テクスチャの開放
		if (g_apTextureFlower[nCntTex] != NULL)
		{
			g_apTextureFlower[nCntTex]->Release();
			g_apTextureFlower[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffFlower != NULL)
	{
		g_pVtxBuffFlower->Release();
		g_pVtxBuffFlower = NULL;
	}
}

//========================================================================================
// ポリゴンの更新処理
//========================================================================================
void UpdateFlower(void)
{
	if (GetMode() == MODE_GAME)
	{
		g_nAnim += 1;
		//華
		MakeVertexFlower(TEXTURETYPE_CORE, D3DXVECTOR3(1200, 50, 0.0f), D3DXVECTOR3(150, 150, 0.0f));
	}
}

//========================================================================================
// ポリゴンの描画処理
//========================================================================================
void DrawFlower(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	for (int nCntFlower = 0; nCntFlower < TEXTURETYPE_MAX; nCntFlower++)
	{
		//デバイスを取得する
		pDevice = GetDevice();

		//頂点バッファをデバイスのデータにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffFlower, 0, sizeof(VERTEX_2D));

		//テクスチャの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//頂点フォーマットの設定
		pDevice->SetTexture(0, g_apTextureFlower[nCntFlower]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFlower * 4, 2);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexFlower(int nIndex, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	VERTEX_2D *pVtx;
	PLAYER *pPlayer = GetPlayer();

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFlower->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nIndex;

	g_aFlower[nIndex].size = size;						//サイズの値を格納
	g_aFlower[nIndex].pos = pos;						//位置の値を格納

	g_aFlower[nIndex].fAngle = atan2f(g_aFlower[nIndex].size.x, g_aFlower[nIndex].size.y);
	g_aFlower[nIndex].fLength = (float)sqrt(g_aFlower[nIndex].size.x * g_aFlower[nIndex].size.x + g_aFlower[nIndex].size.y * g_aFlower[nIndex].size.y) / 2;

	//座標の位置
	pVtx[0].pos.x = g_aFlower[nIndex].pos.x + sinf(-D3DX_PI + g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[0].pos.y = g_aFlower[nIndex].pos.y + cosf(-D3DX_PI + g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aFlower[nIndex].pos.x + sinf(D3DX_PI - g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[1].pos.y = g_aFlower[nIndex].pos.y + cosf(D3DX_PI - g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aFlower[nIndex].pos.x + sinf(-g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[2].pos.y = g_aFlower[nIndex].pos.y + cosf(-g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aFlower[nIndex].pos.x + sinf(g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[3].pos.y = g_aFlower[nIndex].pos.y + cosf(g_aFlower[nIndex].fAngle + g_aFlower[nIndex].rot.z) * g_aFlower[nIndex].fLength;
	pVtx[3].pos.z = 0.0f;

	//1.0で固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	if (nIndex == TEXTURETYPE_CORE)
	{
		//カラーチャートの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 216, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 216, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 216, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 216, 255);
	}
	else if (nIndex == TEXTURETYPE_FLOWER || nIndex == TEXTURETYPE_CONTOUR)
	{
		//カラーチャートの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 216, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 216, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 216, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 216, 255);
	}
	else
	{
		//カラーチャートの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	}

	if (nIndex == TEXTURETYPE_WAVE)
	{
		if (g_nAnim % 5 == 0)
		{
			g_nCntPage += 1;
			//テクスチャ描写の位置
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCntPage, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCntPage, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCntPage, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCntPage, 1.0f);
		}
	}
	else
	{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点データのアンロック
	g_pVtxBuffFlower->Unlock();
}
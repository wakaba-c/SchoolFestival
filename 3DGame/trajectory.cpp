//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "polygon.h"
#include "input.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEPTH 	50											//奥行き
#define WIDE 50												//幅

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexTrajectory(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTrajectory = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTrajectory = NULL;			//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffTrajectory = NULL;			//インデックスバッファへのポインタ

D3DXVECTOR3				g_posTrajectory;						//ポリゴンの位置
D3DXVECTOR3				g_rotTrajectory;						//ポリゴンの向き(回転)
D3DXMATRIX				g_mtxWorldTrajectory;				//ワールドマトリックス

int g_nNumVertexTrajectory;									//総頂点数
int g_nNumIndexTrajectory;									//インデックス数
int g_nNumPolygonTrajectory;									//総ポリゴン数

int g_nCount;		//狩り

//=============================================================================
// 初期化処理
//=============================================================================
void InitTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	WORD *pIdx;									//インデックスデータへポインタ

												// 位置・回転の初期設定
	g_posTrajectory = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotTrajectory = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//総頂点数
	g_nNumVertexTrajectory = (DEPTH + 1) * (WIDE + 1);

	//インデックス数
	g_nNumIndexTrajectory = ((DEPTH + 1) * 2) * DEPTH + ((WIDE - 1) * 2);

	//総ポリゴン数
	g_nNumPolygonTrajectory = DEPTH * 2 + WIDE * 2;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/stone.pn", &g_pTextureTrajectory);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (DEPTH + 1) * (WIDE + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffTrajectory, NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * DEPTH * WIDE * (2 + 4), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffTrajectory, NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < DEPTH + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WIDE + 1; nWide++)
		{
			//頂点座標の設定
			pVtx[(WIDE + 1) * nDepth + nWide].pos = D3DXVECTOR3((-50.0f * WIDE) + 100 * nWide, 0.0f, (50.0f * DEPTH) - 100 * nDepth);
			//法線ベクトル
			pVtx[(WIDE + 1) * nDepth + nWide].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[(WIDE + 1) * nDepth + nWide].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			//テクスチャ描写の位置
			pVtx[(WIDE + 1) * nDepth + nWide].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

		}
	}

	//頂点データのアンロック
	g_pVtxBuffTrajectory->Unlock();

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffTrajectory->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH; nCntZ++)
	{
		if (nCntZ != 0)
		{
			pIdx[1] = (DEPTH + 1) + nCntZ * (DEPTH + 1);
			pIdx += 2;
		}

		for (int nCntX = 0; nCntX < WIDE + 1; nCntX++)
		{
			pIdx[0] = (DEPTH + 1) + nCntX + nCntZ * (DEPTH + 1);
			pIdx[1] = 0 + nCntX + nCntZ * (DEPTH + 1);
			pIdx += 2;
		}

		if (DEPTH > 0 && DEPTH > nCntZ)
		{
			pIdx[0] = DEPTH + nCntZ * (DEPTH + 1);
		}
	}

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffTrajectory->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTrajectory(void)
{
	//テクスチャの開放
	if (g_pTextureTrajectory != NULL)
	{
		g_pTextureTrajectory->Release();
		g_pTextureTrajectory = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffTrajectory != NULL)
	{
		g_pIdxBuffTrajectory->Release();
		g_pIdxBuffTrajectory = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffTrajectory != NULL)
	{
		g_pVtxBuffTrajectory->Release();
		g_pVtxBuffTrajectory = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTrajectory(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				//計算用マトリックス

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldTrajectory);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotTrajectory.y, g_rotTrajectory.x, g_rotTrajectory.z);
	D3DXMatrixMultiply(&g_mtxWorldTrajectory, &g_mtxWorldTrajectory, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posTrajectory.x, g_posTrajectory.y, g_posTrajectory.z);
	D3DXMatrixMultiply(&g_mtxWorldTrajectory, &g_mtxWorldTrajectory, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldTrajectory);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

	// 頂点バッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffTrajectory);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureTrajectory);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexTrajectory, 0, g_nNumIndexTrajectory);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTrajectory(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffTrajectory, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-50.0f, 0.0f, 50.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	pVtx[2].pos = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, -50.0f);

	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点データのアンロック
	g_pVtxBuffTrajectory->Unlock();
}

//=============================================================================
// セット完了
//=============================================================================
void SetTrajectory(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	WORD *pIdx;									//インデックスデータへポインタ

	PLAYER *pPlayer = GetPlayer();

	////頂点座標の設定
	//pVtx[(WIDE + 1) * nDepth + nWide].pos = pos;
	////法線ベクトル
	//pVtx[(WIDE + 1) * nDepth + nWide].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	////頂点カラー
	//pVtx[(WIDE + 1) * nDepth + nWide].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	////テクスチャ描写の位置
	//pVtx[(WIDE + 1) * nDepth + nWide].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

	//頂点データのアンロック
	g_pVtxBuffTrajectory->Unlock();

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffTrajectory->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = g_nCount;
	pIdx[1] = g_nCount + 1;
	pIdx += 2;
	g_nCount += 2;

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffTrajectory->Unlock();
}
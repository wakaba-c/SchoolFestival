//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "polygon.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEPTH 	50											//奥行き
#define WIDE 50												//幅

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;			//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;			//インデックスバッファへのポインタ

D3DXVECTOR2				g_tex;

D3DXVECTOR3				g_posMeshField;						//ポリゴンの位置
D3DXVECTOR3				g_rotMeshField;						//ポリゴンの向き(回転)
D3DXMATRIX				g_mtxWorldMeshField;				//ワールドマトリックス

int g_nNumVertexMeshField;									//総頂点数
int g_nNumIndexMeshField;									//インデックス数
int g_nNumPolygonMeshField;									//総ポリゴン数

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	WORD *pIdx;									//インデックスデータへポインタ

	g_tex = D3DXVECTOR2(1.0f, 1.0f);

												// 位置・回転の初期設定
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//総頂点数
	g_nNumVertexMeshField = (DEPTH + 1) * (WIDE + 1);

	//インデックス数
	g_nNumIndexMeshField = ((DEPTH + 1) *  2) * DEPTH + ((WIDE - 1) * 2);

	//総ポリゴン数
	g_nNumPolygonMeshField = DEPTH * WIDE * 2 + 4 * (DEPTH - 1);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/FOG.png", &g_pTextureMeshField);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (DEPTH + 1) * (WIDE + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * DEPTH * WIDE * (2 + 4), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < DEPTH + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WIDE + 1; nWide++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((-100.0f * WIDE) / 2 + 100 * nWide, 0.0f, (50.0f * DEPTH) - 100 * nDepth);

			//法線ベクトル
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ描写の位置
			pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			pVtx++;
		}
	}

	//頂点データのアンロック
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < DEPTH; nCntY++)
	{
		if (nCntY != 0)
		{
			pIdx[0] = (WIDE + 1) + nCntY * (WIDE + 1);
			pIdx++;
		}

		for (int nCntX = 0; nCntX < WIDE + 1; nCntX++)
		{
			pIdx[0] = (WIDE + 1) + nCntX + nCntY * (WIDE + 1);
			pIdx[1] = nCntX + nCntY * (WIDE + 1);
			pIdx += 2;
		}

		if (DEPTH > nCntY + 1)
		{
			pIdx[0] = WIDE + nCntY * (WIDE + 1);
			pIdx++;

		}
	}

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffMeshField->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	//テクスチャの開放
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				//計算用マトリックス

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	// 頂点バッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//// ポリゴンの描画
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexMeshField, 0, g_nNumPolygonMeshField);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffMeshField->Unlock();
}


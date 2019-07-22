//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshSphere.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define RADIUS	120										//半径
#define DIVISION 20										//高さ
#define DEPTH 	20										//幅

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMeshSphere(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMeshSphere = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshSphere = NULL;			//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshSphere = NULL;			//インデックスバッファへのポインタ

D3DXVECTOR3				g_posMeshSphere;						//ポリゴンの位置
D3DXVECTOR3				g_rotMeshSphere;						//ポリゴンの向き(回転)
D3DXMATRIX				g_mtxWorldMeshSphere;				//ワールドマトリックス

int g_nNumVertexMeshSphere;									//総頂点数
int g_nNumIndexMeshSphere;									//インデックス数
int g_nNumPolygonMeshSphere;									//総ポリゴン数

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	WORD *pIdx;									//インデックスデータへポインタ

												// 位置・回転の初期設定
	g_posMeshSphere = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	g_rotMeshSphere = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//総頂点数
	g_nNumVertexMeshSphere = (DIVISION + 1) + (DEPTH + 1) * (DIVISION + 1);

	//インデックス数
	g_nNumIndexMeshSphere = (DIVISION + 1) + ((DEPTH + 1) * 2) * DEPTH + ((DIVISION - 1) * 2);

	//総ポリゴン数
	g_nNumPolygonMeshSphere = (DIVISION - 1) + (DEPTH * DIVISION * 2 + 4 * (DEPTH - 1));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/wood_Seamless.jg", &g_pTextureMeshSphere);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexMeshSphere, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshSphere, NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshSphere, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshSphere, NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//================= 円の中心 ===================//
	pVtx[0].pos = g_posMeshSphere;

	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ描写の位置
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

	//pVtx[1].pos = D3DXVECTOR3(40.0f, 25.0f, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(0.0f, 25.0f, 40.0f);
	//pVtx[3].pos = D3DXVECTOR3(-40.0f, 25.0f, 0.0f);
	//pVtx[4].pos = D3DXVECTOR3(0.0f, 25.0f, -40.0f);
	//pVtx[5].pos = D3DXVECTOR3(40.0f, 25.0f, 0.0f);
	pVtx++;

	//================= 円を形成 ===================//
	for (int nDepth = 0; nDepth < DIVISION; nDepth++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f + ((6.28f / (DIVISION - 1)) * nDepth)) * RADIUS, 5.0f, cosf(D3DX_PI + 6.28f + ((6.28f / (DIVISION - 1)) * nDepth)) * RADIUS);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ描写の位置
		pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f * nDepth);

		pVtx++;
	}

	//for (int nDepth = 0; nDepth < 2; nDepth++)
	//{
	//	for (int nWide = 0; nWide < DIVISION; nWide++)
	//	{
	//		//頂点座標の設定
	//		pVtx[0].pos = D3DXVECTOR3((-100.0f * DIVISION) / 2 + 100 * nWide, (100.0f * DIVISION) - 100 * nDepth, 0.0f);

	//		//法線ベクトル
	//		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//		//頂点カラー
	//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//		//テクスチャ描写の位置
	//		pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

	//		pVtx++;
	//	}
	//}

	//頂点データのアンロック
	g_pVtxBuffMeshSphere->Unlock();

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffMeshSphere->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < g_nNumVertexMeshSphere; nCntY++)
	{
		pIdx[0] = nCntY;
		pIdx++;
	}


	//for (int nCntY = 0; nCntY < 2; nCntY++)
	//{
	//	if (nCntY != 0)
	//	{
	//		pIdx[0] = (DIVISION + 1) + nCntY * (DIVISION + 1);
	//		pIdx++;
	//	}

	//	for (int nCntX = 0; nCntX < DIVISION + 1; nCntX++)
	//	{
	//		pIdx[0] = (DIVISION + 1) + nCntX + nCntY * (DIVISION + 1);
	//		pIdx[1] = nCntX + nCntY * (DIVISION + 1);
	//		pIdx += 2;
	//	}

	//	if (DEPTH > nCntY + 1)
	//	{
	//		pIdx[0] = DIVISION + nCntY * (DIVISION + 1);
	//		pIdx++;

	//	}
	//}

	//インデックスバッファをロックし、インデックスデータへのポインタ
	g_pIdxBuffMeshSphere->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshSphere(void)
{
	//テクスチャの開放
	if (g_pTextureMeshSphere != NULL)
	{
		g_pTextureMeshSphere->Release();
		g_pTextureMeshSphere = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshSphere != NULL)
	{
		g_pIdxBuffMeshSphere->Release();
		g_pIdxBuffMeshSphere = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshSphere != NULL)
	{
		g_pVtxBuffMeshSphere->Release();
		g_pVtxBuffMeshSphere = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshSphere(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				//計算用マトリックス

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshSphere);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshSphere.y, g_rotMeshSphere.x, g_rotMeshSphere.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshSphere, &g_mtxWorldMeshSphere, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshSphere.x, g_posMeshSphere.y, g_posMeshSphere.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshSphere, &g_mtxWorldMeshSphere, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshSphere);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshSphere, 0, sizeof(VERTEX_3D));

	// 頂点バッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshSphere);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureMeshSphere);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (DIVISION + 1), 0, (DIVISION + 1));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, (DIVISION + 1), (DIVISION + 1), (DIVISION + 1), 0, (DIVISION + 1));

	//// ポリゴンの描画
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, (DIVISION + 1), (DIVISION + 1), g_nNumVertexMeshSphere, (DIVISION + 1) + 1, g_nNumPolygonMeshSphere);

	// //ポリゴンの描画
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, g_nNumVertexMeshSphere, 0, g_nNumPolygonMeshSphere);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshSphere(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshSphere, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffMeshSphere->Unlock();
}


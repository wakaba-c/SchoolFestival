//=============================================================================
//
// ポリゴン処理 [wall.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "wall.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_WALL	(130)
#define WALL_DEPTH	7
#define WALL_WIDE 20

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 rot;										//回転
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffWall;					//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshField;				//インデックスバッファへのポインタ
	int nNumVertexMeshField;								//総頂点数
	int nNumPolygonMeshField;								//総ポリゴン数
	bool bUse;												//使用しているかどうか
} WALL;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;			//テクスチャへのポインタ
WALL					g_aWall[MAX_WALL];				//壁
int g_nNumIndexWall;									//インデックス数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// 初期化処理
//=============================================================================
void InitWall (void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].bUse = false;
		g_aWall[nCntWall].pVtxBuffWall = NULL;
		g_aWall[nCntWall].pIdxBuffMeshField = NULL;
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//総頂点数
		g_aWall[nCntWall].nNumVertexMeshField = (WALL_DEPTH + 1) * (WALL_WIDE + 1);

		//インデックス数
		g_nNumIndexWall = ((WALL_DEPTH + 1) * 2) * WALL_DEPTH + ((WALL_WIDE - 1) * 2);

		//総ポリゴン数
		g_aWall[nCntWall].nNumPolygonMeshField = WALL_DEPTH * WALL_WIDE * 2 + 4 * (WALL_DEPTH - 1);

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (WALL_DEPTH + 1) * (WALL_WIDE + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_aWall[nCntWall].pVtxBuffWall, NULL);

		//インデックスバッファを生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * WALL_DEPTH * WALL_WIDE * (2 + 4), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_aWall[nCntWall].pIdxBuffMeshField, NULL);
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/FOG.png", &g_pTextureWall);

	//MakeVertexWall(pDevice);

	//SetWall(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0));
	//SetWall(D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(500.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	//テクスチャの開放
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	for (int nCntBuff = 0; nCntBuff < MAX_WALL; nCntBuff++)
	{
		//インデックスバッファの開放
		if (g_aWall[nCntBuff].pIdxBuffMeshField != NULL)
		{
			g_aWall[nCntBuff].pIdxBuffMeshField->Release();
			g_aWall[nCntBuff].pIdxBuffMeshField = NULL;
		}

		// 頂点バッファの開放
		if (g_aWall[nCntBuff].pVtxBuffWall != NULL)
		{
			g_aWall[nCntBuff].pVtxBuffWall->Release();
			g_aWall[nCntBuff].pVtxBuffWall = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//計算用マトリックス


	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aWall[nCntWall].pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 頂点バッファをデータストリームに設定
			pDevice->SetIndices(g_aWall[nCntWall].pIdxBuffMeshField);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureWall);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aWall[nCntWall].nNumVertexMeshField, 0, g_aWall[nCntWall].nNumPolygonMeshField);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, NULL);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	WORD *pIdx;									//インデックスデータへポインタ

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_aWall[nCntWall].pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

		for (int nDepth = 0; nDepth < WALL_DEPTH + 1; nDepth++)
		{
			for (int nWide = 0; nWide < WALL_WIDE + 1; nWide++)
			{
				//頂点座標の設定
				pVtx[(WALL_WIDE + 1) * nDepth + nWide].pos = D3DXVECTOR3((-50.0f * WALL_WIDE) + 100.0f * nWide, (50.0f * WALL_DEPTH) - 50.0f * nDepth, 0.0f);
				//法線
				pVtx[(WALL_WIDE + 1) * nDepth + nWide].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				//頂点カラー
				pVtx[(WALL_WIDE + 1) * nDepth + nWide].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		//頂点データのアンロック
		g_aWall[nCntWall].pVtxBuffWall->Unlock();

		//インデックスバッファをロックし、インデックスデータへのポインタ
		g_aWall[nCntWall].pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntY = 0; nCntY < WALL_DEPTH; nCntY++)
		{
			if (nCntY != 0)
			{
				pIdx[1] = (WALL_WIDE + 1) + nCntY * (WALL_WIDE + 1);
				pIdx += 2;
			}

			for (int nCntX = 0; nCntX < WALL_WIDE + 1; nCntX++)
			{
				pIdx[0] = (WALL_WIDE + 1) + nCntX + nCntY * (WALL_WIDE + 1);
				pIdx[1] = nCntX + nCntY * (WALL_WIDE + 1);
				pIdx += 2;
			}

			if (WALL_DEPTH > nCntY)
			{
				pIdx[0] = WALL_WIDE + nCntY * (WALL_WIDE + 1);
			}
		}

		//インデックスバッファをロックし、インデックスデータへのポインタ
		g_aWall[nCntWall].pIdxBuffMeshField->Unlock();
	}
}

//=============================================================================
// 壁の作成
//=============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	WORD *pIdx;									//インデックスデータへポインタ

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (!g_aWall[nCntWall].bUse)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_aWall[nCntWall].pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

			for (int nDepth = 0; nDepth < WALL_DEPTH + 1; nDepth++)
			{
				for (int nWide = 0; nWide < WALL_WIDE + 1; nWide++)
				{
					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3((-100.0f * WALL_WIDE) / 2 + 100 * nWide, (100.0f * WALL_DEPTH) - 100 * nDepth, 0.0f);

					//法線ベクトル
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//テクスチャ描写の位置
					pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

					pVtx++;
				}
			}

			//頂点データのアンロック
			g_aWall[nCntWall].pVtxBuffWall->Unlock();

			//インデックスバッファをロックし、インデックスデータへのポインタ
			g_aWall[nCntWall].pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntY = 0; nCntY < WALL_DEPTH; nCntY++)
			{
				if (nCntY != 0)
				{
					pIdx[0] = (WALL_WIDE + 1) + nCntY * (WALL_WIDE + 1);
					pIdx++;
				}

				for (int nCntX = 0; nCntX < WALL_WIDE + 1; nCntX++)
				{
					pIdx[0] = (WALL_WIDE + 1) + nCntX + nCntY * (WALL_WIDE + 1);
					pIdx[1] = nCntX + nCntY * (WALL_WIDE + 1);
					pIdx += 2;
				}

				if (WALL_DEPTH > nCntY + 1)
				{
					pIdx[0] = WALL_WIDE + nCntY * (WALL_WIDE + 1);
					pIdx++;

				}
			}

			//インデックスバッファをロックし、インデックスデータへのポインタ
			g_aWall[nCntWall].pIdxBuffMeshField->Unlock();

			g_aWall[nCntWall].bUse = true;
			break;
		}
	}
}
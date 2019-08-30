//=============================================================================
//
// ポリゴン処理 [meshorbit.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshorbit.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ORBIT	(1)
#define ORBIT_DEPTH	1
#define ORBIT_WIDE 10

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 rot;										//回転
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffOrbit;					//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshOrbit;				//インデックスバッファへのポインタ
	int nNumVertexMeshOrbit;								//総頂点数
	int nNumPolygonMeshOrbit;								//総ポリゴン数
	bool bUse;												//使用しているかどうか
} ORBIT;

typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 rot;										//回転
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffOrbit;					//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshOrbit;				//インデックスバッファへのポインタ
	int nNumVertexMeshOrbit;								//総頂点数
	int nNumPolygonMeshOrbit;								//総ポリゴン数
	bool bUse;												//使用しているかどうか
	int nLife;												//時限
} ORBITMTX;


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureOrbit = NULL;			//テクスチャへのポインタ
ORBIT					g_aOrbit[MAX_ORBIT];				//壁
ORBITMTX				g_aMtxMeshOrbit;					
int g_nNumIndexOrbit;									//インデックス数

//持ち手
D3DXMATRIX g_mtxWorldMeshOrbit;
D3DXMATRIX g_mtxWorldMeshOrbitOld[ORBIT_WIDE];
//剣先
D3DXMATRIX g_mtxWorldWeapon;
D3DXMATRIX g_mtxWorldWeaponOld[ORBIT_WIDE];


//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// 初期化処理
//=============================================================================
void InitOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												// 位置・回転の初期設定
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aOrbit[nCntOrbit].bUse = false;
		g_aOrbit[nCntOrbit].pVtxBuffOrbit = NULL;
		g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit = NULL;
		g_aOrbit[nCntOrbit].pos = D3DXVECTOR3(0.0f,5.0f, 0.0f);
		g_aOrbit[nCntOrbit].rot = D3DXVECTOR3(133.0f, 3.0f, 3.0f);

		//総頂点数
		g_aOrbit[nCntOrbit].nNumVertexMeshOrbit = (ORBIT_DEPTH + 1) * (ORBIT_DEPTH + 1);

		//インデックス数
		g_nNumIndexOrbit = ((ORBIT_DEPTH + 1) * 2) * ORBIT_DEPTH + ((ORBIT_WIDE - 1) * 2);

		//総ポリゴン数
		g_aOrbit[nCntOrbit].nNumPolygonMeshOrbit = ORBIT_DEPTH * ORBIT_WIDE * 2 + 4 * (ORBIT_DEPTH - 1);

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (ORBIT_DEPTH + 1) * (ORBIT_WIDE + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_aOrbit[nCntOrbit].pVtxBuffOrbit, NULL);

		//インデックスバッファを生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * ORBIT_DEPTH * ORBIT_WIDE * (2 + 4), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit, NULL);
	}

	g_aMtxMeshOrbit.pos = D3DXVECTOR3(28.0f, 3.0f, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/dam.jp", &g_pTextureOrbit);

	//MakeVertexOrbit(pDevice);

	SetOrbit(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOrbit(void)
{
	//テクスチャの開放
	if (g_pTextureOrbit != NULL)
	{
		g_pTextureOrbit->Release();
		g_pTextureOrbit = NULL;
	}

	for (int nCntBuff = 0; nCntBuff < MAX_ORBIT; nCntBuff++)
	{
		//インデックスバッファの開放
		if (g_aOrbit[nCntBuff].pIdxBuffMeshOrbit != NULL)
		{
			g_aOrbit[nCntBuff].pIdxBuffMeshOrbit->Release();
			g_aOrbit[nCntBuff].pIdxBuffMeshOrbit = NULL;
		}

		// 頂点バッファの開放
		if (g_aOrbit[nCntBuff].pVtxBuffOrbit != NULL)
		{
			g_aOrbit[nCntBuff].pVtxBuffOrbit->Release();
			g_aOrbit[nCntBuff].pVtxBuffOrbit = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOrbit(void)
{
	VERTEX_3D *pVtx;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_aOrbit[0].pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_mtxWorldWeapon._41, g_mtxWorldWeapon._42, g_mtxWorldWeapon._43);
	pVtx[ORBIT_WIDE + 1].pos = D3DXVECTOR3(pPlayer->aModel[14].mtxWorldPlayer._41, pPlayer->aModel[14].mtxWorldPlayer._42, pPlayer->aModel[14].mtxWorldPlayer._43);

	for (int nCntOrbit = 0; nCntOrbit < ORBIT_WIDE; nCntOrbit++)
	{
		pVtx[nCntOrbit + 1].pos = D3DXVECTOR3(
			g_mtxWorldWeaponOld[nCntOrbit]._41,
			g_mtxWorldWeaponOld[nCntOrbit]._42,
			g_mtxWorldWeaponOld[nCntOrbit]._43);

		pVtx[nCntOrbit + ORBIT_WIDE + 2].pos = D3DXVECTOR3(
			g_mtxWorldMeshOrbitOld[nCntOrbit]._41,
			g_mtxWorldMeshOrbitOld[nCntOrbit]._42,
			g_mtxWorldMeshOrbitOld[nCntOrbit]._43);
	}


	for (int nCntObit = ORBIT_WIDE - 1; nCntObit > 0; nCntObit--)
	{
		g_mtxWorldMeshOrbitOld[nCntObit] = g_mtxWorldMeshOrbitOld[nCntObit - 1];
		g_mtxWorldWeaponOld[nCntObit] = g_mtxWorldWeaponOld[nCntObit - 1];
	}

	g_mtxWorldMeshOrbitOld[0] = pPlayer->aModel[14].mtxWorldPlayer;
	g_mtxWorldWeaponOld[0] = g_mtxWorldWeapon;

	//頂点データのアンロック
	g_aOrbit[0].pVtxBuffOrbit->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOrbit(void)
{
	PLAYER *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面(左回り)をカリングする


	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// ワールドマトリックスの初期化
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldMeshOrbit);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldWeapon);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aOrbit[nCntOrbit].rot.y, g_aOrbit[nCntOrbit].rot.x, g_aOrbit[nCntOrbit].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldMeshOrbit, &g_mtxWorldMeshOrbit, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aOrbit[nCntOrbit].pos.x, g_aOrbit[nCntOrbit].pos.y, g_aOrbit[nCntOrbit].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldMeshOrbit, &g_mtxWorldMeshOrbit, &mtxTrans);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, g_aMtxMeshOrbit.rot.y, g_aMtxMeshOrbit.rot.x, g_aMtxMeshOrbit.rot.z);
			D3DXMatrixMultiply(&g_mtxWorldWeapon, &g_mtxWorldWeapon, &mtxMeshRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxMeshTrans, g_aMtxMeshOrbit.pos.x, g_aMtxMeshOrbit.pos.y, g_aMtxMeshOrbit.pos.z);
			D3DXMatrixMultiply(&g_mtxWorldWeapon, &g_mtxWorldWeapon, &mtxMeshTrans);

			D3DXMatrixMultiply(&g_mtxWorldWeapon, &g_mtxWorldWeapon, &pPlayer->aModel[3].mtxWorldPlayer);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshOrbit);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aOrbit[nCntOrbit].pVtxBuffOrbit, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 頂点バッファをデータストリームに設定
			pDevice->SetIndices(g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureOrbit);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aOrbit[nCntOrbit].nNumVertexMeshOrbit, 0, g_aOrbit[nCntOrbit].nNumPolygonMeshOrbit);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, NULL);
		}
	}

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice)
{
	WORD *pIdx;									//インデックスデータへポインタ

												// 頂点情報の設定
	VERTEX_3D *pVtx;

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_aOrbit[nCntOrbit].pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

		for (int nDepth = 0; nDepth < ORBIT_DEPTH + 1; nDepth++)
		{
			for (int nWide = 0; nWide < ORBIT_WIDE + 1; nWide++)
			{
				//頂点座標の設定
				pVtx[(ORBIT_WIDE + 1) * nDepth + nWide].pos = D3DXVECTOR3((-50.0f * ORBIT_WIDE) + 100.0f * nWide, (50.0f * ORBIT_DEPTH) - 50.0f * nDepth, 0.0f);
				//法線
				pVtx[(ORBIT_WIDE + 1) * nDepth + nWide].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				//頂点カラー
				pVtx[(ORBIT_WIDE + 1) * nDepth + nWide].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		//頂点データのアンロック
		g_aOrbit[nCntOrbit].pVtxBuffOrbit->Unlock();

		//インデックスバッファをロックし、インデックスデータへのポインタ
		g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntY = 0; nCntY < ORBIT_DEPTH; nCntY++)
		{
			if (nCntY != 0)
			{
				pIdx[1] = (ORBIT_WIDE + 1) + nCntY * (ORBIT_WIDE + 1);
				pIdx += 2;
			}

			for (int nCntX = 0; nCntX < ORBIT_WIDE + 1; nCntX++)
			{
				pIdx[0] = (ORBIT_WIDE + 1) + nCntX + nCntY * (ORBIT_WIDE + 1);
				pIdx[1] = nCntX + nCntY * (ORBIT_WIDE + 1);
				pIdx += 2;
			}

			if (ORBIT_DEPTH > nCntY)
			{
				pIdx[0] = ORBIT_WIDE + nCntY * (ORBIT_WIDE + 1);
			}
		}

		//インデックスバッファをロックし、インデックスデータへのポインタ
		g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit->Unlock();
	}
}

//=============================================================================
// 壁の作成
//=============================================================================
void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	WORD *pIdx;									//インデックスデータへポインタ

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (!g_aOrbit[nCntOrbit].bUse)
		{
			g_aOrbit[nCntOrbit].pos = pos;
			g_aOrbit[nCntOrbit].rot = rot;

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_aOrbit[nCntOrbit].pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

			for (int nDepth = 0; nDepth < ORBIT_DEPTH + 1; nDepth++)
			{
				for (int nWide = 0; nWide < ORBIT_WIDE + 1; nWide++)
				{
					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3((-100.0f * ORBIT_WIDE) / 2 + 100 * nWide, (100.0f * ORBIT_DEPTH) - 100 * nDepth, 0.0f);

					//法線ベクトル
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);

					//テクスチャ描写の位置
					pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

					pVtx++;
				}
			}

			//頂点データのアンロック
			g_aOrbit[nCntOrbit].pVtxBuffOrbit->Unlock();

			//インデックスバッファをロックし、インデックスデータへのポインタ
			g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntY = 0; nCntY < ORBIT_DEPTH; nCntY++)
			{
				if (nCntY != 0)
				{
					pIdx[0] = (ORBIT_WIDE + 1) + nCntY * (ORBIT_WIDE + 1);
					pIdx++;
				}

				for (int nCntX = 0; nCntX < ORBIT_WIDE + 1; nCntX++)
				{
					pIdx[0] = (ORBIT_WIDE + 1) + nCntX + nCntY * (ORBIT_WIDE + 1);
					pIdx[1] = nCntX + nCntY * (ORBIT_WIDE + 1);
					pIdx += 2;
				}

				if (ORBIT_DEPTH > nCntY + 1)
				{
					pIdx[0] = ORBIT_WIDE + nCntY * (ORBIT_WIDE + 1);
					pIdx++;

				}
			}

			//インデックスバッファをロックし、インデックスデータへのポインタ
			g_aOrbit[nCntOrbit].pIdxBuffMeshOrbit->Unlock();

			g_aOrbit[nCntOrbit].bUse = true;
			break;
		}
	}
}

//=============================================================================
// 軌跡の取得
//=============================================================================
D3DXMATRIX *GetOrbit(void)
{
	return &g_mtxWorldWeapon;
}

//=============================================================================
// 軌跡の手持ち側
//=============================================================================
D3DXMATRIX *GetHand(void)
{
	return &g_mtxWorldMeshOrbit;
}
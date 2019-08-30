//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "explosion.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EXPLOSION	(128)

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 move;										//移動量
	D3DXVECTOR3 rot;										//回転
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	int nCounterAnim;										//アニメーションカウンター
	int nPatternAnim;										//アニメーションNo
	bool bUse;												//使用しているかどうか
} EXPLOSION;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;			//頂点バッファへのポインタ
EXPLOSION				g_aExplosion[MAX_EXPLOSION];			//爆発

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// 初期化処理
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												// 位置・回転の初期設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/explosion000.png", &g_pTextureExplosion);

	MakeVertexExplosion(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{
	//テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	VERTEX_3D *pVtx;										//頂点情報へのポインタ
	int nCntExplosion;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//カウンターの加算
		g_aExplosion[nCntExplosion].nCounterAnim++;

		//爆発が使われている場合
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			if (g_aExplosion[nCntExplosion].nCounterAnim % 5 == 0)
			{
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;

				//テクスチャ座標の更新
				pVtx[0].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);

				//アニメーションパターンが一周したら
				if (g_aExplosion[nCntExplosion].nPatternAnim % 8 == 0)
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}
			}
		}
		pVtx += 4;										//頂点データのポイントを4つ分進める	
	}
	//頂点データのアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//計算用マトリックス

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aExplosion[nCntExplosion].mtxWorld._11 = mtxView._11;
			g_aExplosion[nCntExplosion].mtxWorld._12 = mtxView._21;
			g_aExplosion[nCntExplosion].mtxWorld._13 = mtxView._31;
			g_aExplosion[nCntExplosion].mtxWorld._21 = mtxView._12;
			g_aExplosion[nCntExplosion].mtxWorld._22 = mtxView._22;
			g_aExplosion[nCntExplosion].mtxWorld._23 = mtxView._32;
			g_aExplosion[nCntExplosion].mtxWorld._31 = mtxView._13;
			g_aExplosion[nCntExplosion].mtxWorld._32 = mtxView._23;
			g_aExplosion[nCntExplosion].mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aExplosion[nCntExplosion].rot.y, g_aExplosion[nCntExplosion].rot.x, g_aExplosion[nCntExplosion].rot.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntExplosion, 2);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, NULL);

		}
	}

	//////通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, -0.0f);
		pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f, -0.0f);

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

		//テクスチャ描写の位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.128f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.128f, 1.0f);

		pVtx += 4;
	}

	//頂点データのアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 爆発の作成
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (!g_aExplosion[nCntExplosion].bUse)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
	}
}
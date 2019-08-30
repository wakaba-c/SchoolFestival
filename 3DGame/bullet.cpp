//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "bullet.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BULLET	(128)


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;			//頂点バッファへのポインタ
BULLET					g_aBullet[MAX_BULLET];			//玉

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nIndexShadow = 0;
		g_aBullet[nCntBullet].nLife = 0;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/bullet000.png", &g_pTextureBullet);

	MakeVertexBullet(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			//玉の移動
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			g_aBullet[nCntBullet].nLife++;
			SetPositionShadow(g_aBullet[nCntBullet].nIndexShadow, g_aBullet[nCntBullet].pos);

			//エフェクトの設定
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 60, EFFECTTYPE_NORMAL);

			if (g_aBullet[nCntBullet].nLife > 100)
			{
				SetExplosion(g_aBullet[nCntBullet].pos);
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIndexShadow);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//計算用マトリックス

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 220);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCntBullet].rot.y, g_aBullet[nCntBullet].rot.x, g_aBullet[nCntBullet].rot.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, NULL);

		}
	}

	//通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_BULLET; nCntWall++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-3.0f, 3.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(3.0f, 3.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-3.0f, 0.0f, -0.0f);
		pVtx[3].pos = D3DXVECTOR3(3.0f, 0.0f, -0.0f);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ描写の位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;
	}

	//頂点データのアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 玉の作成
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot = rot;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].nLife = 0;
			g_aBullet[nCntBullet].nIndexShadow = SetShadow(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].rot, D3DXVECTOR3(2.0f, 0.0f, 2.0f));
			break;
		}
	}
}

//=============================================================================
// 弾の取得
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_aBullet[0];
}
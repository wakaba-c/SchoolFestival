//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EFFECT 3000										//影の最大値

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 rot;										//回転
	D3DXVECTOR3 size;										//サイズ
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	D3DXCOLOR col;											//カラー
	float fRadius;											//半径
	int nLife;												// ライフ
	bool bUse;												//使用しているかどうか
	bool bAuthorization;									//エフェクト側で編集することを許可するかどうか
} EFFECT;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//頂点バッファへのポインタ
EFFECT g_aEffect[MAX_EFFECT];								//影

//=============================================================================
// 初期化処理
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/shadow000.jpg", &g_pTextureEffect);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 位置・回転の初期設定
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].size = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].size.x, g_aEffect[nCntEffect].size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].size.x, g_aEffect[nCntEffect].size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].size.x, -g_aEffect[nCntEffect].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].size.x, -g_aEffect[nCntEffect].size.y, 0.0f);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ描写の位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点データのアンロック
	g_pVtxBuffEffect->Unlock();

	//MakeVertexPolygon(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	//テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;										//頂点情報へのポインタ
	float fRadius;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			
			fRadius = g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius -= fRadius;
			g_aEffect[nCntEffect].col.a -= 0.05f;

			if (g_aEffect[nCntEffect].bAuthorization)
			{
				g_aEffect[nCntEffect].pos.y++;
			}

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				//エフェクト消滅
				g_aEffect[nCntEffect].bUse = false;
				g_aEffect[nCntEffect].col.a = 5.0f;
			}

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nCntEffect].col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nCntEffect].col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nCntEffect].col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nCntEffect].col.a);
		}

		pVtx += 4;
	}

	// 頂点データをアンロックする
	g_pVtxBuffEffect->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//計算用マトリックス
	EFFECT *pEffect;							//影のポインタ


	// レンダーステート(加算合成処理)
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pEffect = &g_aEffect[0];

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, 2);

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
}

//=============================================================================
// 影の作成
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, bool bUse)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = rot;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = 15;
			g_aEffect[nCntEffect].nLife = 20;
			g_aEffect[nCntEffect].bUse = true;
			g_aEffect[nCntEffect].bAuthorization = bUse;
			break;
		}
	}
}
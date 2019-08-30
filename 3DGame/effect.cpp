// ----------------------------------------------------------------------------
//
// エフェクトの処理 [effect.cpp]
// Author : Ryouma Inoue
//
// ----------------------------------------------------------------------------
#include "effect.h"

// ----------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------
#define	TEXTURE_EFFECT				"data/tex/effect000.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_EFFECT_HEART		"data/TEXTURE/ui_heart.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_EFFECT_HIBANA		"data/TEXTURE/hibana.png"		// 読み込むテクスチャファイル名
#define	MAX_EFFECT			(4096)									// エフェクトの最大数
#define EFFECTSIZE_X		(20)		
#define EFFECTSIZE_Y		(20)

// ----------------------------------------------------------------------------
// プロトタイプ宣言
// ----------------------------------------------------------------------------
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

// ----------------------------------------------------------------------------
// グローバル変数
// ----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureEffect[EFFECTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
EFFECT					g_aEffect[MAX_EFFECT];		// エフェクト情報
float					g_fAngleadd;

// ----------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
		TEXTURE_EFFECT,							// ファイルの名前
		&g_pTextureEffect[EFFECTTYPE_NORMAL]);	// 読み込むメモリー
												// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_EFFECT_HEART,						// ファイルの名前
		&g_pTextureEffect[EFFECTTYPE_LIFE]);		// 読み込むメモリー

													// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_EFFECT_HIBANA,						// ファイルの名前
		&g_pTextureEffect[EFFECTTYPE_HIBANA]);		// 読み込むメモリー

													// 頂点情報の作成
	MakeVertexEffect(pDevice);
}

// ----------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		// テクスチャの開放
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

// ----------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;
	int nCntEffect;
	float fRadius;
	float fAngle;

	fAngle = float(rand() % 314);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			fRadius = g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].fRadius -= fRadius;
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			//g_aEffect[nCntEffect].col.a -= 5;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			if (g_aEffect[nCntEffect].pos <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			// 座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		}
		// 頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	// 頂点データをアンロックする
	g_pVtxBuffEffect->Unlock();
}

// ----------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;
	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 254);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].type]);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

// ----------------------------------------------------------------------------
// 頂点の作成
// ----------------------------------------------------------------------------
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;
	int nCntEffect;
	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報の設定
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pVtx[0].pos = D3DXVECTOR3(-20.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, 0.0f, 0.0f);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	// 頂点データをアンロックする
	g_pVtxBuffEffect->Unlock();
}

// ----------------------------------------------------------------------------
// エフェクト設定
// ----------------------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife, EFFECTTYPE type)
{
	int nCntEffect;
	VERTEX_3D *pVtx;				// 頂点情報へのポインタ
									// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].type = type;
			g_aEffect[nCntEffect].bUse = true; 
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
			break;
		}
		// 頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	// 頂点データをアンロック
	g_pVtxBuffEffect->Unlock();
}
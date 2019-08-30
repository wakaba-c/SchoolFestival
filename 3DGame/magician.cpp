//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "magician.h"
#include "meshorbit.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_MAGICIAN	(128)

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;										//位置
	D3DXCOLOR	col;										
	D3DXVECTOR3 move;										//移動量
	D3DXVECTOR3 rot;										//回転
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	int nCounterAnim;										//アニメーションカウンター
	int nPatternAnim;										//アニメーションNo
	bool bUse;												//使用しているかどうか
} MAGICIAN;

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMagician = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagician = NULL;			//頂点バッファへのポインタ
MAGICIAN				g_aMagician[MAX_MAGICIAN];			//爆発

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMagician(LPDIRECT3DDEVICE9 pDevice);


//=============================================================================
// 初期化処理
//=============================================================================
void InitMagician(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												// 位置・回転の初期設定
	for (int nCntMagician = 0; nCntMagician < MAX_MAGICIAN; nCntMagician++)
	{
		g_aMagician[nCntMagician].bUse = false;
		g_aMagician[nCntMagician].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagician[nCntMagician].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aMagician[nCntMagician].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagician[nCntMagician].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/Magician.png", &g_pTextureMagician);

	MakeVertexMagician(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagician(void)
{
	//テクスチャの開放
	if (g_pTextureMagician != NULL)
	{
		g_pTextureMagician->Release();
		g_pTextureMagician = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMagician != NULL)
	{
		g_pVtxBuffMagician->Release();
		g_pVtxBuffMagician = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagician(void)
{
	VERTEX_3D *pVtx;										//頂点情報へのポインタ
	D3DXMATRIX *pHand = GetOrbit();
	PLAYER *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagician->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMagician = 0; nCntMagician < MAX_MAGICIAN; nCntMagician++)
	{
		//カウンターの加算
		g_aMagician[nCntMagician].nCounterAnim++;

		//爆発が使われている場合
		if (g_aMagician[nCntMagician].bUse == true)
		{
			g_aMagician[nCntMagician].rot.y = pPlayer->rot.y + 0.5f;

			g_aMagician[nCntMagician].pos = D3DXVECTOR3(pHand->_41, pHand->_42, pHand->_43);

			
			g_aMagician[nCntMagician].col.a -= 0.1;

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aMagician[nCntMagician].col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aMagician[nCntMagician].col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aMagician[nCntMagician].col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aMagician[nCntMagician].col.a);
		}
		pVtx += 4;										//頂点データのポイントを4つ分進める	
	}
	//頂点データのアンロック
	g_pVtxBuffMagician->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagician(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//計算用マトリックス

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面(左回り)をカリングする

	for (int nCntMagician = 0; nCntMagician < MAX_MAGICIAN; nCntMagician++)
	{
		if (g_aMagician[nCntMagician].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMagician[nCntMagician].mtxWorld);

			//pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//g_aMagician[nCntMagician].mtxWorld._11 = mtxView._11;
			//g_aMagician[nCntMagician].mtxWorld._12 = mtxView._21;
			//g_aMagician[nCntMagician].mtxWorld._13 = mtxView._31;
			//g_aMagician[nCntMagician].mtxWorld._21 = mtxView._12;
			//g_aMagician[nCntMagician].mtxWorld._22 = mtxView._22;
			//g_aMagician[nCntMagician].mtxWorld._23 = mtxView._32;
			//g_aMagician[nCntMagician].mtxWorld._31 = mtxView._13;
			//g_aMagician[nCntMagician].mtxWorld._32 = mtxView._23;
			//g_aMagician[nCntMagician].mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagician[nCntMagician].rot.y, g_aMagician[nCntMagician].rot.x, g_aMagician[nCntMagician].rot.z);
			D3DXMatrixMultiply(&g_aMagician[nCntMagician].mtxWorld, &g_aMagician[nCntMagician].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMagician[nCntMagician].pos.x, g_aMagician[nCntMagician].pos.y, g_aMagician[nCntMagician].pos.z);
			D3DXMatrixMultiply(&g_aMagician[nCntMagician].mtxWorld, &g_aMagician[nCntMagician].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMagician[nCntMagician].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMagician, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureMagician);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMagician, 2);

			//頂点フォーマットの設定
			pDevice->SetTexture(0, NULL);

		}
	}

	//通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMagician(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAGICIAN, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMagician, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagician->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMagician = 0; nCntMagician < MAX_MAGICIAN; nCntMagician++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-30.0f, -30.0f, -0.0f);
		pVtx[3].pos = D3DXVECTOR3(30.0f, -30.0f, -0.0f);

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
	g_pVtxBuffMagician->Unlock();
}

//=============================================================================
// 爆発の作成
//=============================================================================
void SetMagician(D3DXVECTOR3 pos)
{
	for (int nCntExplosion = 0; nCntExplosion < MAX_MAGICIAN; nCntExplosion++)
	{
		if (!g_aMagician[nCntExplosion].bUse)
		{
			g_aMagician[nCntExplosion].pos = pos;
			g_aMagician[nCntExplosion].bUse = true;
			break;
		}
	}
}
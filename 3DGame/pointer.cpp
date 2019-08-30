//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "pointer.h"
#include "mouse.h"
#include "camera.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EXPLOSION	(128)

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTexturePointer = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPointer = NULL;			//頂点バッファへのポインタ
POINTER				g_pointer;			//爆発

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexPointer(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// 初期化処理
//=============================================================================
void InitPointer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
		g_pointer.bUse = true;
		g_pointer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_pointer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_pointer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/tex/Aim.png", &g_pTexturePointer);

	MakeVertexPointer(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPointer(void)
{
	//テクスチャの開放
	if (g_pTexturePointer != NULL)
	{
		g_pTexturePointer->Release();
		g_pTexturePointer = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPointer != NULL)
	{
		g_pVtxBuffPointer->Release();
		g_pVtxBuffPointer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePointer(void)
{
	CAMERA *pCamera = GetCamera();
	VERTEX_3D *pVtx;										//頂点情報へのポインタ

	float fPosX = 0;
	float fPosY = 0;
	LONG pMouseX = 0;
	LONG pMouseY = 0;
	pMouseX = GetMouseX();
	pMouseY = GetMouseY();

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);

	//爆発が使われている場合
	if (g_pointer.bUse == true)
	{
		g_pointer.pos.x = (float)pMouseX + pCamera->posR.x;
		g_pointer.pos.y = (float)pMouseY + pCamera->posR.y;
		g_pointer.pos.z = 0;
	}
	//頂点データのアンロック
	g_pVtxBuffPointer->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPointer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//計算用マトリックス

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_pointer.mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	g_pointer.mtxWorld._11 = mtxView._11;
	g_pointer.mtxWorld._12 = mtxView._21;
	g_pointer.mtxWorld._13 = mtxView._31;
	g_pointer.mtxWorld._21 = mtxView._12;
	g_pointer.mtxWorld._22 = mtxView._22;
	g_pointer.mtxWorld._23 = mtxView._32;
	g_pointer.mtxWorld._31 = mtxView._13;
	g_pointer.mtxWorld._32 = mtxView._23;
	g_pointer.mtxWorld._33 = mtxView._33;

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_pointer.rot.y, g_pointer.rot.x, g_pointer.rot.z);
	D3DXMatrixMultiply(&g_pointer.mtxWorld, &g_pointer.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_pointer.pos.x, g_pointer.pos.y, g_pointer.pos.z);
	D3DXMatrixMultiply(&g_pointer.mtxWorld, &g_pointer.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_pointer.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPointer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturePointer);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);

	//通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPointer(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPointer, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-25.0f, 25.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(25.0f, 25.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-25.0f, -25.0f, -0.0f);
	pVtx[3].pos = D3DXVECTOR3(25.0f, -25.0f, -0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データのアンロック
	g_pVtxBuffPointer->Unlock();
}

//=============================================================================
// 情報取得
//=============================================================================
POINTER *GetPointer(void)
{
	return &g_pointer;
}
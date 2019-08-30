//========================================================================================
//
// 基本[result.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "input.h"
#include "fade.h"
#include "result.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "shadow.h"
#include "controller.h"
#include "sound.h"
#include "tutorial.h"

//========================================================================================
// マクロ定義
//========================================================================================
#define TEXTURE_ADD "data/tex/bg003.png"
#define TEXTURE_BG	"data/tex/gameover_bg.png"
#define TEXTURE_CLEAR "data/tex/gameclear.png"
#define TEXTURE_FAILD "data/tex/gameover.png"
#define MAX_RESULT 2

//========================================================================================
// グローバル変数
//========================================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[5] = {};						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;				//頂点バッファへのポインタ

RESULTSTATE g_resultState;
float g_fMoveResultPos;
//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	SetTutorial();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG, &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_CLEAR, &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FAILD, &g_pTextureResult[3]);
	D3DXCreateTextureFromFile(pDevice, "data/tex/bg003", &g_pTextureResult[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	SetVertexResult(4, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), SCREEN_WIDTH, SCREEN_HEIGHT);

	SetVertexResult(1, D3DXVECTOR3(640, 200, 0), 600, 200);

	// 影の初期化処理
	InitShadow();

	// フィールドの初期化処理
	InitMeshField();

	PlaySound(SOUND_LABEL_SE_GAMEOVER);
}

//========================================================================================
// ポリゴンの開放処理
//========================================================================================
void UninitResult(void)
{
	int nCntTex;

	//フィールドの終了処理
	UninitMeshField();

	// 影の終了処理
	UninitShadow();

	//テクスチャの開放
	for (nCntTex = 0; nCntTex < 5; nCntTex++)
	{
		if (g_pTextureResult[nCntTex] != NULL)
		{
			g_pTextureResult[nCntTex]->Release();
			g_pTextureResult[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//========================================================================================
// ポリゴンの更新処理
//========================================================================================
void UpdateResult(void)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//フィールドの更新処理
	UpdateMeshField();

	//影の更新処理
	UpdateShadow();

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + g_fMoveResultPos);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + g_fMoveResultPos);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + g_fMoveResultPos);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f + g_fMoveResultPos);

	//頂点データのアンロック
	g_pVtxBuffResult->Unlock();

	// 画面遷移
	if (GetTriggerKeyboard(DIK_RETURN) || GetControllerTrigger(0, JOYPADKEY_START))
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_RANKING);
		}
	}
}

//========================================================================================
// ポリゴンの描画処理
//========================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCount;

	////フィールドの描画処理
	//DrawMeshField();

	// 影の描画処理
	DrawShadow();

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCount = 0; nCount < MAX_RESULT; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCount * 2 + (g_resultState == RESULTSTATE_CLEAR ? 0 : 1)]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexResult(int index, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

															//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += index * 4;

	// 頂点情報の設定
	//頂点座標の設定(基準のx座標 + 間隔 * nCntScore (+ 幅), 基準のy座標)
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth / 2, pos.y - fHeight / 2, 0);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth / 2, pos.y - fHeight / 2, 0);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth / 2, pos.y + fHeight / 2, 0);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth / 2, pos.y + fHeight / 2, 0);

	//1.0で固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	if (index == 4)
	{
		//カラーチャートの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	}
	else
	{
		//カラーチャートの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャ描写の位置
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データのアンロック
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// 結果の設定
//=============================================================================
void SetResultState(RESULTSTATE state)
{
	g_resultState = state;
}
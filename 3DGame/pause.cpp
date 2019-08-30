//========================================================================================
//
// 基本[game_bg.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "pause.h"
#include "input.h"
#include "game.h"

//========================================================================================
// マクロ定義
//========================================================================================
#define TEXTURE_PAUSE_ADD	"data/tex/bg003"
#define TEXTURE_CONTINUE	"data/tex/continu.png"
#define TEXTURE_TITLE		"data/tex/title_go.png"
#define TEXTURE_PAUSE		"data/tex/pause.png"
#define MAX_PAUSE 4

//========================================================================================
// グローバル変数
//========================================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PAUSE] = {};						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//頂点バッファへのポインタ

bool g_bPause;
int g_nSelect;
GAMESTATE g_pauseMode;
D3DXVECTOR3 g_Selectpos;
//float fMove_Pos;

//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	g_bPause = false;
	g_Selectpos = D3DXVECTOR3(640, 320, 0);
	g_nSelect = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE_ADD, &g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE, &g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_CONTINUE, &g_pTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &g_pTexturePause[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	SetVertexPause(0, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),D3DXCOLOR(0.0f,0.0f,0.0f,0.5f), SCREEN_WIDTH, SCREEN_HEIGHT);

	SetVertexPause(1, D3DXVECTOR3(640, 150, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500, 100);

	SetVertexPause(2, D3DXVECTOR3(640, 290, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 100);

	SetVertexPause(3, D3DXVECTOR3(640, 450, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 100);
}

//========================================================================================
// ポリゴンの開放処理
//========================================================================================
void UninitPause(void)
{
	int nCntTex;

	//テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_PAUSE; nCntTex++)
	{
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//========================================================================================
// ポリゴンの更新処理
//========================================================================================
void UpdatePause(void)
{
	//選択肢処理
	if (GetTriggerKeyboard(DIK_UPARROW))
	{
		g_nSelect -= 1;
	}
	if (GetTriggerKeyboard(DIK_DOWNARROW))
	{
		g_nSelect += 1;
	}

	//選択肢の繰り返し処理
	if (g_nSelect > 1)
	{
		g_nSelect = 0;
	}
	else if (g_nSelect < 0)
	{
		g_nSelect = 1;
	}

	SetVertexPause(2, D3DXVECTOR3(640, 320, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250, 50);
	SetVertexPause(3, D3DXVECTOR3(640, 470, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250, 50);

	//選択肢の表示処理
	if (g_nSelect == 0)
	{
		g_Selectpos.y = 320;
		SetVertexPause(2, g_Selectpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500, 100);
	}
	else if (g_nSelect == 1)
	{
		g_Selectpos.y = 470;
		SetVertexPause(3, g_Selectpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500, 100);
	}

	if (GetTriggerKeyboard(DIK_RETURN))
	{
		if (g_nSelect == 0)
		{
			SetPause(false);
			SetGameState(GetPauseMode());
		}
		else if (g_nSelect == 1)
		{
			SetGameState(GAMESTATE_BREAK);
		}
	}
}

//========================================================================================
// ポリゴンの描画処理
//========================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCount;

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bPause == true)
	{
		for (nCount = 0; nCount < MAX_PAUSE; nCount++)
		{
			//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTexturePause[nCount]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//========================================================================================
// ポーズ処理
//========================================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}

//========================================================================================
// ポーズ前のモード取得
//========================================================================================
void SetPauseMode(GAMESTATE mode)
{
	g_pauseMode = mode;
}

//========================================================================================
// ポーズ前のモード取得
//========================================================================================
GAMESTATE GetPauseMode(void)
{
	return g_pauseMode;
}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexPause(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

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

	//カラーチャートの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//テクスチャ描写の位置
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データのアンロック
	g_pVtxBuffPause->Unlock();
}
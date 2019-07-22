//========================================================================================
//
// タイトル処理[title.cpp]
//
// Author : masayasu wakita
// 07/13
//
//========================================================================================
#include "title.h"
#include "fade.h"
#include "game.h"
#include "meshField.h"
#include "camera.h"
#include "light.h"
#include "wall.h"
#include "shadow.h"
#include "gradation.h"
#include "input.h"
#include "controller.h"
#include "sound.h"

//========================================================================================
// マクロ定義
//========================================================================================
#define TEXTURE_TITLE_ADD "data/tex/flower.png"
#define TEXTURE_PRESS_ADD "data/tex/press enter.png"
#define TEXTURE_START_ADD "data/tex/game start.png"
#define TEXTURE_SELECT_ADD	"data/tex/select002.png"
#define TEXTURE_RANKING_ADD "data/tex/ranking.png"
#define RANKING_TIME 500
#define MAX_TITLE 4

//========================================================================================
// グローバル変数
//========================================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;							//頂点バッファへのポインタ

D3DXVECTOR3 g_pos(640, 200, 0);											//タイトルのポジション
D3DXCOLOR g_col(1.0f, 1.0f, 1.0f, 0.0f);
D3DXCOLOR g_logo;
int g_nRanking;
int g_nMode;
D3DXVECTOR3 g_Modepos;
bool g_bPressEnter;
bool g_bSelect;
float fMove_Title_Pos;

//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	g_nRanking = 0;

	g_Modepos = D3DXVECTOR3(640, 320, 0);
	g_nMode = 0;
	g_bSelect = false;
	g_bPressEnter = false;
	g_logo = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//グラデーションの初期化処理
	InitGradation();

	//影の初期化処理
	InitShadow();

	//フィールドの初期化処理
	InitMeshField();

	// 壁の初期化処理
	InitWall();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_ADD, &g_pTextureTitle[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PRESS_ADD, &g_pTextureTitle[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_START_ADD, &g_pTextureTitle[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_ADD, &g_pTextureTitle[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE + 1, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);


	SetVertexTitle(0, D3DXVECTOR3(g_pos), g_col, 800, 200);

	SetVertexTitle(1, D3DXVECTOR3(640, 600, 0), g_logo, 400, 100);

	SetVertexTitle(2, D3DXVECTOR3(640, 600, 0), g_logo, 400, 100);

	SetVertexTitle(3, D3DXVECTOR3(640, 600, 0), g_logo, 400, 100);

	PlaySound(SOUND_LABEL_BGM000, false);
}

//========================================================================================
// ポリゴンの開放処理
//========================================================================================
void UninitTitle(void)
{
	int nCntTex;

	//グラデーションの終了処理
	UninitGradation();

	//フィールドの終了処理
	UninitMeshField();

	//影の終了処理
	UninitShadow();

	//テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TITLE; nCntTex++)
	{
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}
	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//========================================================================================
// ポリゴンの更新処理
//========================================================================================
void UpdateTitle(void)
{
	float fAlpha = 1.0;
	int nCount = 0;

	//フィールドの更新処理
	UpdateMeshField();

	// 壁の更新処理
	UpdateWall();

	g_nRanking++;

	//タイトルからランキングへの自動遷移
	if (g_bPressEnter == false)
	{
		if (g_nRanking >= RANKING_TIME)
		{
			if (GetFade() == FADE_NONE)
			{
				SetFade(MODE_RANKING);
			}
		}
	}

	//タイトルが完全に表示されたときのpressエンターの処理
	if (g_col.a > 1.0f && g_bPressEnter == false)
	{
		g_logo.a += 0.1f;

		SetVertexTitle(0, D3DXVECTOR3(g_pos), g_col, 800, 200);
		SetVertexTitle(1, D3DXVECTOR3(640, 600, 0), g_logo, 400, 100);
	}
	else
	{
		g_col.a += 0.01f;
		SetVertexTitle(0, D3DXVECTOR3(g_pos), g_col, 800, 200);
	}

	//pressenterの削除
	if (g_pos.y >= 200 && g_bPressEnter == true)
	{
		SetVertexTitle(1, D3DXVECTOR3(640, 600, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 400, 100);

		SetVertexTitle(2, D3DXVECTOR3(640, 450, 0), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 250, 50);

		SetVertexTitle(3, D3DXVECTOR3(640, 600, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250, 50);
	}

	//どのモードにするかを決める処理
	if (g_bPressEnter == true)
	{
		//選択肢処理
		if (GetTriggerKeyboard(DIK_UPARROW) || GetControllerTrigger(0, JOYPADKEY_UP))
		{
			g_nMode -= 1;
		}
		if (GetTriggerKeyboard(DIK_DOWNARROW) || GetControllerTrigger(0, JOYPADKEY_DOWN))
		{
			g_nMode += 1;
		}

		//選択肢の繰り返し処理
		if (g_nMode > 1)
		{
			g_nMode = 0;
		}
		else if (g_nMode < 0)
		{
			g_nMode = 1;
		}

		//選択肢の表示処理
		if (g_nMode == 0)
		{
			//ゲーム開始
			g_Modepos.y = 470;
			SetVertexTitle(2, g_Modepos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500, 100);
		}
		else if (g_nMode == 1)
		{
			//ランキング
			g_Modepos.y = 620;
			SetVertexTitle(3, g_Modepos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500, 100);
		}
	}

	if (GetTriggerKeyboard(DIK_RETURN) || GetControllerTrigger(0, JOYPADKEY_B))
	{
		if (g_col.a > 1.0f && g_bPressEnter == true)
		{
			//PlaySound(SOUND_LABEL_SE_SELECT);
			if (GetFade() == FADE_NONE)
			{
				if (g_nMode == 0)
				{
					g_bSelect = true;
					SetFade(MODE_GAME);
				}
				else if (g_nMode == 1)
				{
					SetFade(MODE_RANKING);
				}
			}
		}
		//タイトルが指定した場所にいた場合
		else if (g_col.a >= 1.0f)
		{
			g_bPressEnter = true;
		}
		else
		{
			g_col.a = 1.0f;
		}
	}
}

//========================================================================================
// ポリゴンの描画処理
//========================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCount;

	//　フィールドの描画処理
	DrawMeshField();

	// 壁の描画処理
	DrawWall();

	//影の描画処理
	DrawShadow();

	//グラデーションの描画処理
	DrawGradation();

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCount = 0; nCount < MAX_TITLE; nCount++)
	{
		pDevice->SetTexture(0, g_pTextureTitle[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexTitle(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// ゲーム開始(チュートリアルカット版)
//=============================================================================
bool GetState(void)
{
	return g_bSelect;
}
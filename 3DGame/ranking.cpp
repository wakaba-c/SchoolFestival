//========================================================================================
//
// ランキング[Ranking[.cpp]
// Author : masayasu wakita
//
//========================================================================================
#include "ranking.h"
#include "fade.h"
#include "time.h"
#include "wall.h"
#include "meshField.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "gradation.h"
#include "input.h"
#include "controller.h"

//========================================================================================
// マクロ定義
//========================================================================================
#define TEXTURE_BG_ADD			"data/tex/nc40567.png"		//背景のテクスチャ
#define TEXTURE_RANKING_ADD		"data/tex/ranking.png"			//「ranking」のテクスチャ
#define TEXTURE_NUMBER_ADD		"data/tex/number3D.png"			//数字のテクスチャ
#define TEXTFILE_RANKING		"data/RANKING.txt"		//ランキングのデータ読み込み
#define MAX_RANKINGTEX 3										//ランキングテクスチャの最大数
#define MAX_DIGIT 8
#define MAX_RANK 6
#define MAX_PREFAB 45

//========================================================================================
// グローバル変数
//========================================================================================
LPDIRECT3DTEXTURE9 g_pTextureRanking[MAX_RANKINGTEX] = {};						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;								//頂点バッファへのポインタ
RANKING g_rank[5];
int g_nNum;
float fMove_Ranking_Pos;
bool g_bInit;
int g_nTitle;
D3DXCOLOR g_aColor[5] = { D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f,1.0f,0.0f,1.0f), 
	D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) };

//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCount;
	g_nTitle = 0;
	
	// フィールドの初期化処理
	InitMeshField();

	// 壁の初期化処理
	InitWall();
	
	// 影の初期化処理
	InitShadow();

	//グラデーションの初期化処理
	InitGradation();

	g_bInit = false;

	for (nCount = 0; nCount < 5; nCount++)
	{
		g_rank[nCount].pos = D3DXVECTOR3(0, 0, 0);
		g_rank[nCount].col = D3DXCOLOR(1, 1, 1, 1);
		g_rank[nCount].nScore = 0;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG_ADD, &g_pTextureRanking[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_ADD, &g_pTextureRanking[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NUMBER_ADD, &g_pTextureRanking[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_RANKINGTEX + MAX_PREFAB), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL);

	//SetVertexRanking(0, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	SetVertexRanking(1, D3DXVECTOR3(640, 100, 0), 350, 100, 0);

	SetVertexRanking(2, D3DXVECTOR3(300, 220, 0), 120, 100, 1.09f);

	SetVertexRanking(3, D3DXVECTOR3(300, 320, 0), 120, 100, 2);

	SetVertexRanking(4, D3DXVECTOR3(300, 420, 0), 120, 100, 3);

	SetVertexRanking(5, D3DXVECTOR3(300, 530, 0), 120, 100, 4);

	SetVertexRanking(6, D3DXVECTOR3(300, 630, 0), 120, 100, 5);

	LoadRanking();

	ConvertRanking(g_nNum);

	SaveRanking();

	// 頂点情報の作成
	MakeVertexRanking(RANKINGSTATE_1, D3DXVECTOR3(400, 400, 0), D3DXCOLOR(1.0f, 0, 0, 1.0f));
}

//========================================================================================
// ポリゴンの開放処理
//========================================================================================
void UninitRanking(void)
{
	int nCntTex;

	// フィールドの終了処理
	UninitMeshField();

	// 壁の終了処理
	UninitWall();

	// 影の終了処理
	UninitShadow();

	//グラデーションの終了処理
	UninitGradation();

	//テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_RANKINGTEX; nCntTex++)
	{
		if (g_pTextureRanking[nCntTex] != NULL)
		{
			g_pTextureRanking[nCntTex]->Release();
			g_pTextureRanking[nCntTex] = NULL;
		}
	}
	//頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//========================================================================================
// ポリゴンの更新処理
//========================================================================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ
	fMove_Ranking_Pos -= 0.01f;
	float fAlpha = 1.0;
	int nCount = 0;

	// フィールドの更新処理
	UpdateMeshField();

	// 壁の更新処理
	UpdateWall();

	// 影の更新処理
	UpdateShadow();

	// グラデーションの更新処理
	UpdateGradation();

	g_nTitle++;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nTitle > 1000)
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE);
		}
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + fMove_Ranking_Pos);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + fMove_Ranking_Pos);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + fMove_Ranking_Pos);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f + fMove_Ranking_Pos);

	//頂点データのアンロック
	g_pVtxBuffRanking->Unlock();

	if (GetTriggerKeyboard(DIK_RETURN) || GetControllerTrigger(0, JOYPADKEY_B))
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE);
		}
	}
}

//========================================================================================
// ポリゴンの描画処理
//========================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCount;

	// フィールドの描画処理
	DrawMeshField();

	// 壁の描画処理
	DrawWall();

	// 影の描画処理
	DrawShadow();

	//グラデーションの描画処理
	DrawGradation();

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCount = 0; nCount < (MAX_RANKINGTEX + MAX_PREFAB); nCount++)
	{
		if (nCount < MAX_RANKINGTEX)
		{
			pDevice->SetTexture(0, g_pTextureRanking[nCount]);
		}

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}

//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexRanking(int index, D3DXVECTOR3 pos, float fWidth, float fHeight, float nNumber)
{
	VERTEX_2D *pVtx;										//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (index < 2)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	else
	{
		//テクスチャ描写の位置
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);

	}

	//頂点データのアンロック
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexRanking(RANKINGSTATE rank, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;
	int nCntScore;
	int nCntRank;
	int nNumber;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	g_rank[rank].pos = pos;
	g_rank[rank].col = col;

	pVtx += (4 * 7);

	// 頂点情報の設定
	for (nCntRank = 0; nCntRank < 5; nCntRank++)
	{
		for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			//頂点座標の設定(基準のx座標 + 間隔 * nCntScore (+ 幅), 基準のy座標)
			pVtx[0].pos = D3DXVECTOR3(400.0f + 85 * nCntScore, 155.0f + nCntRank * 100, 0);
			pVtx[1].pos = D3DXVECTOR3(400.0f + 85 * nCntScore + 70.0f, 155.0f + nCntRank * 100, 0);
			pVtx[2].pos = D3DXVECTOR3(400.0f + 85 * nCntScore, (155.0f + 110.0f) + nCntRank * 100, 0);
			pVtx[3].pos = D3DXVECTOR3(400.0f + 85 * nCntScore + 70.0f, (155.0f + 110.0f) + nCntRank * 100, 0);

			//1.0で固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//カラーチャートの設定
			pVtx[0].col = g_aColor[nCntRank];
			pVtx[1].col = g_aColor[nCntRank];
			pVtx[2].col = g_aColor[nCntRank];
			pVtx[3].col = g_aColor[nCntRank];

			nNumber = (int)g_rank[nCntRank].nScore % (int)pow(10, 8 - nCntScore) / (int)pow(10, 8 - 1 - nCntScore);

			pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);

			pVtx += 4;
		}
	}
	// 頂点データをアンロックする
	g_pVtxBuffRanking->Unlock();
}
//=============================================================================
// 結果の整理
//============================================================================
void ConvertRanking(int nNumber)
{
	int nCount, nCountA;
	int nNum01;
	g_rank[RANKINGSTATE_6].nScore = nNumber;

	//データのソートを実行
	for (nCount = 0; nCount < MAX_RANK; nCount++)
	{
		for (nCountA = 0; nCountA < MAX_RANK - nCount; nCountA++)
		{
			if (g_rank[nCountA].nScore < g_rank[nCountA + 1].nScore)
			{
				nNum01 = g_rank[nCountA].nScore;				//小さかった数字を入れ替える
				g_rank[nCountA].nScore = g_rank[nCountA + 1].nScore;	//大きかった数字を格納
				g_rank[nCountA + 1].nScore = nNum01;			//小さい数字を入れる
			}
		}
	}
}

//=============================================================================
// 結果の保存
//=============================================================================
void SaveRanking(void)
{
	FILE *pFile;
	int nCount;

	pFile = fopen(TEXTFILE_RANKING, "w");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < 5; nCount++)
		{
			//データの保存
			fprintf(pFile, "第%d位\n", nCount + 1);
			fprintf(pFile, "%d\n", g_rank[nCount].nScore);
		}
		fclose(pFile);
	}
}

//=============================================================================
// 結果のロード
//=============================================================================
void LoadRanking(void)
{
	FILE *pFile;
	int nCount;
	char x[128];

	pFile = fopen(TEXTFILE_RANKING, "r");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < 5; nCount++)
		{
			//データのロード
			fscanf(pFile, "%s", &x[0]);
			fscanf(pFile, "%d", &g_rank[nCount].nScore);
		}
		fclose(pFile);
	}
}


//=============================================================================
// 結果の設定
//=============================================================================
void SetResultIndex(int nScore)
{
	g_nNum = nScore;
}
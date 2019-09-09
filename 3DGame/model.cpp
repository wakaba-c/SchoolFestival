//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "shadow.h"
#include "result.h"
#include "effect.h"
#include "tutorial.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ITEM 128						//アイテムの最大数

//通常の配置情報
#define	NORMAL_WEDTH	6					//通常配置の最大数
#define	NORMAL_DEPTH	4					//通常配置の最大数

//ウォールの配置情報
#define WALL_WEDTH		12					//ウォールの最大数
#define WALL_DEPTH		4					//ウォールの高さ

//サークルの配置情報
#define CIRCLE_MAX		4					//サークルの最大数
//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	LPDIRECT3DTEXTURE9		pTextureModel = NULL;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffModel = NULL;		//頂点バッファへのポインタ
	LPD3DXMESH		pMeshModel = NULL;				//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatModel = NULL;				//マテリアル情報へのポインタ
	DWORD			nNumMatModel = 0;					//マテリアル情報の数
	D3DXVECTOR3		vtxMinModel;						//最小
	D3DXVECTOR3		vtxMaxModel;						//最大
} MODEL;

typedef struct
{
	D3DXVECTOR3		posModel;							//モデルの位置
	D3DXVECTOR3		moveModel;							//モデルの移動量
	D3DXVECTOR3		rotModel;							//モデルの向き(回転)
	D3DXVECTOR3		rotDestModel;						//モデルの最大回転
	D3DXMATRIX		mtxWorldModel;						//モデルのワールドマトリックス
	int nIdx;											//影のID
	int nCntMax;
	ITEMTYPE type;											//アイテムのタイプ
	bool bUse;											//使用されているかどうか
} ITEM;

//=============================================================================
// グローバル変数
//=============================================================================
MODEL g_aModel[ITEMTYPE_MAX];
ITEM g_aItem[MAX_ITEM];

int g_nLayout;

//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//======== 初期化 ========//
	bool bFlip = false;
	g_nLayout = 0;

	for (int nCntReset = 0; nCntReset < MAX_ITEM; nCntReset++)
	{
		// 位置・向きの初期設定
		g_aItem[nCntReset].posModel = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		g_aItem[nCntReset].moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntReset].rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/cloud1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aModel[ITEMTYPE_CLOUD_1].pBuffMatModel,
		NULL,
		&g_aModel[ITEMTYPE_CLOUD_1].nNumMatModel,
		&g_aModel[ITEMTYPE_CLOUD_1].pMeshModel);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/cloud4.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aModel[ITEMTYPE_CLOUD_2].pBuffMatModel,
		NULL,
		&g_aModel[ITEMTYPE_CLOUD_2].nNumMatModel,
		&g_aModel[ITEMTYPE_CLOUD_2].pMeshModel);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/cloud3.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aModel[ITEMTYPE_CLOUD_3].pBuffMatModel,
		NULL,
		&g_aModel[ITEMTYPE_CLOUD_3].nNumMatModel,
		&g_aModel[ITEMTYPE_CLOUD_3].pMeshModel);

	//モデルの頂点座標の最大・最小の設定
	int nNumVertices;			//頂点数
	DWORD sizeFVF;				//頂点フォーマット
	BYTE	*pVertexBuffer;		//頂点バッファへのポインタ

	for (int nCntType = 0; nCntType < ITEMTYPE_MAX; nCntType++)
	{
		//最小・最大のリセット
		g_aModel[nCntType].vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
		g_aModel[nCntType].vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

		//頂点数を取得
		nNumVertices = g_aModel[nCntType].pMeshModel->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntType].pMeshModel->GetFVF());

		//頂点バッファをロック
		g_aModel[nCntType].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
		{
			D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

			//すべての頂点情報を比較して最小最大を抜き出す
			if (g_aModel[nCntType].vtxMinModel.x > vtx.x)
			{
				g_aModel[nCntType].vtxMinModel.x = vtx.x;
			}
			if (g_aModel[nCntType].vtxMinModel.y > vtx.y)
			{
				g_aModel[nCntType].vtxMinModel.y = vtx.y;
			}
			if (g_aModel[nCntType].vtxMinModel.z > vtx.z)
			{
				g_aModel[nCntType].vtxMinModel.z = vtx.z;
			}

			//最小値を求める
			if (g_aModel[nCntType].vtxMaxModel.x < vtx.x)
			{
				g_aModel[nCntType].vtxMaxModel.x = vtx.x;
			}
			if (g_aModel[nCntType].vtxMaxModel.y < vtx.y)
			{
				g_aModel[nCntType].vtxMaxModel.y = vtx.y;
			}
			if (g_aModel[nCntType].vtxMaxModel.z < vtx.z)
			{
				g_aModel[nCntType].vtxMaxModel.z = vtx.z;
			}

			pVertexBuffer += sizeFVF;			//サイズ分のポインタを進める
		}

		//頂点バッファをアンロック
		g_aModel[nCntType].pMeshModel->UnlockVertexBuffer();
	}
	SetStage(0);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < ITEMTYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_aModel[nCntModel].pMeshModel != NULL)
		{
			g_aModel[nCntModel].pMeshModel->Release();
			g_aModel[nCntModel].pMeshModel = NULL;
		}

		// マテリアルの開放
		if (g_aModel[nCntModel].pBuffMatModel != NULL)
		{
			g_aModel[nCntModel].pBuffMatModel->Release();
			g_aModel[nCntModel].pBuffMatModel = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	//if (GetMode() == MODE_GAME)
	//{
	//	for (int nCntModel = 0; nCntModel < MAX_ITEM; nCntModel++)
	//	{
	//		if (g_aItem[nCntModel].bUse)
	//		{
	//			switch (g_aItem[nCntModel].type)
	//			{
	//			case ITEMTYPE_CLOUD_3:
	//				SetEffect(g_aItem[nCntModel].posModel, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_aItem[nCntModel].rotModel, true);
	//				break;
	//			}
	//		}
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用
	D3DMATERIAL9	matDef;							//マテリアルデータへのポインタ
	
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorldModel);

			// スケールを反映


			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rotModel.y, g_aItem[nCntItem].rotModel.x, g_aItem[nCntItem].rotModel.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorldModel, &g_aItem[nCntItem].mtxWorldModel, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].posModel.x, g_aItem[nCntItem].posModel.y, g_aItem[nCntItem].posModel.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorldModel, &g_aItem[nCntItem].mtxWorldModel, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorldModel);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aModel[g_aItem[nCntItem].type].pBuffMatModel->GetBufferPointer();


			for (int nCntMat = 0; nCntMat < (int)g_aModel[g_aItem[nCntItem].type].nNumMatModel; nCntMat++)
			{
				
				//// マテリアルの設定
				//pDevice->SetMaterial(&pMat[nCntMat].pTextureFilename);

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// 描画
				g_aModel[g_aItem[nCntItem].type].pMeshModel->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// モデルの当たり判定
//=============================================================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bRand = false;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			//上下判定
			if ((pPosOld->y >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.y + g_aItem[nCntItem].posModel.y) &&
				(pPosOld->y <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.y + g_aItem[nCntItem].posModel.y))
			{
				//左右判定
				if ((pPosOld->x >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.x + g_aItem[nCntItem].posModel.x) &&
					(pPosOld->x <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.x + g_aItem[nCntItem].posModel.x))
				{
					//プレイヤー判定
					if ((pPos->z >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.z + g_aItem[nCntItem].posModel.z) &&
						(pPosOld->z <= g_aModel[g_aItem[nCntItem].type].vtxMinModel.z + g_aItem[nCntItem].posModel.z))
					{
						pPos->z = g_aModel[g_aItem[nCntItem].type].vtxMinModel.z + g_aItem[nCntItem].posModel.z;
						pMove->z = 0.0f;
						////アイテムの当たり判定
						//if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_3)
						//{
						//	g_aItem[nCntItem].bUse = false;
						//	//DeleteShadow(g_aItem[nCntItem].nIdx);
						//	SetResultState(RESULTSTATE_CLEAR);
						//	SetGameState(GAMESTATE_END);
						//}
					}
					else if ((pPos->z <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.z + g_aItem[nCntItem].posModel.z) &&
						(pPosOld->z >= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.z + g_aItem[nCntItem].posModel.z))
					{
						pPos->z = g_aModel[g_aItem[nCntItem].type].vtxMaxModel.z + g_aItem[nCntItem].posModel.z;
						pMove->z = 0.0f;

						////アイテムの当たり判定
						//if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_3)
						//{
						//	g_aItem[nCntItem].bUse = false;
						//	//DeleteShadow(g_aItem[nCntItem].nIdx);
						//	SetResultState(RESULTSTATE_CLEAR);
						//	SetGameState(GAMESTATE_END);
						//}
					}
				}
			}
			//左右判定
			if ((pPosOld->x >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.x + g_aItem[nCntItem].posModel.x) &&
				(pPosOld->x <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.x + g_aItem[nCntItem].posModel.x))
			{
				//左右判定
				if ((pPosOld->z >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.z + g_aItem[nCntItem].posModel.z) &&
					(pPosOld->z <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.z + g_aItem[nCntItem].posModel.z))
				{
					//プレイヤーの判定
					if ((pPos->y >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.y + g_aItem[nCntItem].posModel.y) &&
						(pPosOld->y <= g_aModel[g_aItem[nCntItem].type].vtxMinModel.y + g_aItem[nCntItem].posModel.y))
					{
						pPos->y = g_aModel[g_aItem[nCntItem].type].vtxMinModel.y + g_aItem[nCntItem].posModel.y;
					}
					else if (pPos->y <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.y + g_aItem[nCntItem].posModel.y &&
						(pPosOld->y >= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.y + g_aItem[nCntItem].posModel.y))
					{
						pPos->y = g_aModel[g_aItem[nCntItem].type].vtxMaxModel.y + g_aItem[nCntItem].posModel.y;
						pMove->y = 0.0f;
						bRand = true;
					}
				}
			}
			//上下判定
			if ((pPosOld->y >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.y + g_aItem[nCntItem].posModel.y) &&
				(pPosOld->y <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.y + g_aItem[nCntItem].posModel.y))
			{
				//左右判定
				if ((pPosOld->z >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.z + g_aItem[nCntItem].posModel.z) &&
					(pPosOld->z <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.z + g_aItem[nCntItem].posModel.z))
				{
					if (pPos->x >= g_aModel[g_aItem[nCntItem].type].vtxMinModel.x + g_aItem[nCntItem].posModel.x &&
						(pPosOld->x <= g_aModel[g_aItem[nCntItem].type].vtxMinModel.x + g_aItem[nCntItem].posModel.x))
					{
						pPos->x = g_aModel[g_aItem[nCntItem].type].vtxMinModel.x + g_aItem[nCntItem].posModel.x;
						pMove->x = 0.0f;

						/*if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_3)
						{
							g_aItem[nCntItem].bUse = false;
							DeleteShadow(g_aItem[nCntItem].nIdx);
							SetResultState(RESULTSTATE_CLEAR);
							SetGameState(GAMESTATE_END);
						}*/
					}
					else if (pPos->x <= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.x + g_aItem[nCntItem].posModel.x &&
						(pPosOld->x >= g_aModel[g_aItem[nCntItem].type].vtxMaxModel.x + g_aItem[nCntItem].posModel.x))
					{
						pPos->x = g_aModel[g_aItem[nCntItem].type].vtxMaxModel.x + g_aItem[nCntItem].posModel.x;
						pMove->x = 0.0f;

					/*	if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_3)
						{
							g_aItem[nCntItem].bUse = false;
							DeleteShadow(g_aItem[nCntItem].nIdx);
							SetResultState(RESULTSTATE_CLEAR);
							SetGameState(GAMESTATE_END);
						}*/
					}
				}
			}

			if (pPos->y < 0)
			{
				pPos->y = 0.0f;
				pPosOld->y = 0.0f;
				pMove->y = 0.0f;
				bRand = true;
			}
		}
	}
	return bRand;
}

//=============================================================================
// モデルの出現
//=============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move, ITEMTYPE type,int nCntMax)
{
	//情報の使われていない場所を検索する
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_aItem[nCntItem].bUse)
		{
			//情報の格納及び使用中に設定
			g_aItem[nCntItem].posModel = pos;
			g_aItem[nCntItem].moveModel = move;
			g_aItem[nCntItem].rotModel = rot;
			g_aItem[nCntItem].nCntMax = nCntMax;
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].bUse = true;

			//==========================影=========================//
			////今のアイテムが机だったとき
			//if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_1)
			//{
			//	//影の作成
			//	g_aItem[nCntItem].nIdx = SetShadow(g_aItem[nCntItem].posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(65.0f, 65.0f, 65.0f));
			//}

			////今のアイテムが椅子だったとき
			//else if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_2)
			//{
			//	//影の作成
			//	g_aItem[nCntItem].nIdx = SetShadow(g_aItem[nCntItem].posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 50.0f));
			//}

			////今のアイテムが花だった時
			//else if (g_aItem[nCntItem].type == ITEMTYPE_CLOUD_3)
			//{
			//	g_aItem[nCntItem].nIdx = SetShadow(D3DXVECTOR3(430.0f, 75.5f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(15.0f, 10.0f, 15.0f));

			//}
			break;
		}
	}
}

////=============================================================================
//// モデル配置の種類
////=============================================================================
//void SetLayout(LAYOUTTYPE nType)
//{
//	//情報の使われていない場所を検索する
//	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
//	{
//		if (g_aItem[nCntItem].bUse)
//		{
//			//情報の格納及び使用中に設定
//			g_aItem[nCntItem].posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_aItem[nCntItem].rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_aItem[nCntItem].nType = ITEMTYPE_NONE;
//			g_aItem[nCntItem].bUse = false;
//			DeleteShadow(g_aItem[nCntItem].nIdx);
//		}
//	}
//
//	//等間隔の机椅子配置
//	if (nType == LAYOUTTYPE_NORMAL)
//	{
//		//机と椅子
//		for (int nCntDepth = 0; nCntDepth < NORMAL_DEPTH; nCntDepth++)
//		{
//			for (int nCntWidth = 0; nCntWidth < NORMAL_WEDTH; nCntWidth++)
//			{
//				SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(430.0f - (170 * nCntWidth), 0.0f, -250.0f + (170 * nCntDepth)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//				SetModel(ITEMTYPE_CLOUD_2, D3DXVECTOR3(430.0f - (170 * nCntWidth), 0.0f, -230.0f + (170 * nCntDepth)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//			}
//		}
//
//		//今の画面がタイトル画面だったとき
//		if (GetMode() == MODE_TITLE)
//		{
//			SetModel(ITEMTYPE_CLOUD_3, D3DXVECTOR3(430.0f, 80.0f, -250.0f), D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
//
//			//影の作成
//			SetShadow(D3DXVECTOR3(430.0f, 75.5f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(15.0f, 10.0f, 15.0f));
//		}
//
//		//現在のレイアウトタイプの変更
//		g_nLayout = LAYOUTTYPE_NORMAL;
//	}
//	else if (nType == LAYOUTTYPE_WALL)
//	{
//		//机ウォール
//		for (int nCntWidth = 0; nCntWidth < WALL_WEDTH; nCntWidth++)
//		{
//			for (int nCntWall = 0; nCntWall < WALL_DEPTH; nCntWall++)
//			{
//				//一回目
//				if (nCntWall == 0)
//				{
//					//モデルの配置(壁の配置位置【左端】 - (机と机の間隔 * 数), 0.0f, 0.0f)
//					SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(450.0f - (78 * nCntWidth), 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//				}
//				//二回目以降
//				else
//				{
//					//モデルの配置(壁の配置位置【左端】 - (机と机の間隔 * 数), 0.0f, 0.0f)
//					SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(450.0f - (78 * nCntWidth), 150.0f + (75 * (nCntWall - 1)), 0.0f), D3DXVECTOR3(3.14f, 0.0f, 0.0f));
//				}
//			}
//		}
//
//		//現在のレイアウトタイプの変更
//		g_nLayout = LAYOUTTYPE_WALL;
//	}
//	else if (nType == LAYOUTTYPE_CIRCLE)
//	{
//		//円
//		for (int nCntDepth = 0; nCntDepth < CIRCLE_MAX; nCntDepth++)
//		{
//			//一列目及び最後の列
//			if (nCntDepth % CIRCLE_MAX == 0 || nCntDepth == CIRCLE_MAX - 1)
//			{
//				for (int nCntWidth = 0; nCntWidth < CIRCLE_MAX; nCntWidth++)
//				{
//					SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(430.0f - (170 * (nCntWidth + 1)), -0.5f, -250.0f + (170 * nCntDepth)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//				}
//			}
//			else
//			{
//				for (int nCount = 0; nCount < 2; nCount++)
//				{
//					SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(260.0f - 170 * (CIRCLE_MAX - 1) * nCount, 0.0f, -250.0f + 170 * nCntDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//				}
//			}
//		}
//
//		//現在のレイアウトタイプの変更
//		g_nLayout = LAYOUTTYPE_CIRCLE;
//	}
//	else if (nType == LAYOUTTYPE_GROUP)
//	{
//		SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(200.0f, 0.0f, -220.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f));
//		SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(148.0f, 0.0f, -220.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f));
//		SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(200.0f, 0.0f, -145.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f));
//		SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(148.0f, 0.0f, -145.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f));
//
//		//現在のレイアウトタイプの変更
//		g_nLayout = LAYOUTTYPE_GROUP;
//	}
//}

//=============================================================================
// モデル配置の種類
//=============================================================================
bool SphereModel(COLLISIONTYPE  nType, D3DXVECTOR3 *C1, D3DXVECTOR3 *C2, D3DXVECTOR3 *M1, D3DXVECTOR3 *M2, float R1, float R2)
{
	bool bDetection = false;
	//D3DXVECTOR3 C3;							//計算用
	D3DXVECTOR3 save = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//保存用
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if ((C1->x - C2->x) * (C1->x - C2->x) + (C1->z - C2->z) * (C1->z - C2->z) <= (R1 + R2) * (R1 + R2))
	{
		save = *C1 - *C2;
		D3DXVec3Normalize(&vec, &save);			//正規化するb

		if (nType == COLLISIONTYPE_NORMAL)
		{
			C1->z += vec.z * ((R1 + R2) - fabsf(save.z));
			C1->x += vec.x * ((R1 + R2) - fabsf(save.x));

			M1->x = 0.0f;
			M1->z = 0.0f;
			M2->x = 0.0f;
			M2->z = 0.0f;
		}
		else if (nType == COLLISIONTYPE_WEAPON)
		{
			bDetection = true;
		}
	}
	return bDetection;
}

////=============================================================================
//// モデル配置のチェンジ
////=============================================================================
//void ChangeModel(void)
//{
//	//============================ モデルの配置 ============================//
//	if (GetMode() == MODE_RESULT)
//	{
//		//机セットのモデル
//		SetModel(ITEMTYPE_CLOUD_1, D3DXVECTOR3(430.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		SetModel(ITEMTYPE_CLOUD_2, D3DXVECTOR3(430.0f, 0.0f, -230.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//
//		//フラワー
//		SetModel(ITEMTYPE_CLOUD_3, D3DXVECTOR3(430.0f, 80.0f, -250.0f), D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
//	}
//	else if (GetMode() == MODE_TITLE)
//	{
//		//レイアウト「普通の教室」
//		SetLayout(LAYOUTTYPE_NORMAL);
//	}
//	else if (GetMode() == MODE_GAME)
//	{
//		if (IsFinish())
//		{
//			//レイアウト「普通の教室」
//			SetLayout(LAYOUTTYPE_CIRCLE);
//		}
//		else
//		{
//			//レイアウト「普通の教室」
//			SetLayout(LAYOUTTYPE_NONE);
//		}
//	}
//	else
//	{
//		//レイアウト「普通の教室」
//		SetLayout(LAYOUTTYPE_CIRCLE);
//	}
//}

// ----------------------------------------------------------------------------
// ステージ読み込み
// ----------------------------------------------------------------------------
bool SetStage(int nStageNo)
{
	FILE *pFile;
	char aTextBuff[128];
	int nCntObject = 0;
	pFile = 0;

	// ブロックの情報の初期化
	for (nCntObject = 0; nCntObject < MAX_ITEM; nCntObject++)
	{
		g_aItem[nCntObject].bUse = false;
	}

	nCntObject = 0;

	if (nStageNo == 0)
	{
		pFile = fopen(STAGE_FILE, "r");
	}


	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aTextBuff[0]);
			// ファイル範囲
			if (strcmp(&aTextBuff[0], "SCRIPT") == 0)
			{
				while (strcmp(&aTextBuff[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aTextBuff[0]);
					// ブロックファイル読み込み
					if (strcmp(&aTextBuff[0], "BLOCKSET") == 0)
					{
						while (strcmp(&aTextBuff[0], "END_BLOCKSET") != 0)
						{
							fscanf(pFile, "%s", &aTextBuff[0]);
							if (strcmp(&aTextBuff[0], "pos") == 0)
							{
								fscanf(pFile, " = %f,%f,%f", &g_aItem[nCntObject].posModel.x, &g_aItem[nCntObject].posModel.y, &g_aItem[nCntObject].posModel.z);
							}
							else if (strcmp(&aTextBuff[0], "move") == 0)
							{
								fscanf(pFile, " = %f,%f,%f", &g_aItem[nCntObject].moveModel.x, &g_aItem[nCntObject].moveModel.y, &g_aItem[nCntObject].moveModel.z);
							}
							else if (strcmp(&aTextBuff[0], "rot") == 0)
							{
								fscanf(pFile, " = %f,%f,%f", &g_aItem[nCntObject].rotModel.x, &g_aItem[nCntObject].rotModel.y, &g_aItem[nCntObject].rotModel.z);
							}
							else if (strcmp(&aTextBuff[0], "CountMax") == 0)
							{
								fscanf(pFile, " = %d", &g_aItem[nCntObject].nCntMax);
							}
							else if (strcmp(&aTextBuff[0], "type") == 0)
							{
								fscanf(pFile, " = %d", &g_aItem[nCntObject].type);
							}
						}
						// ブロック情報入手
						SetModel(g_aItem[nCntObject].posModel, g_aItem[nCntObject].rotModel, g_aItem[nCntObject].moveModel, g_aItem[nCntObject].type, g_aItem[nCntObject].nCntMax);
						nCntObject++;
					}
				}
				break;
			}

		}
		fclose(pFile);
		return true;
	}
	return false;
}

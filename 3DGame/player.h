//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PLAYER	30
#define PLAYER_LIFE		30

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_RUN,
	MOTIONTYPE_RUNATTACK,
	MOTIONTYPE_ATTACK_1,
	MOTIONTYPE_ATTACK_2,
	MOTIONTYPE_DAMAGE,
	MOTIONTYPE_MAX
} MOTIONTYPE;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		//頂点バッファへのポインタ
	LPD3DXMESH		pMeshPlayer = NULL;						//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatPlayer = NULL;					//マテリアル情報へのポインタ
} PLAYERPARTS;

typedef struct
{
	D3DXVECTOR3	pos;										//位置
	D3DXVECTOR3 rot;										//回転
	int		nFrame;											//フレーム数
} KEY;

typedef struct
{
	KEY		aKey[10];										//キーの構造体
	D3DXVECTOR3 CurrentPos;									//1フレーム当たりの移動量
	D3DXVECTOR3 CurrentRot;									//1フレーム当たりの回転量
	int		nMaxKey;										//キー数
	int		nLoop;											//ループするかどうか
} MOTION;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		//頂点バッファへのポインタ
	LPD3DXMESH		pMeshPlayer = NULL;						//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatPlayer = NULL;					//マテリアル情報へのポインタ

	MOTION	aMotion[MOTIONTYPE_MAX];						//モーションの構造体
	DWORD			nNumMatPlayer = 0;						//マテリアル情報の数
	D3DXVECTOR3		originPos;								//位置のプリセット
	D3DXVECTOR3		posPlayer;								//モデルの位置
	D3DXVECTOR3		posOld;									//プレイヤーの前回の位置
	D3DXVECTOR3		movePlayer;								//モデルの移動量
	D3DXVECTOR3		originRot;								//回転のプリセット
	D3DXVECTOR3		rotPlayer;								//モデルの向き(回転)
	D3DXMATRIX		mtxWorldPlayer;							//モデルのワールドマトリックス
	int nIdxShadow;											//影のID
	int nIndex;												//モデルのインデックス
	int nParent;											//モデルの親番号
	char sAdd[64];											//モデルのアドレス
} MODEL;

typedef struct
{
	D3DXMATRIX	mtxWorld;									//ワールドマトリックス
	D3DXVECTOR3 pos;										//位置
	D3DXVECTOR3 posOld;										//前の位置
	D3DXVECTOR3 move;										//移動量
	D3DXVECTOR3 rot;										//回転
	D3DXVECTOR3	dest;										//モデルの最終到達点
	D3DXVECTOR3	Difference;									//モデルの最大回転
	int			nIdxShadow;									//影のID
	int			nLife;										//体力
	MODEL		aModel[MAX_PLAYER];							//モデルの情報
	bool bJump;												//ジャンプしているかどうか
	int CurrentFrame;									//現在のフレーム数
	int CurrentKey;									//現在のキー
	int nAnimationType;								//現在のアニメーション
	float nValueH;									//コントローラー
	float nValueV;									//コントローラー
} PLAYER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
#endif
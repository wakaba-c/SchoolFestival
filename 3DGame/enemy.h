//=============================================================================
//
// 敵処理 [enemy.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_MODEL	30

#define MAX_LIFE_EFFECT (1)

#define MAX_RANDOM (10)

#define MAX_RATE (20)

#define MAX_BASE (20)

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	MOTIONENEMY_NEUTRAL = 0,
	MOTIONENEMY_RUN,
	MOTIONENEMY_RUNATTACK,
	MOTIONENEMY_ATTACK_1,
	MOTIONENEMY_ATTACK_2,
	MOTIONENEMY_DAMAGE,
	MOTIONENEMY_MAX
} MOTIONENEMY;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//頂点バッファへのポインタ
	LPD3DXMESH		pMeshEnemy = NULL;						//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatEnemy = NULL;					//マテリアル情報へのポインタ
	DWORD			nNumMatEnemy = 0;						//マテリアル情報の数
} ENEMYPARTS;

typedef struct
{
	D3DXVECTOR3	pos;										//位置
	D3DXVECTOR3 rot;										//回転
	int		nFrame;											//フレーム数
} ENEMYKEY;

typedef struct
{
	ENEMYKEY		aKey[10];										//キーの構造体
	D3DXVECTOR3 CurrentPos;									//1フレーム当たりの移動量
	D3DXVECTOR3 CurrentRot;									//1フレーム当たりの回転量
	int		nMaxKey;										//キー数
	int		nLoop;											//ループするかどうか
} ENEMYMOTION;

typedef struct
{
	ENEMYMOTION		aMotion[MOTIONENEMY_MAX];				//モーションの構造体
	D3DXVECTOR3		originPos;								//位置のプリセット
	D3DXVECTOR3		posEnemy;								//モデルの位置
	D3DXVECTOR3		posOld;									//プレイヤーの前回の位置
	D3DXVECTOR3		moveEnemy;								//モデルの移動量
	D3DXVECTOR3		originRot;								//回転のプリセット
	D3DXVECTOR3		rotEnemy;								//モデルの向き(回転)
	D3DXMATRIX		mtxWorldEnemy;							//モデルのワールドマトリックス
	int				nIdxShadow;								//影のID
	int				nIndex;									//モデルのインデックス
	int				nParent;								//モデルの親番号
	char			sAdd[64];								//モデルのアドレス
} ENEMYMODEL;

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
	int			nWait;										//次の行動に移す時間
	ENEMYMODEL	aModel[MAX_MODEL];							//モデルの情報
	bool bJump;												//ジャンプしているかどうか
	bool bUse;												//使われているか

	int CurrentEnemyFrame = 0;								//現在のフレーム数
	int CurrentEnemyKey = 1;								//現在のキー
	int nAnimationEnemy = 0;								//現在のアニメーション

	int nCntAttack = 0;										//アタックするまでのカウンタ

} ENEMY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, int nLife);
ENEMY *GetEnemy(void);
#endif
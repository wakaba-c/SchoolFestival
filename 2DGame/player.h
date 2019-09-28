// ==========================================================
//
// プレイヤー[player.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene2D.h"

#define PLAYER_TEX "data/TEXTURE/プレイヤーチーム.png"
#define PLAYERSIZE_X	(60)		//
#define PLAYERSIZE_Y	(100)
#define PLAYER_LIFE		(999)
#define PLAYERTEX_X		(0.25f)		//
#define PLAYERTEX_Y		(0.5f)
#define PLAYERTEX_RIGHT (1.0f)
#define PLAYER_STOCK (-1)
#define ANIM_INTERVAL (12)
#define PLAYER_ANIM (4)
#define BULLET_MOVE (12.0f)
#define BULLET_LIFE (60)
#define ACTION_LIMIT (100)


// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CPlayer : public CScene2D
{
public:

	enum PLAYERSTATE
	{
		PLAYERSTATE_NORMAL = 0,
		PLAYERSTATE_DASH,//ダッシュ
		PLAYERSTATE_MAX
	};

	CPlayer(PRIORITY type);					// コンストラクタ
	~CPlayer();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetMove(D3DXVECTOR3 move);
	void AddDamage(int nDamage);
	void PLAYERDASH(void);
	void AddLife(int nLife);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetMove(void);

	int GetDirectionMove(void);

	static int GetLife(void);
	static CPlayer *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static int GetCntWaterspout(void);
	static void SetCntWaterspout(int nCntWaterspout);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static int m_nCntWaterspout;
	static int m_nLife;

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_rot;		// 回転
	D3DXVECTOR3 m_move;		// 移動
	PLAYERSTATE m_state;

	int m_nCounterAnim;						// カウンター
	int m_nPatternAnim;						// パターンNo
	int m_nDirectionMove;
	int m_nCount;
	float m_fVelocity;						//重力
	float m_nGravity;						//重力
	bool m_bJump;							//ジャンプしているかどうか
	bool m_bDoubleJump;						//二段ジャンプしているかどうか
};
#endif
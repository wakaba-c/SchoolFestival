// ==========================================================
//
// ステート[playerState.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _PLAYERSTATE_H_
#define _PLAYERSTATE_H_

#include "main.h"
#include "scene2D.h"

#define MAX_EXPLOSION (128)			// 爆発の最大数
#define COORDINATE_X (150)			// X座標
#define COORDINATE_Y (150)			// Y座標
#define EXPLOSION_ANIM_INTERVAL (10)
#define STATE_X (0.1f)
#define STATE_Y (1.0f)
#define EXPLOSION_ANIM_COUNT (10)
#define DEATH_X (150)			// X座標
#define DEATH_Y (150)			// Y座標
#define	PLAYER_UP		"data/TEXTURE/pipo-btleffect019.png"		// 読み込むテクスチャファイル名
#define	PLAYER_STATE		"data/TEXTURE/btleffect.png"				// 読み込むテクスチャファイル名

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CPlayerState : public CScene2D
{
public:
	//=============================================================================
	// 弾の種類
	//=============================================================================
	typedef enum
	{
		STATETYPE_PLAYER = 0,
		STATETYPE_UP,
		STATETYPE_BARRIER,
		STATETYPE_MAX
	} STATETYPE;

	CPlayerState(PRIORITY type);	// コンストラクタ
	~CPlayerState();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CPlayerState *Create(STATETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[STATETYPE_MAX];
	static CPlayerState *m_pPlayerState;
	static STATETYPE m_state;

	int m_nCounterAnim;						// カウンター
	int m_nPatternAnim;						// パターンNo
};
#endif
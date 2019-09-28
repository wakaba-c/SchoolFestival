// ==========================================================
//
// 爆破[explosion.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _EXPLSION_H_
#define _EXPLSION_H_

#include "main.h"
#include "scene2D.h"

#define MAX_EXPLOSION (128)			// 爆発の最大数
#define COORDINATE_X (150)			// X座標
#define COORDINATE_Y (150)			// Y座標
#define EXPLOSION_ANIM_INTERVAL (10)
#define EXPLOSIONTEX_X (0.33f)
#define EXPLOSIONTEX_Y (1.0f)
#define EXPLOSION_ANIM_COUNT (10)
#define DEATH_X (150)			// X座標
#define DEATH_Y (150)			// Y座標
#define	PLAYER_EFFECT		"data/TEXTURE/ダッシュエフェクト.png"		// 読み込むテクスチャファイル名

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CExplosion : public CScene2D
{
public:
	//=============================================================================
	// 弾の種類
	//=============================================================================
	typedef enum
	{
		EXPLOSIONTYPE_PLAYER = 0,
		EXPLOSIONTYPE_PLAYER_EFFECT,
		EXPLOSIONTYPE_MAX
	} EXPLOSIONTYPE;

	CExplosion(PRIORITY type);	// コンストラクタ
	~CExplosion();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CExplosion *Create(EXPLOSIONTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[EXPLOSIONTYPE_MAX];
	static CExplosion *m_pExplosion;

	int m_nCounterAnim;						// カウンター
	int m_nPatternAnim;						// パターンNo
};
#endif
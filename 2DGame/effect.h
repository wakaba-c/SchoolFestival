// ==========================================================
//
// エフェクト[effect.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene2D.h"

#define	TEXTURE_EFFECT		"data/TEXTURE/effect.png"	// 読み込むテクスチャファイル名
#define EFFECTSIZE_X		(150)
#define EFFECTSIZE_Y		(250)
#define ATTACKENEMYTEX_X (0.1f)
#define ATTACKENEMYTEX_Y (1.0f)
#define EFFECTANIM_INTERVAL (10)

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CEffect : public CScene2D
{
public:

	CEffect(PRIORITY type);				// コンストラクタ
	~CEffect();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CEffect *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static void MovePos(D3DXVECTOR3 pos, float fMove);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static CEffect *m_pEffect;

	D3DXCOLOR m_col;						// 色
	int m_nCounterAnim;						// カウンター
	int m_nPatternAnim;						// パターンNo	
};
#endif
// ==========================================================
//
// ランキング[ranking.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

#define	RANKING_BG_X (640.0f)
#define	RANKING_BG_Y (360.0f)
#define	TIME_COUNT (600)

class CBg;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CRanking
{
public:
	CRanking();					// コンストラクタ
	~CRanking();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CRanking *Create(void);
protected:

private:
	CBg *m_pBg;

	static int m_nTime;
};
#endif
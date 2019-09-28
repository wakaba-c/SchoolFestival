// ==========================================================
//
// タイトル[title.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

#define TITLE_BG_X (640.0f)
#define TITLE_BG_Y (360.0f)
#define TITLE_EEG_X (500.0f)
#define TITLE_EEG_Y (400.0f)
#define TITLE_EFFECT_X (500.0f)
#define TITLE_EFFECT_Y (350.0f)
#define FADE_COUNT (600)

class CBg;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CTitle
{
public:
	CTitle();					// コンストラクタ
	~CTitle();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CTitle *Create(void);
protected:

private:
	CBg *m_pBg;

	static int m_nTime;
};
#endif
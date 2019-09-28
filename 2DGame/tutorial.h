// ==========================================================
//
// チュートリアル[tutorial.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

#define TUTORIAL_BG_X (640.0f)
#define TUTORIAL_BG_Y (360.0f)

class CBg;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CTutorial
{
public:

	CTutorial();					// コンストラクタ
	~CTutorial();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CTutorial *Create(void);

protected:

private:
	static CBg *m_pBg;
};
#endif
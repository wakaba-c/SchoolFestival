// ==========================================================
//
// リザルト[result.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

#define RESULT_BG_X (640.0f)
#define RESULT_BG_Y (360.0f)

class CBg;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CResult
{
public:

	CResult();					// コンストラクタ
	~CResult();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static CResult *Create(void);

protected:

private:
	static CBg *m_pBg;
};
#endif
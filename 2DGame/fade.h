// ==========================================================
//
// フェード[fade.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

#define FADESIZE_X (1280)
#define FADESIZE_Y (720)

class CPolygon;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CFade
{
public:
	//=============================================================================
	// フェードの状態
	//=============================================================================
	typedef enum
	{
		FADE_NONE = 0,		// 何もない状態
		FADE_IN,			// フェードイン処理
		FADE_OUT,			// フェードアウト処理
		FADE_MAX
	} FADE;

	CFade();					// コンストラクタ
	~CFade();					// デストラクタ

	void Init(CManager::MODE modeNext);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static FADE GetFade(void);
	static void SetFade(CManager::MODE modeNext);
	static CFade *Create(CManager::MODE mode);

protected:

private:
	static CFade *m_pFade;
	static FADE						m_fade;							// フェード状態
	static CManager::MODE			m_modeNext;					// 次の画面（モード）
	static D3DXCOLOR				m_colorFade;

	CPolygon *m_pPolygon;
};
#endif
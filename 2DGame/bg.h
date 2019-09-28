// ==========================================================
//
// 背景[bg.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "scene2D.h"

#define BGSIZE_X (1280)
#define BGSIZE_Y (720)

#define	TEXTURE_BG	"data/TEXTURE/bg_sabaku.jpg"			// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE	"data/TEXTURE/sabaku(タイトル).png"				// 読み込むテクスチャファイル名
#define	TEXTURE_TUTORIAL	"data/TEXTURE/チュートリアルチーム.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT	"data/TEXTURE/oasis.jpg"			// 読み込むテクスチャファイル名
#define TEXTURE_FAILD "data/TEXTURE/desert.jpg"

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CBg : public CScene2D
{
public:
	//=============================================================================
	// 構造体定義
	//=============================================================================
	typedef enum
	{
		RESULTSTATE_CLEAR = 0,
		RESULTSTATE_FAILD,
		RESULTSTATE_MAX
	} RESULTSTATE;

	CBg(PRIORITY type);					// コンストラクタ
	~CBg();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static void SetResultState(RESULTSTATE state);
	static void MoveBg(D3DXVECTOR3 fMove);
	static CBg *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static RESULTSTATE GetResultState(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static CBg *m_pBg;
	static RESULTSTATE m_ResultState;
	D3DXVECTOR3 m_move;
	float m_fXpos;
};
#endif
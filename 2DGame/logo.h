// ==========================================================
//
// ロゴ[logo.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"

#define LOGOSIZE_X (500)
#define LOGOSIZE_Y (180)
#define LOGO_POSX (800.0f)
#define LOGO_POSY (300.0f)
#define ENTERSIZE_X (450)
#define ENTERSIZE_Y (200)
#define ENTER_POSX (640.0f)
#define ENTER_POSY (600.0f)
#define RANKENTER_POS (650.0f)
#define TUTORIAL_ENTER_POSX (400.0f)
#define TUTORIAL_ENTER_POSY (650.0f)
#define GRIFFONSIZE_X (500)
#define GRIFFONSIZE_Y (400)
#define GRIFFON_POSX (1000.0f)
#define GRIFFON_POSY (200.0f)
#define FAILDSIZE_X (500)
#define FAILDSIZE_Y (100)
#define FAILD_POSX (640.0f)
#define FAILD_POSY (300.0f)
#define CLEARSIZE_X (500)
#define CLEARSIZE_Y (100)
#define CLEAR_POSX (640.0f)
#define CLEAR_POSY (300.0f)
#define STOCKSIZE_X (150)
#define STOCKSIZE_Y (100)
#define STOCK_POSX (780.0f)
#define STOCK_POSY (45.0f)
#define RANKNUMSIZE_X (100)
#define RANKNUMSIZE_Y (500)
#define RANKNUM_POSX (450.0f)
#define RANKNUM_POSY (350.0f)
#define UISIZE_Y (100)
#define UI_POSX (640.0f)
#define UI_POSY (50.0f)
#define PLAYER_POSX (70.0f)
#define PLAYER_POSY (55.0f)
#define PLAYER_SIZE (50)
#define RANK_POSX (640.0f)
#define RANK_POSY (50.0f)
#define LOGOANIM_INTERVAL (20)
#define LOGOANIM_INTERVAL_2 (3)
#define LOGOANIM_COUNT (8)
#define LOGOTEX_X (0.5f)
#define LOGOTEX_Y (1.0f)


#define	LOGO_TITLE	"data/TEXTURE/ブロスピ.png"		// 読み込むテクスチャファイル名
#define	LOGO_TUTORIAL	"data/TEXTURE/チュートリアルチーム.png"		// 読み込むテクスチャファイル名
//#define	TEXTURE_LOGO	"data/TEXTURE/logo.png"			// 読み込むテクスチャファイル名
#define	TEXTURE_ENTER	"data/TEXTURE/PRESSENTER01.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_CLEAR	"data/TEXTURE/GAME CLEAR.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_FAILDLOGO	"data/TEXTURE/GAME OVER.png"	// 読み込むテクスチャファイル名
//#define	TEXTURE_BULLET	"data/TEXTURE/bullet.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_UI	"data/TEXTURE/square.jpg"			// 読み込むテクスチャファイル名
//#define	TEXTURE_PLAYER	"data/TEXTURE/Bird546.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING	"data/TEXTURE/Ranking01.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKNUM	"data/TEXTURE/rank02.png"		// 読み込むテクスチャファイル名

class CScene2D;
// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CLogo : public CScene
{
public:
	//=============================================================================
	// 構造体定義
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_TITLE = 0,
		LOGOTYPE_ENTER,
		LOGOTYPE_TUTORIAL,
		LOGOTYPE_CLEAR,
		LOGOTYPE_FAILD,
		LOGOTYPE_UI,
		LOGOTYPE_RANKING,
		LOGOTYPE_RANKNUM,
		LOGOTYPE_MAX
	} LOGOTYPE;

	CLogo(PRIORITY type);			// コンストラクタ
	~CLogo();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetLogo(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt);

	static CLogo *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static LOGOTYPE GetType(void);
	static void SetEnter(bool bEnter);

protected:

private:
	static LOGOTYPE m_type;
	static LPDIRECT3DTEXTURE9 m_pTex[LOGOTYPE_MAX];
	static bool m_bEnter;

	CScene2D *m_pScene2D[LOGOTYPE_MAX];
	D3DXVECTOR3 m_pos;

	int m_nCounterAnim;						// カウンター
	int m_nPatternAnim;						// パターンNo
	float m_fWidth;
	float m_fSize;
};
#endif
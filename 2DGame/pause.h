// ==========================================================
//
// ポーズ[pause.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene.h"


#define	TEXTURE_PAUSE		"data/TEXTURE/huruikami.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PAUSE_TITLE		"data/TEXTURE/taitoru.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TUDUKERU	"data/TEXTURE/tudukeru.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_MARK		"data/TEXTURE/nikukyuu.png"	// 読み込むテクスチャファイル名

#define PAUSE_X (500)									// X座標
#define PAUSE_Y (720)									// Y座標
#define PAUSE_POSX (640.0f)									// X座標
#define PAUSE_POSY (360.0f)									// Y座標
#define TITlE_X (500)									// X座標
#define TITlE_Y (500)									// Y座標
#define TITlE_POSX (600.0f)									// X座標
#define TITlE_POSY (400.0f)									// Y座標
#define TUDUKERU_X (500)								// X座標
#define TUDUKERU_Y (500)								// Y座標
#define TUDUKERU_POSX (600.0f)								// X座標
#define TUDUKERU_POSY (200.0f)								// Y座標
#define SAKANA_X (100)									// X座標
#define SAKANA_Y (100)
#define SAKANA_POSX (750.0f)								
#define SAKANA_POSY (180.0f)
#define SAKANA_POSX_2 (800.0f)								
#define SAKANA_POSY_2 (380.0f)

class CScene2D;
// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CPause : public CScene
{
public:
	//=============================================================================
	// 構造体定義
	//=============================================================================
	typedef enum
	{

		PAUSETYPE_PAUSE = 0,
		PAUSETYPE_TITLE,
		PAUSETYPE_TUDUKERU,
		PAUSETYPE_MARK,
		PAUSETYPE_MAX
	} PAUSETYPE;

	CPause(PRIORITY type);			// コンストラクタ
	~CPause();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetLogo(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt);
	void SetPause(bool bPause);

	static CPause *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static PAUSETYPE GetType(void);

protected:

private:
	CScene2D *m_pScene2D[PAUSETYPE_MAX];

	static PAUSETYPE m_type;
	static LPDIRECT3DTEXTURE9 m_pTex[PAUSETYPE_MAX];
	static bool m_bEnter;

	bool m_bPause;
	int m_nSelect;
};
#endif
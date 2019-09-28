// ==========================================================
//
// ポリゴン[polygon.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

#define	TEXTURE_POLYGON	"data/TEXTURE/square.jpg"	// 読み込むテクスチャファイル名

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CPolygon
{
public:
	CPolygon();					// コンストラクタ
	~CPolygon();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetPolygon(D3DXVECTOR3 pos, float fWidth,int nSizeY);
	void SetLife(float fWidth);
	void SetColPolygon(D3DXCOLOR col);

	static CPolygon *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;	// テクスチャへのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// バッファ
	VERTEX_2D *m_pVtx;						// 頂点格納
	D3DXVECTOR3 m_pos;
	D3DXCOLOR m_col;

	int m_nSizeX;
	int m_nSizeY;
	float m_fWidth;
};
#endif
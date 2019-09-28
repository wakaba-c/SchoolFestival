// ==========================================================
//
// 数字[number.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

#define NUMBERSIZE_X (33)
#define NUMBERSIZE_Y (60)
#define	TEXTURE_SCORE	"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CNumber
{
public:

	CNumber();					// コンストラクタ
	~CNumber();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);			// 描画
	void SetNumber(int nNumber);
	void SetPosNum(D3DXVECTOR3 pos, int nSizeX, int nSizeY);

	static CNumber *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;	// テクスチャへのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// バッファ
	VERTEX_2D *m_pVtx;						// 頂点格納
	D3DXVECTOR3 m_pos;
};
#endif
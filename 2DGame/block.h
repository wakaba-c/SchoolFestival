// ==========================================================
//
// ブロック[block.h]
// Author:Seiya Takahashi
//
// ==========================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "scene2D.h"

#define MAX_BLOCK (200)								// ブロックの最大数
#define BLOCK_TEXTURE "data/TEXTURE/block2.png"

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CBlock : public CScene2D
{
public:
	CBlock(PRIORITY type);					// コンストラクタ
	~CBlock();							// デストラクタ

	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	void Update(void);					// 更新
	void Draw(void);					// 描画
	void SetMove(D3DXVECTOR3 move);
	void SetBlockNum(int nNumBlock);
	int GetBlockNum(void);

	static CBlock *GetBlock(void);
	static CBlock *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static CBlock *m_pBlock;
	static int m_nNumBlock;								//ブロックの個数
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_posOld;								//前回の位置
	D3DXVECTOR3 m_move;									// 移動量

};
#endif
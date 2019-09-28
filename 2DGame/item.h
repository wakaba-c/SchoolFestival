// ==========================================================
//
// アイテム[item.h]
// Author:Seiya Takahashi
//
// ==========================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "scene2D.h"

#define MAX_ITEM (200)								// ブロックの最大数
#define ITEM_TEXTURE_DRINK "data/TEXTURE/energydrink2.png"
#define ITEM_TEXTURE_COIN "data/TEXTURE/bullet000.png"
#define ITEM_TEXTURE_MAGNET "data/TEXTURE/nikukyuu.png"
#define MAX_ITEMTEX (3)

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CItem : public CScene2D
{
public:

	enum ITEMTYPE
	{
		ITEMTYPE_NORMAL = 0,
		ITEMTYPE_WATER,//ライフ回復用
		ITEMTYPE_COIN,//スコア
		ITEMTYPE_MAGNET,//磁石 コインを引き寄せる
		ITEMTYPE_DASH,//移動速度が上がり障害物を吹き飛ばす
		ITEMTYPE_MAX
	};

public:
	CItem(PRIORITY type);					// コンストラクタ
	~CItem();							// デストラクタ

	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	void Update(void);					// 更新
	void Draw(void);					// 描画
	void SetMove(D3DXVECTOR3 move);
	void SetItemNum(int nNumItem);
	void SetType(ITEMTYPE type);

	static CItem *Create(ITEMTYPE nType);
	static HRESULT Load(void);
	static void Unload(void);

	ITEMTYPE GetItemType(void);
	D3DXVECTOR3 GetPosition(void);


protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[MAX_ITEMTEX];
	static CItem *m_pItem;
	static int m_nNumItem;								//ブロックの個数
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_posOld;									//前回の位置
	D3DXVECTOR3 m_move;									// 移動量
	ITEMTYPE m_type;
	D3DXCOLOR m_col;
	D3DXVECTOR3 m_Direction;
	float m_MagneticForce;
};
#endif

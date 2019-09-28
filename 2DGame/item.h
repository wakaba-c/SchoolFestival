// ==========================================================
//
// �A�C�e��[item.h]
// Author:Seiya Takahashi
//
// ==========================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "scene2D.h"

#define MAX_ITEM (200)								// �u���b�N�̍ő吔
#define ITEM_TEXTURE_DRINK "data/TEXTURE/energydrink2.png"
#define ITEM_TEXTURE_COIN "data/TEXTURE/bullet000.png"
#define ITEM_TEXTURE_MAGNET "data/TEXTURE/nikukyuu.png"
#define MAX_ITEMTEX (3)

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CItem : public CScene2D
{
public:

	enum ITEMTYPE
	{
		ITEMTYPE_NORMAL = 0,
		ITEMTYPE_WATER,//���C�t�񕜗p
		ITEMTYPE_COIN,//�X�R�A
		ITEMTYPE_MAGNET,//���� �R�C���������񂹂�
		ITEMTYPE_DASH,//�ړ����x���オ���Q���𐁂���΂�
		ITEMTYPE_MAX
	};

public:
	CItem(PRIORITY type);					// �R���X�g���N�^
	~CItem();							// �f�X�g���N�^

	void Init(void);					// ������
	void Uninit(void);					// �I��
	void Update(void);					// �X�V
	void Draw(void);					// �`��
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
	static int m_nNumItem;								//�u���b�N�̌�
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_posOld;									//�O��̈ʒu
	D3DXVECTOR3 m_move;									// �ړ���
	ITEMTYPE m_type;
	D3DXCOLOR m_col;
	D3DXVECTOR3 m_Direction;
	float m_MagneticForce;
};
#endif

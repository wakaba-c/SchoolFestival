// ==========================================================
//
// �u���b�N[block.h]
// Author:Seiya Takahashi
//
// ==========================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "scene2D.h"

#define MAX_BLOCK (200)								// �u���b�N�̍ő吔
#define BLOCK_TEXTURE "data/TEXTURE/block2.png"

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CBlock : public CScene2D
{
public:
	CBlock(PRIORITY type);					// �R���X�g���N�^
	~CBlock();							// �f�X�g���N�^

	void Init(void);					// ������
	void Uninit(void);					// �I��
	void Update(void);					// �X�V
	void Draw(void);					// �`��
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
	static int m_nNumBlock;								//�u���b�N�̌�
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_posOld;								//�O��̈ʒu
	D3DXVECTOR3 m_move;									// �ړ���

};
#endif
// ==========================================================
//
// ����[number.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

#define NUMBERSIZE_X (33)
#define NUMBERSIZE_Y (60)
#define	TEXTURE_SCORE	"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CNumber
{
public:

	CNumber();					// �R���X�g���N�^
	~CNumber();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);			// �`��
	void SetNumber(int nNumber);
	void SetPosNum(D3DXVECTOR3 pos, int nSizeX, int nSizeY);

	static CNumber *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;	// �e�N�X�`���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// �o�b�t�@
	VERTEX_2D *m_pVtx;						// ���_�i�[
	D3DXVECTOR3 m_pos;
};
#endif
// ==========================================================
//
// �|���S��[polygon.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

#define	TEXTURE_POLYGON	"data/TEXTURE/square.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CPolygon
{
public:
	CPolygon();					// �R���X�g���N�^
	~CPolygon();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPolygon(D3DXVECTOR3 pos, float fWidth,int nSizeY);
	void SetLife(float fWidth);
	void SetColPolygon(D3DXCOLOR col);

	static CPolygon *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;	// �e�N�X�`���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// �o�b�t�@
	VERTEX_2D *m_pVtx;						// ���_�i�[
	D3DXVECTOR3 m_pos;
	D3DXCOLOR m_col;

	int m_nSizeX;
	int m_nSizeY;
	float m_fWidth;
};
#endif
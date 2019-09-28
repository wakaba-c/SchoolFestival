// ==========================================================
//
// �`��2D[scene2D.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

#define POLGON_X (100)			// X���W
#define POLGON_Y (100)			// Y���W
#define CENTERPOS_NORMAL (2)
#define SPIDERPOS_Y (20)
#define SPIDERPOS_Y2 (80)

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CScene2D : public CScene
{
public:
	CScene2D(PRIORITY type);	// �R���X�g���N�^
	~CScene2D();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void BindTex(LPDIRECT3DTEXTURE9 tex);
	void SetAnimation(float fTexX, float fTexY,float fTexY2, int nPatternAnim);
	void SetMove(float fTexX, float fTexY, D3DXVECTOR3 fMove);
	void SetCol(D3DXCOLOR col);
	bool SetCollision(D3DXVECTOR3 pos, int sizeX, int sizeY, D3DXVECTOR3 PartnerPos);
	bool HitBlock(D3DXVECTOR3 *block, D3DXVECTOR3 *player, D3DXVECTOR3 *BposOld, D3DXVECTOR3 *PposOld,
					float blockRadias_x, float blockRadias_y, float playerRadias_x, float playerRadias_y);
	void SetPos(D3DXVECTOR3 pos,float fSizeX,float fSizeY,int nCenterPos);
	void SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	static CScene2D *Create(void);

protected:

private:
	static CScene2D *m_pScene2D;

	LPDIRECT3DTEXTURE9 m_pTexturePolygon;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// �o�b�t�@
	VERTEX_2D *m_pVtx;						// ���_�i�[
	D3DXVECTOR3 m_pos;						// ���݂̈ʒu
	D3DXVECTOR3 m_posOld;					//�O��̈ʒu
	D3DXVECTOR3 m_rot;						// ��]
	
	float m_fAngle;							// �p�x
	float m_fLength;						// ����
	bool m_bLand;							//�u���b�N�ɗ����Ă��邩�ǂ���
};
#endif
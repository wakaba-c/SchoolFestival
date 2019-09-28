// ==========================================================
//
// �̗�[life.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"
#include "scene.h"

#define MAX_LIFE (2)
#define WIDTH_SIZE (500)

class CPolygon;
// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CLife : public CScene
{
public:
	//=============================================================================
	// �̗͂̎��
	//=============================================================================
	typedef enum
	{
		LIFETYPE_PLAYER = 0,
		LIFETYPE_ENEMY,
		LIFETYPE_EGG,
		LIFETYPE_MAX
	} LIFETYPE;

	CLife(PRIORITY type);			// �R���X�g���N�^
	~CLife();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetLife(int nLife);
	void AddLife(int nLife);

	static CLife *GetLife(void);
	static CLife *Create(void);

protected:

private:

	static CLife *m_pLife;

	LIFETYPE m_type;
	CPolygon *m_apPolygon[MAX_LIFE];
	D3DXVECTOR3 m_pos;

	int	m_nLife;				// �̗�
	float m_fWidth;
	float m_fSize;
	float m_fLife;
};
#endif
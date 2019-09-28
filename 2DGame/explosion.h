// ==========================================================
//
// ���j[explosion.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _EXPLSION_H_
#define _EXPLSION_H_

#include "main.h"
#include "scene2D.h"

#define MAX_EXPLOSION (128)			// �����̍ő吔
#define COORDINATE_X (150)			// X���W
#define COORDINATE_Y (150)			// Y���W
#define EXPLOSION_ANIM_INTERVAL (10)
#define EXPLOSIONTEX_X (0.33f)
#define EXPLOSIONTEX_Y (1.0f)
#define EXPLOSION_ANIM_COUNT (10)
#define DEATH_X (150)			// X���W
#define DEATH_Y (150)			// Y���W
#define	PLAYER_EFFECT		"data/TEXTURE/�_�b�V���G�t�F�N�g.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CExplosion : public CScene2D
{
public:
	//=============================================================================
	// �e�̎��
	//=============================================================================
	typedef enum
	{
		EXPLOSIONTYPE_PLAYER = 0,
		EXPLOSIONTYPE_PLAYER_EFFECT,
		EXPLOSIONTYPE_MAX
	} EXPLOSIONTYPE;

	CExplosion(PRIORITY type);	// �R���X�g���N�^
	~CExplosion();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static CExplosion *Create(EXPLOSIONTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[EXPLOSIONTYPE_MAX];
	static CExplosion *m_pExplosion;

	int m_nCounterAnim;						// �J�E���^�[
	int m_nPatternAnim;						// �p�^�[��No
};
#endif
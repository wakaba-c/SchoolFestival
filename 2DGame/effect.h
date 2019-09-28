// ==========================================================
//
// �G�t�F�N�g[effect.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene2D.h"

#define	TEXTURE_EFFECT		"data/TEXTURE/effect.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define EFFECTSIZE_X		(150)
#define EFFECTSIZE_Y		(250)
#define ATTACKENEMYTEX_X (0.1f)
#define ATTACKENEMYTEX_Y (1.0f)
#define EFFECTANIM_INTERVAL (10)

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CEffect : public CScene2D
{
public:

	CEffect(PRIORITY type);				// �R���X�g���N�^
	~CEffect();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static CEffect *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static void MovePos(D3DXVECTOR3 pos, float fMove);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static CEffect *m_pEffect;

	D3DXCOLOR m_col;						// �F
	int m_nCounterAnim;						// �J�E���^�[
	int m_nPatternAnim;						// �p�^�[��No	
};
#endif
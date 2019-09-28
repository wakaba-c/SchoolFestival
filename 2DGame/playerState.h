// ==========================================================
//
// �X�e�[�g[playerState.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _PLAYERSTATE_H_
#define _PLAYERSTATE_H_

#include "main.h"
#include "scene2D.h"

#define MAX_EXPLOSION (128)			// �����̍ő吔
#define COORDINATE_X (150)			// X���W
#define COORDINATE_Y (150)			// Y���W
#define EXPLOSION_ANIM_INTERVAL (10)
#define STATE_X (0.1f)
#define STATE_Y (1.0f)
#define EXPLOSION_ANIM_COUNT (10)
#define DEATH_X (150)			// X���W
#define DEATH_Y (150)			// Y���W
#define	PLAYER_UP		"data/TEXTURE/pipo-btleffect019.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PLAYER_STATE		"data/TEXTURE/btleffect.png"				// �ǂݍ��ރe�N�X�`���t�@�C����

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CPlayerState : public CScene2D
{
public:
	//=============================================================================
	// �e�̎��
	//=============================================================================
	typedef enum
	{
		STATETYPE_PLAYER = 0,
		STATETYPE_UP,
		STATETYPE_BARRIER,
		STATETYPE_MAX
	} STATETYPE;

	CPlayerState(PRIORITY type);	// �R���X�g���N�^
	~CPlayerState();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static CPlayerState *Create(STATETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[STATETYPE_MAX];
	static CPlayerState *m_pPlayerState;
	static STATETYPE m_state;

	int m_nCounterAnim;						// �J�E���^�[
	int m_nPatternAnim;						// �p�^�[��No
};
#endif
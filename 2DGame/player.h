// ==========================================================
//
// �v���C���[[player.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene2D.h"

#define PLAYER_TEX "data/TEXTURE/�v���C���[�`�[��.png"
#define PLAYERSIZE_X	(60)		//
#define PLAYERSIZE_Y	(100)
#define PLAYER_LIFE		(999)
#define PLAYERTEX_X		(0.25f)		//
#define PLAYERTEX_Y		(0.5f)
#define PLAYERTEX_RIGHT (1.0f)
#define PLAYER_STOCK (-1)
#define ANIM_INTERVAL (12)
#define PLAYER_ANIM (4)
#define BULLET_MOVE (12.0f)
#define BULLET_LIFE (60)
#define ACTION_LIMIT (100)


// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CPlayer : public CScene2D
{
public:

	enum PLAYERSTATE
	{
		PLAYERSTATE_NORMAL = 0,
		PLAYERSTATE_DASH,//�_�b�V��
		PLAYERSTATE_MAX
	};

	CPlayer(PRIORITY type);					// �R���X�g���N�^
	~CPlayer();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetMove(D3DXVECTOR3 move);
	void AddDamage(int nDamage);
	void PLAYERDASH(void);
	void AddLife(int nLife);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetMove(void);

	int GetDirectionMove(void);

	static int GetLife(void);
	static CPlayer *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static int GetCntWaterspout(void);
	static void SetCntWaterspout(int nCntWaterspout);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static int m_nCntWaterspout;
	static int m_nLife;

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_rot;		// ��]
	D3DXVECTOR3 m_move;		// �ړ�
	PLAYERSTATE m_state;

	int m_nCounterAnim;						// �J�E���^�[
	int m_nPatternAnim;						// �p�^�[��No
	int m_nDirectionMove;
	int m_nCount;
	float m_fVelocity;						//�d��
	float m_nGravity;						//�d��
	bool m_bJump;							//�W�����v���Ă��邩�ǂ���
	bool m_bDoubleJump;						//��i�W�����v���Ă��邩�ǂ���
};
#endif
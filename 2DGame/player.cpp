// ==========================================================
//
// �v���C���[[player.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "player.h"
#include "manager.h"
#include "inputController.h"
#include "inputKeyboard.h"
#include "renderer.h"
#include "explosion.h"
#include "life.h"
#include "bg.h"
#include "stock.h"
#include "sound.h"
#include "block.h"
#include "scene.h"
#include "playerState.h"
#include "item.h"
#include "game.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
#define PLAYER_GRAVITY 2.5f	//�d��
#define PLAYER_VELOSCITY 70.0f	//�W�����v�̍���

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
int CPlayer::m_nCntWaterspout;
LPDIRECT3DTEXTURE9 CPlayer::m_pTex = NULL;
int CPlayer::m_nLife = 0;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CPlayer::CPlayer(PRIORITY type = CScene::PRIORITY_PLAYER):CScene2D(type)
{
	m_state = PLAYERSTATE_NORMAL;
	m_nCount = 0;
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CPlayer::~CPlayer()
{

}

// ==========================================================
// �v���C���[������
// ==========================================================
void CPlayer::Init(void)
{
	// �v���C���[�̗͏�����
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = PLAYER_LIFE;
	m_fVelocity = PLAYER_VELOSCITY;
	m_bJump = false;
	m_bDoubleJump = false;
	m_nGravity = PLAYER_GRAVITY;
	m_state = PLAYERSTATE_NORMAL;
	m_nCount = 0;

	// �X�L���J�E���g������
	m_nCntWaterspout = 0;

	CScene2D::Init();

	// �A�j���[�V����������
	SetAnimation(PLAYERTEX_X, PLAYERTEX_Y, 0.0f, 0);

	SetObjType(OBJTYPE_PLAYER);

	CExplosion *pExplosion = CExplosion::Create(CExplosion::EXPLOSIONTYPE_PLAYER_EFFECT);

	CPlayerState *pPlayerState = CPlayerState::Create(CPlayerState::STATETYPE_BARRIER);

}

// ==========================================================
// �v���C���[�I��
// ==========================================================
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// �v���C���[�X�V
// ==========================================================
void CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();
	CStock *pStock = CStock::GetStock();
	CBlock *pBlock = CBlock::GetBlock();

	// �p�^�[���؂�ւ�
	m_nCounterAnim++;
	//m_nPatternAnim++;//�J�E���^�[���Z

	m_pos = GetPos();

	//���݂̈ʒu��O��̈ʒu�ɂ���
	m_posOld = m_pos;

	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �L�[�{�[�hC��������
	if (pInputKeyboard->GetKeyboardTngger(DIK_N))
	{
		// �X�g�b�N�����炷
		pStock->AddStock(PLAYER_STOCK);

		// �J�E���g��2��菬�����ꍇ
		if (m_nCntWaterspout <= 2)
		{
			// �J�E���g�𑝂₷
			m_nCntWaterspout++;
		}
	}

	if (pInputKeyboard->GetKeyboardPress(DIK_A))
	{// A�{�^���������ꂽ�Ƃ�
	 // ��
		if (m_state == PLAYERSTATE_NORMAL)
		{
			m_move.x += sinf(-D3DX_PI*0.2f)*4.0f;
			m_move.y += cosf(D3DX_PI*0.2f)*4.0f;
			m_nDirectionMove = 1;
		}

		if (m_state == PLAYERSTATE_DASH)
		{
			m_move.x += sinf(-D3DX_PI*0.2f)*6.0f;
			m_move.y += cosf(D3DX_PI*0.2f)*6.0f;
			m_nDirectionMove = 1;
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_D))
	{// D�{�^���������ꂽ�Ƃ�
		if (m_state == PLAYERSTATE_NORMAL)
		{
			m_move.x += sinf(D3DX_PI*0.5f)*4.0f;
			m_move.y += cosf(D3DX_PI*0.5f)*4.0f;
			m_nDirectionMove = 1;
		}

		if (m_state == PLAYERSTATE_DASH)
		{
			m_move.x += sinf(D3DX_PI*0.5f)*6.0f;
			m_move.y += cosf(D3DX_PI*0.5f)*6.0f;
			m_nDirectionMove = 1;
		}
	}

	if (m_nDirectionMove == 1)
	{// nDirectionMove��0�̂Ƃ�
	 // �p�^�[���X�V
		if (m_nCounterAnim % ANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % PLAYER_ANIM;

			SetAnimation(PLAYERTEX_X, 1.0, PLAYERTEX_Y, m_nPatternAnim);
		}
	}

	if (pInputKeyboard->GetKeyboardTngger(DIK_W))
	{
		if (m_bJump == false)
		{//��i�W�����v
			m_bJump = true;
			m_move.y -= m_fVelocity;
		}

		if (m_bJump == true && m_bDoubleJump == false)
		{//��i�W�����v
			m_bJump = false;
			m_bDoubleJump = true;
			m_move.y = 0;
			m_move.y -= m_fVelocity;
		}
	}

	//�A�j���[�V����
	//if (m_nDirectionMove == 0)
	//{
	//	if (m_nCounterAnim % ANIM_INTERVAL == 0)
	//	{
	//		m_nPatternAnim = (m_nPatternAnim + 1) % PLAYER_ANIM;//�p�^�[���X�V

	//		CScene2D::SetAnimation((float)m_nDirectionMove, 0.25f, 0.5f, m_nPatternAnim);
	//	}
	//}
	//if (m_nDirectionMove == 1)
	//{
	//	if (m_nCounterAnim % ANIM_INTERVAL == 0)
	//	{
	//		m_nPatternAnim = (m_nPatternAnim + 1) % PLAYER_ANIM;//�p�^�[���X�V

	//		CScene2D::SetAnimation((float)m_nDirectionMove, 0.25f, 0.5f, m_nPatternAnim);
	//	}
	//}
	/////////////////////////

	if (m_state == PLAYERSTATE_DASH)
	{
		m_nCount++;
		if (m_nCount >= 180)
		{
			m_state = PLAYERSTATE_NORMAL;
		}
	}

	//�d�� ���t���[��0.5�����[�u�����Z
	m_move.y += m_nGravity;

	// �ړ���
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	// ����
	m_move.x += (0 - m_move.x) * 0.2f;
	m_move.y += (0 - m_move.y) * 0.2f;

	// �̗͎擾
	CLife *pLife = CLife::GetLife();
	//�̗͌���
	m_nLife--;
	// �_���[�W�ʂ𔽉f
	pLife->AddLife(m_nLife);

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
		{
			CScene *pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE type;
				type = pScene->GetObjType();

				if (type == OBJTYPE_BLOCK)
				{
					CBlock *pBlock;
					pBlock = (CBlock*)GetScene(nCntPriority, nCnt);

					if (HitBlock(&pBlock->GetPos(), &m_pos, &pBlock->GetPosOld(), &m_posOld, BLOCKSIZE_X / 2, BLOCKSIZE_Y / 2, PLAYERSIZE_X / 2, PLAYERSIZE_Y / 2) == true)
					{
						if (m_bJump == true)
						{
							m_bJump = false;
						}

						if (pInputKeyboard->GetKeyboardTngger(DIK_W))
						{
							if (m_bJump == false)
							{
								m_bJump = true;

								//�W�����v�̋���
								m_move.y = PLAYER_VELOSCITY;
							}
						}
					}
				}
			}
		}
	}

	// ���ˏ���
	if (m_pos.y + PLAYERSIZE_Y / 2 > SCREEN_HEIGHT)
	{
		m_pos.y = SCREEN_HEIGHT - PLAYERSIZE_Y / 2;
		m_bJump = false;
		m_bDoubleJump = false;
	}
	if (m_pos.y - PLAYERSIZE_Y / 2 < ACTION_LIMIT)
	{
		m_pos.y = ACTION_LIMIT + PLAYERSIZE_Y / 2;
	}
	if (m_pos.x + PLAYERSIZE_X / 2 > SCREEN_WIDTH)
	{
		m_pos.x = SCREEN_WIDTH - PLAYERSIZE_X / 2;
	}
	if (m_pos.x - PLAYERSIZE_X / 2 < 0)
	{
		m_pos.x = 0 + PLAYERSIZE_X / 2;
	}

	// �ʒu���X�V
	SetPos(m_pos, PLAYERSIZE_X, PLAYERSIZE_Y, CENTERPOS_NORMAL);

	CScene2D::Update();
}

// ==========================================================
// �v���C���[�`��
// ==========================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// �|���S������
// ==========================================================
CPlayer *CPlayer::Create(void)
{
	// �V�[�����I�Ɋm��
	CPlayer *pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	if (pPlayer != NULL)
	{
		// �V�[��������
		pPlayer->Init();
		pPlayer->BindTex(m_pTex);
	}
	// �l��Ԃ�
	return pPlayer;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CPlayer::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTex);

	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CPlayer::Unload(void)
{
	// �e�N�X�`�����
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// �v���C���[�̈ʒu�擾
// ==========================================================
D3DXVECTOR3 CPlayer::GetPosition(void)
{
	return m_pos;
}

// ==========================================================
// �v���C���[�̈ړ��ʎ擾
// ==========================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

// ==========================================================
// �v���C���[�̈ړ���
// ==========================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

// ==========================================================
// �v���C���[�̃_���[�W����
// ==========================================================
void CPlayer::AddDamage(int nDamage)
{
	CSound *pSound = CManager::GetSound();

	CExplosion *pExplosion;
	m_nLife -= nDamage;

	// �̗͎擾
	CLife *pLife = CLife::GetLife();

	// �_���[�W�ʂ𔽉f
	pLife->AddLife(m_nLife);

	// �̗͂��O�ɂȂ�����
	if (m_nLife <= 0)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_MEDIC);

		// ��������
		pExplosion = CExplosion::Create(CExplosion::EXPLOSIONTYPE_PLAYER_EFFECT);
		pExplosion->SetPos(m_pos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);
		Release();
	}
}

void CPlayer::PLAYERDASH(void)
{
	m_state = PLAYERSTATE_DASH;
}

//���C�t�ǉ�
void CPlayer::AddLife(int nLife)
{
	m_nLife += nLife;
}

// ==========================================================
// �̗͎擾
// ==========================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

// ==========================================================
// �̗͎擾
// ==========================================================
int CPlayer::GetDirectionMove(void)
{
	return m_nDirectionMove;
}

// ==========================================================
// �X�L�����擾
// ==========================================================
int CPlayer::GetCntWaterspout(void)
{
	return m_nCntWaterspout;
}

// ==========================================================
// �X�L�����ݒ�
// ==========================================================
void CPlayer::SetCntWaterspout(int nCntWaterspout)
{
	m_nCntWaterspout = nCntWaterspout;
}
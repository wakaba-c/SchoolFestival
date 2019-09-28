// ==========================================================
//
// �X�e�[�g[playerState.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "playerState.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CPlayerState *CPlayerState::m_pPlayerState;
LPDIRECT3DTEXTURE9 CPlayerState::m_pTex[STATETYPE_MAX] = {};
CPlayerState::STATETYPE CPlayerState::m_state = STATETYPE_PLAYER;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CPlayerState::CPlayerState(PRIORITY type = CScene::PRIORITY_EXPLOSION) :CScene2D(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CPlayerState::~CPlayerState()
{

}

// ==========================================================
// �����̏�����
// ==========================================================
void CPlayerState::Init(void)
{
	CScene2D::Init();
	
	if (m_state == STATETYPE_UP)
	{
		SetAnimation(STATE_X, STATE_Y, 0.0f, 0);
	}
	else if (m_state == STATETYPE_BARRIER)
	{
		SetAnimation(0.2f, STATE_Y, 0.0f, 0);
	}
}

// ==========================================================
// �����̏I��
// ==========================================================
void CPlayerState::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// �����̍X�V
// ==========================================================
void CPlayerState::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// �����̃p�^�[���؂�ւ�
	m_nCounterAnim++;

	if (m_state == STATETYPE_UP)
	{
		if (m_nCounterAnim % EXPLOSION_ANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % EXPLOSION_ANIM_INTERVAL;

			SetAnimation(STATE_X, STATE_Y, 0.0f, m_nPatternAnim);

			SetPos(pos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);

			/*	if (m_nPatternAnim % EXPLOSION_ANIM_COUNT == 0)
			{
			Release();
			}*/
		}

	}
	else if (m_state == STATETYPE_BARRIER)
	{
		if (m_nCounterAnim % EXPLOSION_ANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % EXPLOSION_ANIM_INTERVAL;

			SetAnimation(0.2f, STATE_Y, 0.0f, m_nPatternAnim);

			/*	if (m_nPatternAnim % EXPLOSION_ANIM_COUNT == 0)
			{
			Release();
			}*/
		}

		CPlayer *pPlayer = CGame::GetPlayer();

		D3DXVECTOR3 playerPos = pPlayer->GetPosition();

		SetPos(playerPos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);
	}

	CScene2D::Update();
}

// ==========================================================
// �����̕`��
// ==========================================================
void CPlayerState::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// �����̐���
// ==========================================================
CPlayerState *CPlayerState::Create(STATETYPE type)
{
	m_state = type;
	// �V�[�����I�Ɋm��
	m_pPlayerState = new CPlayerState(CScene::PRIORITY_EXPLOSION);

	// �V�[��������
	m_pPlayerState->Init();

	m_pPlayerState->BindTex(m_pTex[type]);

	// �l��Ԃ�
	return m_pPlayerState;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CPlayerState::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_UP, &m_pTex[STATETYPE_UP]);
	D3DXCreateTextureFromFile(pDevice, PLAYER_STATE, &m_pTex[STATETYPE_BARRIER]);
	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CPlayerState::Unload(void)
{
	for (int nCnt = 0; nCnt < STATETYPE_MAX; nCnt++)
	{
		// �e�N�X�`�����
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}
// ==========================================================
//
// ����[explsion.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "explosion.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CExplosion *CExplosion::m_pExplosion;
LPDIRECT3DTEXTURE9 CExplosion::m_pTex[EXPLOSIONTYPE_MAX] = {};

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CExplosion::CExplosion(PRIORITY type = CScene::PRIORITY_WATERSPOUT):CScene2D(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CExplosion::~CExplosion()
{

}

// ==========================================================
// �����̏�����
// ==========================================================
void CExplosion::Init(void)
{
	CScene2D::Init();
	SetAnimation(EXPLOSIONTEX_X, EXPLOSIONTEX_Y, 0.0f, 0);
}

// ==========================================================
// �����̏I��
// ==========================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// �����̍X�V
// ==========================================================
void CExplosion::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// �����̃p�^�[���؂�ւ�
	m_nCounterAnim++;

	if (m_nCounterAnim % EXPLOSION_ANIM_INTERVAL == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % EXPLOSION_ANIM_INTERVAL;

		SetAnimation(EXPLOSIONTEX_X, EXPLOSIONTEX_Y, 0.0f, m_nPatternAnim);

		SetPos(pos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);

	/*	if (m_nPatternAnim % EXPLOSION_ANIM_COUNT == 0)
		{
			Release();
		}*/
	}


	CPlayer *pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 playerPos = pPlayer->GetPosition();

	SetPos(playerPos + D3DXVECTOR3(-50.0f, 0.0f, 0.0f), COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);

	CScene2D::Update();
}

// ==========================================================
// �����̕`��
// ==========================================================
void CExplosion::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// �����̐���
// ==========================================================
CExplosion *CExplosion::Create(EXPLOSIONTYPE type)
{
	// �V�[�����I�Ɋm��
	m_pExplosion = new CExplosion(CScene::PRIORITY_WATERSPOUT);

	// �V�[��������
	m_pExplosion->Init();

	m_pExplosion->BindTex(m_pTex[type]);

	// �l��Ԃ�
	return m_pExplosion;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CExplosion::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_EFFECT, &m_pTex[EXPLOSIONTYPE_PLAYER_EFFECT]);
	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CExplosion::Unload(void)
{
	for (int nCnt = 0; nCnt < EXPLOSIONTYPE_MAX; nCnt++)
	{
		// �e�N�X�`�����
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}
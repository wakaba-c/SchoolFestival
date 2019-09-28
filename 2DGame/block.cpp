// ==========================================================
//
// �u���b�N[block.cpp]
// Author:Seiya Takahashi
//
// ==========================================================
#include "block.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "effect.h"
#include "player.h"
#include "game.h"
#include "inputKeyboard.h"
#include "sound.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CBlock *CBlock::m_pBlock;
LPDIRECT3DTEXTURE9 CBlock::m_pTex = NULL;
int CBlock::m_nNumBlock = 0;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CBlock::CBlock(PRIORITY type = CScene::PRIORITY_BLOCK) :CScene2D(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CBlock::~CBlock()
{

}

// ==========================================================
// �e�̏�����
// ==========================================================
void CBlock::Init(void)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CScene2D::Init();
	SetObjType(OBJTYPE_BLOCK);
}

// ==========================================================
// �e�̏I��
// ==========================================================
void CBlock::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// �e�̍X�V
// ==========================================================
void CBlock::Update(void)
{
	// �v���C���[���擾
	CPlayer *pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPosition();

	// �|�W�V�����擾
	m_pos = GetPos();

	m_posOld = m_pos;

	m_pos.x -= 5.0f;

	// �ʒu���擾
	SetPos(m_pos, BLOCKSIZE_X, BLOCKSIZE_Y, CENTERPOS_NORMAL);

	CScene2D::Update();
}

// ==========================================================
// �e�̕`��
// ==========================================================
void CBlock::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// �u���b�N�̏��擾
// ==========================================================
CBlock * CBlock::GetBlock(void)
{
	return m_pBlock;
}

// ==========================================================
// �e�̐���
// ==========================================================
CBlock *CBlock::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pBlock = new CBlock(CScene::PRIORITY_BLOCK);

	// �V�[��������
	m_pBlock->Init();

	m_pBlock->BindTex(m_pTex);

	// �l��Ԃ�
	return m_pBlock;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CBlock::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE, &m_pTex);

	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CBlock::Unload(void)
{
	// �e�N�X�`�����
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// �e�̌���
// ==========================================================
void CBlock::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

// ==========================================================
// �e�̌���
// ==========================================================
void CBlock::SetBlockNum(int nNumBlock)
{
	m_nNumBlock = nNumBlock;
}

// ==========================================================
// �u���b�N�̌����擾
// ==========================================================
int CBlock::GetBlockNum(void)
{
	return m_nNumBlock;
}

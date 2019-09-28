// ==========================================================
//
// ����[number.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTex = NULL;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CNumber::CNumber()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CNumber::~CNumber()
{

}

// ==========================================================
// ����������
// ==========================================================
void CNumber::Init(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// ���_���W
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (NUMBERSIZE_X / 2), m_pos.y - (NUMBERSIZE_Y / 2), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (NUMBERSIZE_X / 2), m_pos.y - (NUMBERSIZE_Y / 2), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (NUMBERSIZE_X / 2), m_pos.y + (NUMBERSIZE_Y / 2), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (NUMBERSIZE_X / 2), m_pos.y + (NUMBERSIZE_Y / 2), 0);
	// 1.0�ŌŒ�A�������W
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;
	// ���_�J���[
	m_pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �����I��
// ==========================================================
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ==========================================================
// �����X�V
// ==========================================================
void CNumber::Update(void)
{

}

// ==========================================================
// �����`�揈��
// ==========================================================
void CNumber::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, m_pTex);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ==========================================================
// ��������
// ==========================================================
CNumber *CNumber::Create(void)
{
	// �V�[�����I�Ɋm��
	CNumber *pNumber = new CNumber;

	// �V�[��������
	pNumber->Init();

	// �l��Ԃ�
	return pNumber;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CNumber::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE, &m_pTex);
	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CNumber::Unload(void)
{
	// �e�N�X�`�����
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// �����ݒ�
// ==========================================================
void CNumber::SetNumber(int nNumber)
{
	int nNum = nNumber;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(nNum*0.1f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(nNum *0.1f + 0.1f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(nNum* 0.1f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(nNum * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �����z�u
// ==========================================================
void CNumber::SetPosNum(D3DXVECTOR3 pos, int nSizeX, int nSizeY)
{
	m_pos = pos;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// ���_���W
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (nSizeX / 2), m_pos.y - (nSizeY / 2), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (nSizeX / 2), m_pos.y - (nSizeY / 2), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (nSizeX / 2), m_pos.y + (nSizeY / 2), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (nSizeX / 2), m_pos.y + (nSizeY / 2), 0);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}
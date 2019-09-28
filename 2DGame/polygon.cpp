// ==========================================================
//
// �|���S��[polygon.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "polygon.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTex = NULL;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CPolygon::CPolygon()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0, 0, 0, 0);
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CPolygon::~CPolygon()
{

}

// ==========================================================
// ����������
// ==========================================================
void CPolygon::Init(void)
{
	m_col = D3DXCOLOR(0, 0, 0, 0);

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
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y , 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y , 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_nSizeY, 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_nSizeY, 0);
	// 1.0�ŌŒ�A�������W
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;
	// ���_�J���[
	m_pVtx[0].col = m_col;
	m_pVtx[1].col = m_col;
	m_pVtx[2].col = m_col;
	m_pVtx[3].col = m_col;
	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �����I��
// ==========================================================
void CPolygon::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

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
void CPolygon::Update(void)
{

}

// ==========================================================
// �����`�揈��
// ==========================================================
void CPolygon::Draw(void)
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
CPolygon *CPolygon::Create(void)
{
	// �V�[�����I�Ɋm��
	CPolygon *pPolygon = new CPolygon;

	// �V�[��������
	pPolygon->Init();

	// �l��Ԃ�
	return pPolygon;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CPolygon::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_POLYGON, &m_pTex);
	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CPolygon::Unload(void)
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
void CPolygon::SetPolygon(D3DXVECTOR3 pos, float fWidth, int nSizeY)
{
	m_pos = pos;
	m_fWidth = fWidth;
	m_nSizeY = nSizeY;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// ���_���W
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y , 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x+m_fWidth, m_pos.y , 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y  + m_nSizeY, 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y  + m_nSizeY, 0);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �̗͐ݒ�
// ==========================================================
void CPolygon::SetLife(float fWidth)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// ���_���W
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y , 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_nSizeY, 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + m_nSizeY, 0);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// ����
// ==========================================================
void CPolygon::SetColPolygon(D3DXCOLOR col)
{
	m_col = col;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// ���_�J���[
	m_pVtx[0].col = m_col;
	m_pVtx[1].col = m_col;
	m_pVtx[2].col = m_col;
	m_pVtx[3].col = m_col;

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}
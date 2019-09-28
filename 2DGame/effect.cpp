// ==========================================================
//
// �G�t�F�N�g[effect.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CEffect *CEffect::m_pEffect;
LPDIRECT3DTEXTURE9 CEffect::m_pTex = NULL;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CEffect::CEffect(PRIORITY type = CScene::PRIORITY_EFFECT):CScene2D(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CEffect::~CEffect()
{

}

// ==========================================================
// �G�t�F�N�g�̏�����
// ==========================================================
void CEffect::Init(void)
{
	CScene2D::Init();
	SetAnimation(ATTACKENEMYTEX_X, ATTACKENEMYTEX_Y, 0.0f, 0);
}

// ==========================================================
// �G�t�F�N�g�̏I��
// ==========================================================
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// �G�t�F�N�g�̍X�V
// ==========================================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// �����̃p�^�[���؂�ւ�
	m_nCounterAnim++;

	if (m_nCounterAnim % EFFECTANIM_INTERVAL == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % EFFECTANIM_INTERVAL;

		SetAnimation(ATTACKENEMYTEX_X, ATTACKENEMYTEX_Y, 0.0f, m_nPatternAnim);

		SetPos(pos, EFFECTSIZE_X, EFFECTSIZE_Y, CENTERPOS_NORMAL);
	}

	CScene2D::Update();
}

// ==========================================================
// �G�t�F�N�g�̕`��
// ==========================================================
void CEffect::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ==========================================================
// �G�t�F�N�g�̐���
// ==========================================================
CEffect *CEffect::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pEffect = new CEffect(CScene::PRIORITY_EFFECT);

	// �V�[��������
	m_pEffect->Init();

	m_pEffect->BindTex(m_pTex);

	// �l��Ԃ�
	return m_pEffect;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CEffect::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &m_pTex);
	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CEffect::Unload(void)
{
	// �e�N�X�`�����
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// �|�W�V�����ړ��֐�
// ==========================================================
void CEffect::MovePos(D3DXVECTOR3 pos, float fMove)
{
	m_pEffect->SetPos(D3DXVECTOR3(pos.x - fMove, pos.y, pos.z), EFFECTSIZE_X, EFFECTSIZE_Y, CENTERPOS_NORMAL);
}
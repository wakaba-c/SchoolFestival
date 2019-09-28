// ==========================================================
//
// �w�i[bg.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "bg.h"
#include "renderer.h"
#include "manager.h"
#include "logo.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CBg *CBg::m_pBg;
LPDIRECT3DTEXTURE9 CBg::m_pTex = NULL;
CBg::RESULTSTATE CBg::m_ResultState = RESULTSTATE_CLEAR;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CBg::CBg(PRIORITY type = CScene::PRIORITY_BG):CScene2D(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CBg::~CBg()
{

}

// ==========================================================
// �w�i�̏�����
// ==========================================================
void CBg::Init(void)
{
	m_move = D3DXVECTOR3(0, 0, 0);

	CScene2D::Init();
}

// ==========================================================
// �w�i�̏I��
// ==========================================================
void CBg::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// �w�i�̍X�V
// ==========================================================
void CBg::Update(void)
{
	D3DXVECTOR3 posBG;

	CScene2D::Update();

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		//�V�[��2D�X�V����
		CScene2D::Update();

		//�ʒu�擾
		posBG = GetPos();

		m_move.x += 0.005f;

		if (m_move.y > 1.0f)
		{
			m_move.y = 0.0f;
		}

		if (m_move.x > 1.0f)
		{
			m_move.x = 0.0f;
		}

		//�ʒu�̐ݒ�
		SetPos(posBG, SCREEN_WIDTH, SCREEN_HEIGHT, CENTERPOS_NORMAL);

		//�e�N�X�`��������
		SetMove(1.0f, 1.0f, m_move);
	}
}

// ==========================================================
// �w�i�̕`��
// ==========================================================
void CBg::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// �w�i�̐���
// ==========================================================
CBg *CBg::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pBg = new CBg(CScene::PRIORITY_BG);

	// �V�[��������
	m_pBg->Init();

	m_pBg->BindTex(m_pTex);

	// �l��Ԃ�
	return m_pBg;
}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CBg::Load(void)
{
	CLogo *pLogo;
	pLogo->Load();

	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (CManager::GetMode() == CManager::MODE_TITLE)
	{
		pLogo = CLogo::Create();
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTex);
	}
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTex);
	}
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_BG, &m_pTex);
	}
	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		if (m_ResultState == RESULTSTATE_CLEAR)
		{

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT, &m_pTex);
		}
		else if (m_ResultState == RESULTSTATE_FAILD)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, TEXTURE_FAILD, &m_pTex);
		}
	}
	if (CManager::GetMode() == CManager::MODE_RANKING)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTex);
	}

	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CBg::Unload(void)
{
	// �e�N�X�`�����
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// �w�i�ړ�
// ==========================================================
void CBg::MoveBg(D3DXVECTOR3 fMove)
{
	m_pBg->SetMove(1.0f, 1.0f, fMove);
}

// ==========================================================
// �X�e�[�g
// ==========================================================
void CBg::SetResultState(RESULTSTATE state)
{
	m_ResultState = state;
}

// ==========================================================
// �X�e�[�g�擾
// ==========================================================
CBg::RESULTSTATE CBg::GetResultState(void)
{
	return m_ResultState;
}
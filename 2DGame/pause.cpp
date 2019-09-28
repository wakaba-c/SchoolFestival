// ==========================================================
//
// �|�[�Y[pause.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "bg.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "game.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CPause::PAUSETYPE CPause::m_type = PAUSETYPE_PAUSE;
LPDIRECT3DTEXTURE9 CPause::m_pTex[PAUSETYPE_MAX] = {};
bool CPause::m_bEnter = false;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CPause::CPause(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CPause::~CPause()
{

}

// ==========================================================
// ������
// ==========================================================
void CPause::Init(void)
{
	m_bEnter = false;
	CBg::RESULTSTATE state = CBg::GetResultState();
	for (int nCnt = 0; nCnt < PAUSETYPE_MAX; nCnt++)
	{
		m_pScene2D[nCnt] = CScene2D::Create();
		m_pScene2D[nCnt]->BindTex(m_pTex[nCnt]);

		switch (nCnt)
		{
		case PAUSETYPE_PAUSE:
			SetLogo(D3DXVECTOR3(PAUSE_POSX, PAUSE_POSY, 0.0f), PAUSE_X, PAUSE_Y, PAUSETYPE_PAUSE);
			break;

		case PAUSETYPE_TITLE:
			SetLogo(D3DXVECTOR3(TITlE_POSX, TITlE_POSY, 0.0f), TITlE_X, TITlE_Y, PAUSETYPE_TITLE);
			break;

		case PAUSETYPE_TUDUKERU:
			SetLogo(D3DXVECTOR3(TUDUKERU_POSX, TUDUKERU_POSY, 0.0f), TUDUKERU_X, TUDUKERU_Y, PAUSETYPE_TUDUKERU);
			break;

		case PAUSETYPE_MARK:
			SetLogo(D3DXVECTOR3(SAKANA_POSX, SAKANA_POSY, 0.0f), SAKANA_X, SAKANA_Y, PAUSETYPE_MARK);
			break;
		}
	}

}

// ==========================================================
// �I��
// ==========================================================
void CPause::Uninit(void)
{

}

// ==========================================================
// �X�V
// ==========================================================
void CPause::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CFade::FADE Fade = CFade::GetFade();

	//�I�����̖߂�����
	if (m_nSelect < 0)
	{
		m_nSelect = 1;
	}
	if (m_nSelect > 1)
	{
		m_nSelect = 0;
	}

	//�I�����̏���
	if (pInputKeyboard->GetKeyboardTngger(DIK_DOWNARROW))
	{
		m_nSelect += 1;
	}
	if (pInputKeyboard->GetKeyboardTngger(DIK_UPARROW))
	{
		m_nSelect -= 1;
	}

	//�I�����̕\������
	if (m_nSelect == 0)
	{
		m_pScene2D[PAUSETYPE_MARK]->SetPos(D3DXVECTOR3(SAKANA_POSX, SAKANA_POSY, 0), SAKANA_X, SAKANA_Y, 2);
	}
	else if (m_nSelect == 1)
	{
		m_pScene2D[PAUSETYPE_MARK]->SetPos(D3DXVECTOR3(SAKANA_POSX_2, SAKANA_POSY_2, 0), SAKANA_X, SAKANA_Y, 2);
	}

	if (pInputKeyboard->GetKeyboardTngger(DIK_RETURN))
	{
		if (m_nSelect == 0)
		{
			SetPause(false);
			CGame::SetGameState(CGame::GAMESTATE_NORMAL);
		}
		else if (m_nSelect == 1)
		{
			CFade::SetFade(CManager::MODE_TITLE);
			SetPause(false);
		}
	}

	if (m_bPause == false)
	{
		Release();
	}
}

// ==========================================================
// �`��
// ==========================================================
void CPause::Draw(void)
{

}

// ==========================================================
// ����
// ==========================================================
CPause *CPause::Create(void)
{
	// �V�[�����I�Ɋm��
	CPause *pPause = new CPause(CScene::PRIORITY_UI);

	// �V�[��������
	pPause->Init();

	// �l��Ԃ�
	return pPause;
}

// ==========================================================
// ���
// ==========================================================
void CPause::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{

}

// ==========================================================
// �e�N�X�`�����[�h
// ==========================================================
HRESULT CPause::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE, &m_pTex[PAUSETYPE_PAUSE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE_TITLE, &m_pTex[PAUSETYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TUDUKERU, &m_pTex[PAUSETYPE_TUDUKERU]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MARK, &m_pTex[PAUSETYPE_MARK]);

	return S_OK;
}

// ==========================================================
// �j��
// ==========================================================
void CPause::Unload(void)
{
	for (int nCnt = 0; nCnt < PAUSETYPE_MAX; nCnt++)
	{
		// �e�N�X�`�����
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ==========================================================
// �^�C�v
// ==========================================================
CPause::PAUSETYPE CPause::GetType(void)
{
	return m_type;
}

// ==========================================================
// �쐬
// ==========================================================
void CPause::SetLogo(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt)
{
	m_pScene2D[nCnt]->SetPos(pos, fSizeX, fSizeY, CENTERPOS_NORMAL);
}

//========================================================================================
// �|�[�Y�ݒ�
//========================================================================================
void CPause::SetPause(bool bPause)
{
	m_bPause = bPause;
}
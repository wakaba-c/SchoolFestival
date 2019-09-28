// ==========================================================
//
// �`��[scene.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "scene.h"
#include "renderer.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_POLGON] = {};
int CScene::m_nNumAll = 0;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CScene::CScene(PRIORITY type)
{
	for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
	{
		if (m_apScene[type][nCnt] == NULL)
		{
			m_apScene[type][nCnt] = this;
			m_nNumAll++;
			m_nID = nCnt;
			m_Priority = type;
			break;
		}
	}
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CScene::~CScene()
{

}

// ==========================================================
//
// �S�Ă̍X�V
//
// ==========================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				m_apScene[nCntPriority][nCnt]->Update();
			}
		}
	}
}

// ==========================================================
//
// �S�Ă̕`��
//
// ==========================================================
void CScene::DrewAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				m_apScene[nCntPriority][nCnt]->Draw();
			}
		}
	}
}

// ==========================================================
// ���ׂĔj��
// ==========================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				m_apScene[nCntPriority][nCnt]->Release();
			}
		}
	}
}

// ==========================================================
// �j��
// ==========================================================
void CScene::Release(void)
{
	int nData = m_nID;
	int nPriority = m_Priority;

	if (m_apScene[nPriority][nData] != NULL)
	{
 		m_apScene[nPriority][nData]->Uninit();
		// �J��
		delete m_apScene[nPriority][nData];
		m_apScene[nPriority][nData] = NULL;
		m_nNumAll--;
	}
}

// ==========================================================
// �擾
// ==========================================================
CScene *CScene::GetScene(int nPriority, int nCntScene)
{
	return m_apScene[nPriority][nCntScene];
}

// ==========================================================
// �^�C�v�Z�b�g
// ==========================================================
void CScene::SetObjType(OBJTYPE type)
{
	m_type = type;
}

// ==========================================================
// �^�C�v�擾
// ==========================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_type;
}
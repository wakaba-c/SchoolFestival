// ==========================================================
//
// ���R�[�h[record.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "record.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
int CRecord::m_nSavePoint[SAVE_DATA] = {};
int CRecord::m_nLoadPoint[SAVE_DATA] = {};

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CRecord::CRecord()
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CRecord::~CRecord()
{

}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
void CRecord::InitSave(void)
{
	int nCntSave;

	for (nCntSave = 0; nCntSave < SAVE_DATA; nCntSave++)
	{
		m_nSavePoint[nCntSave] = 0;
	}

}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
void CRecord::InitLoad(void)
{
	int nCntLoad;

	for (nCntLoad = 0; nCntLoad < LOAD_DATA; nCntLoad++)
	{
		m_nLoadPoint[nCntLoad] = 0;
	}

	LoadData();
}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
void CRecord::LoadData(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	int nCntLoad;

	// �t�@�C���J
	pFile = fopen(SAVE_FILE_NAME, "r");

	// �J����
	if (pFile != NULL)
	{
		for (nCntLoad = 0; nCntLoad < LOAD_DATA; nCntLoad++)
		{
			// fscanf(pFile, "%s", &g_aLoad[nCntLoad].aName[0]); // ���O
			fscanf(pFile, "%d", &m_nLoadPoint[nCntLoad]); // ���O
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		printf("�J����܂���ł���\n");
	}
}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
void CRecord::SaveData(int nPoint)
{
	// �t�@�C���|�C���g
	FILE *pFile;
	int nDate;
	int nCntSave;
	int nCntNum;

	for (nCntSave = 0; nCntSave < SAVE_DATA; nCntSave++)
	{
		m_nSavePoint[nCntSave] = m_nLoadPoint[nCntSave];
	}
	m_nSavePoint[5] = nPoint;

	for (nCntSave = 0; nCntSave < SAVE_DATA + 1; nCntSave++)
	{
		for (nCntNum = nCntSave + 1; nCntNum < SAVE_DATA + 1; nCntNum++)
		{
			if (m_nSavePoint[nCntSave] < m_nSavePoint[nCntNum])
			{
				nDate = m_nSavePoint[nCntNum];
				m_nSavePoint[nCntNum] = m_nSavePoint[nCntSave];
				m_nSavePoint[nCntSave] = nDate;
			}
		}
	}

	// �t�@�C���J
	pFile = fopen(SAVE_FILE_NAME, "w");

	// �J����
	if (pFile != NULL)
	{
		for (nCntSave = 0; nCntSave < SAVE_DATA; nCntSave++)
		{
			// fprintf(pFile, "%s\n", &g_aSave[nCntSave].aName[0]);	// ����
			fprintf(pFile, "%d\n\n", m_nSavePoint[nCntSave]);	// �|�C���g
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		printf("�J����܂���ł���\n");
	}

}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
int *CRecord::GetSave(void)
{
	return &m_nSavePoint[0];
}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
int *CRecord::GetLoad(int nLoad)
{
	return &m_nLoadPoint[nLoad];
}
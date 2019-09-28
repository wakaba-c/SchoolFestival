// ==========================================================
//
// レコード[record.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "record.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
int CRecord::m_nSavePoint[SAVE_DATA] = {};
int CRecord::m_nLoadPoint[SAVE_DATA] = {};

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CRecord::CRecord()
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CRecord::~CRecord()
{

}

// ==========================================================
// スコアの初期化
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
// スコアの初期化
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
// スコアの初期化
// ==========================================================
void CRecord::LoadData(void)
{
	// ファイルポイント
	FILE *pFile;

	int nCntLoad;

	// ファイル開
	pFile = fopen(SAVE_FILE_NAME, "r");

	// 開けた
	if (pFile != NULL)
	{
		for (nCntLoad = 0; nCntLoad < LOAD_DATA; nCntLoad++)
		{
			// fscanf(pFile, "%s", &g_aLoad[nCntLoad].aName[0]); // 名前
			fscanf(pFile, "%d", &m_nLoadPoint[nCntLoad]); // 名前
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		printf("開けれませんでした\n");
	}
}

// ==========================================================
// スコアの初期化
// ==========================================================
void CRecord::SaveData(int nPoint)
{
	// ファイルポイント
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

	// ファイル開
	pFile = fopen(SAVE_FILE_NAME, "w");

	// 開けた
	if (pFile != NULL)
	{
		for (nCntSave = 0; nCntSave < SAVE_DATA; nCntSave++)
		{
			// fprintf(pFile, "%s\n", &g_aSave[nCntSave].aName[0]);	// 氏名
			fprintf(pFile, "%d\n\n", m_nSavePoint[nCntSave]);	// ポイント
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		printf("開けれませんでした\n");
	}

}

// ==========================================================
// スコアの初期化
// ==========================================================
int *CRecord::GetSave(void)
{
	return &m_nSavePoint[0];
}

// ==========================================================
// スコアの初期化
// ==========================================================
int *CRecord::GetLoad(int nLoad)
{
	return &m_nLoadPoint[nLoad];
}
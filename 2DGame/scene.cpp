// ==========================================================
//
// 描画[scene.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "scene.h"
#include "renderer.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_POLGON] = {};
int CScene::m_nNumAll = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
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
// デストラクタ
//
// ==========================================================
CScene::~CScene()
{

}

// ==========================================================
//
// 全ての更新
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
// 全ての描画
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
// すべて破棄
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
// 破棄
// ==========================================================
void CScene::Release(void)
{
	int nData = m_nID;
	int nPriority = m_Priority;

	if (m_apScene[nPriority][nData] != NULL)
	{
 		m_apScene[nPriority][nData]->Uninit();
		// 開放
		delete m_apScene[nPriority][nData];
		m_apScene[nPriority][nData] = NULL;
		m_nNumAll--;
	}
}

// ==========================================================
// 取得
// ==========================================================
CScene *CScene::GetScene(int nPriority, int nCntScene)
{
	return m_apScene[nPriority][nCntScene];
}

// ==========================================================
// タイプセット
// ==========================================================
void CScene::SetObjType(OBJTYPE type)
{
	m_type = type;
}

// ==========================================================
// タイプ取得
// ==========================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_type;
}
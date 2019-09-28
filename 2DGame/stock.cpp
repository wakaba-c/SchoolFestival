// ==========================================================
//
// �X�g�b�N[stock.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "stock.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CStock *CStock::m_pStock;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CStock::CStock(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{
	m_nStock = NUM_STOCK;
}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CStock::~CStock()
{

}

// ==========================================================
// �X�R�A�̏�����
// ==========================================================
void CStock::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create();
		m_apNumber[nCnt]->SetPosNum(D3DXVECTOR3(STOCK_POS_X + STOCK_INTERVAL_X * nCnt, STOCK_POS_Y, 0.0f), NUMBERSIZE_X, NUMBERSIZE_Y);
	}
	m_apNumber[1]->SetNumber(m_nStock);
}

// ==========================================================
// �X�R�A�̏I��
// ==========================================================
void CStock::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
		}
	}
}

// ==========================================================
// �X�R�A�̍X�V
// ==========================================================
void CStock::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Update();
		}
	}
}

// ==========================================================
// �X�R�A�̕`��
// ==========================================================
void CStock::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_STOCK; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

// ==========================================================
// �X�R�A�̐���
// ==========================================================
CStock *CStock::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pStock = new CStock(CScene::PRIORITY_UI);

	// �V�[��������
	m_pStock->Init();

	return m_pStock;
}

// ==========================================================
// �X�R�A�ݒ�
// ==========================================================
void CStock::AddStock(int nValue)
{
	int nCount;
	int nNumber;

	m_nStock += nValue;

	for (nCount = 0; nCount < MAX_STOCK; nCount++)
	{
		nNumber = m_nStock % (int)powf(STOCK_INTERVAL, (float)MAX_STOCK - nCount) / (int)powf(STOCK_INTERVAL, (float)MAX_STOCK - 1.0f - nCount);
		if (m_nStock <= 0)
		{
			m_nStock = 0;
		}
		m_apNumber[nCount]->SetNumber(nNumber);
	}
}

// ==========================================================
// �X�R�A���擾
// ==========================================================
CStock *CStock::GetStock(void)
{
	return m_pStock;
}

// ==========================================================
// �X�R�A���擾
// ==========================================================
void CStock::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	m_pos = pos;
}

// ==========================================================
// �X�R�A���擾
// ==========================================================
void CStock::SetStock(int nStock)
{
	m_nStock = nStock;
}
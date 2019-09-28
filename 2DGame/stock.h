// ==========================================================
//
// �X�g�b�N[stock.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "main.h"
#include "scene.h"

#define MAX_STOCK (2)
#define NUM_STOCK (3)
#define STOCK_POS_X (850.0f)
#define STOCK_POS_Y (50.0f)
#define STOCK_INTERVAL_X (33)
#define STOCK_INTERVAL (10.0f)

class CNumber;

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CStock : public CScene
{
public:

	CStock(PRIORITY type);			// �R���X�g���N�^
	~CStock();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void AddStock(int nValue);
	void SetStock(int nStock);

	static CStock *GetStock(void);
	static CStock *Create(void);

protected:

private:
	static CStock *m_pStock;

	CNumber *m_apNumber[MAX_STOCK];
	D3DXVECTOR3 m_pos;

	int	m_nStock;				// �X�R�A
};
#endif
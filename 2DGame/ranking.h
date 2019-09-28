// ==========================================================
//
// �����L���O[ranking.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

#define	RANKING_BG_X (640.0f)
#define	RANKING_BG_Y (360.0f)
#define	TIME_COUNT (600)

class CBg;

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CRanking
{
public:
	CRanking();					// �R���X�g���N�^
	~CRanking();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static CRanking *Create(void);
protected:

private:
	CBg *m_pBg;

	static int m_nTime;
};
#endif
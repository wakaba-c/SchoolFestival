// ==========================================================
//
// �����L���O����[rankingNum.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RANKINGNUM_H_
#define _RANKINGNUM_H_

#include "main.h"
#include "scene.h"

#define RANK_NUM (5)
#define RANK_SCORE (8)
#define NUM_POS_X (520.0f)
#define NUM_POS_Y (150.0f)
#define NUM_INTERVAL_X (44)
#define NUM_INTERVAL_Y (100)
#define NUM_SIZE_X (44)
#define NUM_SIZE_Y (70)

class CNumber;
// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CRankingNum : public CScene
{
public:

	CRankingNum(PRIORITY type);			// �R���X�g���N�^
	~CRankingNum();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void AddRankingNum(int nValue);

	static CRankingNum *GetRank(void);
	static CRankingNum *Create(void);

protected:

private:
	static CRankingNum *m_pRankingNum;

	CNumber *m_apNumber[RANK_NUM][RANK_SCORE];
	D3DXVECTOR3 m_pos;

	int	m_nRanking[RANK_NUM];
	int m_nScore;
};
#endif
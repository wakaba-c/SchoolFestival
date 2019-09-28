// ==========================================================
//
// �X�R�A[score.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

#define MAX_SCORE (8)

#define SCORE_POS_X (1000.0f)
#define SCORE_POS_Y (50.0f)
#define SCORE_INTERVAL_X (33)
#define SCORE_INTERVAL (10.0f)

class CNumber;
// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CScore : public CScene
{
public:

	CScore(PRIORITY type);			// �R���X�g���N�^
	~CScore();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void AddScore(int nValue);
	void SetScore(int nScore);

	static int GetNowScore(void);
	static CScore *GetScore(void);
	static CScore *Create(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static CScore *m_pScore;
	static int	m_nScore;				// �X�R�A

	CNumber *m_apNumber[MAX_SCORE];
	D3DXVECTOR3 m_pos;
};
#endif
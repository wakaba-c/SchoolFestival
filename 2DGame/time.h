// ==========================================================
//
// �^�C��[time.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"
#include "scene.h"

#define MAX_TIME (2)
#define START_TIMU (60)
#define FRAME_TIMU (60)
#define TIME_POS_X (650.0f)
#define TIME_POS_Y (50.0f)
#define TIME_INTERVAL_X (33)
#define TIME_INTERVAL (10.0f)

class CNumber;
// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CTime : public CScene
{
public:

	CTime(PRIORITY type);			// �R���X�g���N�^
	~CTime();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetTime(int nTime);

	static CTime *Create(void);

protected:

private:
	DWORD m_dStartTime;
	CNumber *m_apNumber[MAX_TIME];
	D3DXVECTOR3 m_pos;

	int	m_nTime;				// �^�C��
};
#endif
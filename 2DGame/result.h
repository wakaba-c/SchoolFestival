// ==========================================================
//
// ���U���g[result.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

#define RESULT_BG_X (640.0f)
#define RESULT_BG_Y (360.0f)

class CBg;

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CResult
{
public:

	CResult();					// �R���X�g���N�^
	~CResult();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static CResult *Create(void);

protected:

private:
	static CBg *m_pBg;
};
#endif
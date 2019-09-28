// ==========================================================
//
// �`���[�g���A��[tutorial.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

#define TUTORIAL_BG_X (640.0f)
#define TUTORIAL_BG_Y (360.0f)

class CBg;

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CTutorial
{
public:

	CTutorial();					// �R���X�g���N�^
	~CTutorial();				// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static CTutorial *Create(void);

protected:

private:
	static CBg *m_pBg;
};
#endif
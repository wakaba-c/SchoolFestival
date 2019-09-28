// ==========================================================
//
// �t�F�[�h[fade.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

#define FADESIZE_X (1280)
#define FADESIZE_Y (720)

class CPolygon;

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CFade
{
public:
	//=============================================================================
	// �t�F�[�h�̏��
	//=============================================================================
	typedef enum
	{
		FADE_NONE = 0,		// �����Ȃ����
		FADE_IN,			// �t�F�[�h�C������
		FADE_OUT,			// �t�F�[�h�A�E�g����
		FADE_MAX
	} FADE;

	CFade();					// �R���X�g���N�^
	~CFade();					// �f�X�g���N�^

	void Init(CManager::MODE modeNext);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static FADE GetFade(void);
	static void SetFade(CManager::MODE modeNext);
	static CFade *Create(CManager::MODE mode);

protected:

private:
	static CFade *m_pFade;
	static FADE						m_fade;							// �t�F�[�h���
	static CManager::MODE			m_modeNext;					// ���̉�ʁi���[�h�j
	static D3DXCOLOR				m_colorFade;

	CPolygon *m_pPolygon;
};
#endif
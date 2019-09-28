// ==========================================================
//
// �w�i[bg.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "scene2D.h"

#define BGSIZE_X (1280)
#define BGSIZE_Y (720)

#define	TEXTURE_BG	"data/TEXTURE/bg_sabaku.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE	"data/TEXTURE/sabaku(�^�C�g��).png"				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TUTORIAL	"data/TEXTURE/�`���[�g���A���`�[��.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT	"data/TEXTURE/oasis.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_FAILD "data/TEXTURE/desert.jpg"

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CBg : public CScene2D
{
public:
	//=============================================================================
	// �\���̒�`
	//=============================================================================
	typedef enum
	{
		RESULTSTATE_CLEAR = 0,
		RESULTSTATE_FAILD,
		RESULTSTATE_MAX
	} RESULTSTATE;

	CBg(PRIORITY type);					// �R���X�g���N�^
	~CBg();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��

	static void SetResultState(RESULTSTATE state);
	static void MoveBg(D3DXVECTOR3 fMove);
	static CBg *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static RESULTSTATE GetResultState(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;
	static CBg *m_pBg;
	static RESULTSTATE m_ResultState;
	D3DXVECTOR3 m_move;
	float m_fXpos;
};
#endif
// ==========================================================
//
// �|�[�Y[pause.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene.h"


#define	TEXTURE_PAUSE		"data/TEXTURE/huruikami.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PAUSE_TITLE		"data/TEXTURE/taitoru.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TUDUKERU	"data/TEXTURE/tudukeru.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_MARK		"data/TEXTURE/nikukyuu.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define PAUSE_X (500)									// X���W
#define PAUSE_Y (720)									// Y���W
#define PAUSE_POSX (640.0f)									// X���W
#define PAUSE_POSY (360.0f)									// Y���W
#define TITlE_X (500)									// X���W
#define TITlE_Y (500)									// Y���W
#define TITlE_POSX (600.0f)									// X���W
#define TITlE_POSY (400.0f)									// Y���W
#define TUDUKERU_X (500)								// X���W
#define TUDUKERU_Y (500)								// Y���W
#define TUDUKERU_POSX (600.0f)								// X���W
#define TUDUKERU_POSY (200.0f)								// Y���W
#define SAKANA_X (100)									// X���W
#define SAKANA_Y (100)
#define SAKANA_POSX (750.0f)								
#define SAKANA_POSY (180.0f)
#define SAKANA_POSX_2 (800.0f)								
#define SAKANA_POSY_2 (380.0f)

class CScene2D;
// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CPause : public CScene
{
public:
	//=============================================================================
	// �\���̒�`
	//=============================================================================
	typedef enum
	{

		PAUSETYPE_PAUSE = 0,
		PAUSETYPE_TITLE,
		PAUSETYPE_TUDUKERU,
		PAUSETYPE_MARK,
		PAUSETYPE_MAX
	} PAUSETYPE;

	CPause(PRIORITY type);			// �R���X�g���N�^
	~CPause();					// �f�X�g���N�^

	void Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetLogo(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt);
	void SetPause(bool bPause);

	static CPause *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static PAUSETYPE GetType(void);

protected:

private:
	CScene2D *m_pScene2D[PAUSETYPE_MAX];

	static PAUSETYPE m_type;
	static LPDIRECT3DTEXTURE9 m_pTex[PAUSETYPE_MAX];
	static bool m_bEnter;

	bool m_bPause;
	int m_nSelect;
};
#endif
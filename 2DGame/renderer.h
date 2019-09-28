// ==========================================================
//
// �����_���[[renderer.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CFade;

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CRenderer
{
public:
	CRenderer();

	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawFPS(void);
	LPDIRECT3DDEVICE9 GetDevice(void);
protected:


private:
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
	LPDIRECT3D9 m_pD3D = NULL;
	LPD3DXFONT	m_pFont = NULL;					// �t�H���g�ւ̃|�C���^

	int			m_nCountFPS = 0;				// FPS�J�E���^
	int			m_nCntEffect = 0;

	static CFade *m_pFade;
};
#endif
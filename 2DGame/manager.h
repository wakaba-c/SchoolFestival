// ==========================================================
//
// 管理[manager.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class CRenderer;
class CInputKeyboard;
class CInputController;
class CGame;
class CTitle;
class CResult;
class CRanking;
class CRecord;
class CSound;
class CTutorial;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CManager
{
public:

	// ==========================================================
	// 画面モードの種類
	// ==========================================================
	typedef enum
	{
		MODE_NONE = -1,
		MODE_TITLE,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	} MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputController *GetInputController(void);
	static CSound *GetSound(void);
	static CGame *GetGame(void);
	static MODE GetMode(void);
	static void SetMode(MODE mode);

protected:

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputController *m_pInputController;
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CRanking *m_pRanking;
	static MODE m_mode;
	static CSound *m_pSound;
	static CTutorial *m_pTutorial;

	CRecord *m_pRecord;
};
#endif
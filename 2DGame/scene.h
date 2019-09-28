// ==========================================================
//
// 描画[scene.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CScene
{
public:

	//=============================================================================
	// オブジェクトの種類
	//=============================================================================
	typedef enum
	{
		PRIORITY_BG = 0,
		PRIORITY_EFFECT,
		PRIORITY_WATERSPOUT,
		PRIORITY_PLAYER,
		PRIORITY_BLOCK,
		PRIORITY_ITEM,
		PRIORITY_EXPLOSION,
		PRIORITY_UI,
		PRIORITY_MAX
	} PRIORITY;

	//=============================================================================
	// オブジェクトの種類
	//=============================================================================
	typedef enum
	{
		OBJTYPE_PLAYER = 0,
		OBJTYPE_BLOCK,
		OBJTYPE_ITEM,
		OBJTYPE_MAX
	} OBJTYPE;

	CScene(PRIORITY type);
	virtual~CScene();

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos) = 0;

	static void UpdateAll(void);
	static void DrewAll(void);
	static void ReleaseAll(void);

	CScene *GetScene(int nPriority, int nCntScene);
	void SetObjType(OBJTYPE type);
	OBJTYPE GetObjType(void);
protected:
	void Release(void);

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_POLGON];
	static int m_nNumAll;

	OBJTYPE m_type;
	PRIORITY m_Priority;		// 優先順位の番号

	int m_nID;
};
#endif
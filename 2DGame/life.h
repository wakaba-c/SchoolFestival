// ==========================================================
//
// 体力[life.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"
#include "scene.h"

#define MAX_LIFE (2)
#define WIDTH_SIZE (500)

class CPolygon;
// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CLife : public CScene
{
public:
	//=============================================================================
	// 体力の種類
	//=============================================================================
	typedef enum
	{
		LIFETYPE_PLAYER = 0,
		LIFETYPE_ENEMY,
		LIFETYPE_EGG,
		LIFETYPE_MAX
	} LIFETYPE;

	CLife(PRIORITY type);			// コンストラクタ
	~CLife();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetLife(int nLife);
	void AddLife(int nLife);

	static CLife *GetLife(void);
	static CLife *Create(void);

protected:

private:

	static CLife *m_pLife;

	LIFETYPE m_type;
	CPolygon *m_apPolygon[MAX_LIFE];
	D3DXVECTOR3 m_pos;

	int	m_nLife;				// 体力
	float m_fWidth;
	float m_fSize;
	float m_fLife;
};
#endif
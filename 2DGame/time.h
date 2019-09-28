// ==========================================================
//
// タイム[time.h]
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
// クラスの定義
//
// ==========================================================
class CTime : public CScene
{
public:

	CTime(PRIORITY type);			// コンストラクタ
	~CTime();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos);
	void SetTime(int nTime);

	static CTime *Create(void);

protected:

private:
	DWORD m_dStartTime;
	CNumber *m_apNumber[MAX_TIME];
	D3DXVECTOR3 m_pos;

	int	m_nTime;				// タイム
};
#endif
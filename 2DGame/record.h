// ==========================================================
//
// レコード[record.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _RECORD_H_
#define _RECORD_H_

#include "main.h"

#define SAVE_DATA (5)
#define LOAD_DATA (5)
#define SAVE_FILE_NAME "data/Ranking.txt"

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CRecord
{
public:

	CRecord();			// コンストラクタ
	~CRecord();					// デストラクタ

	void InitSave(void);			// 初期化
	void InitLoad(void);			// 初期化
	void LoadData(void);
	void SaveData(int nPoint);

	static int *GetSave(void);
	static int *GetLoad(int nLoad);

protected:

private:
	static int m_nSavePoint[SAVE_DATA];
	static int m_nLoadPoint[LOAD_DATA];
};
#endif
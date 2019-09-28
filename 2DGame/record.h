// ==========================================================
//
// ���R�[�h[record.h]
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
// �N���X�̒�`
//
// ==========================================================
class CRecord
{
public:

	CRecord();			// �R���X�g���N�^
	~CRecord();					// �f�X�g���N�^

	void InitSave(void);			// ������
	void InitLoad(void);			// ������
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
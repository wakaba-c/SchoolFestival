// ----------------------------------------------------------------------------
// サウンド処理 [sound.h]
// Author : Ryouma Inoue
//
// ----------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"


// ----------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------
#ifdef _XBOX //Big-Endian
#define FOURCC_RIFF	'RIFF'
#define FOURCC_DATA	'data'
#define FOURCC_FMT	'fmt '
#define FOURCC_WAVE	'WAVE'
#define FOURCC_XWMA	'XWMA'
#define FOURCC_DPDS	'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define	_FOURCC_RIFF	'FFIR'
#define	_FOURCC_DATA	'atad'
#define	_FOURCC_FMT		' tmf'
#define _FOURCC_WAVE	'EVAW'
#define _FOURCC_XWMA	'AMWX'
#define _FOURCC_DPDS	'sdpd'
#endif

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CSound
{
public:

	// ----------------------------------------------------------------------------
	// サウンドファイル
	// ----------------------------------------------------------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_LABEL_BGM003,			// BGM3
		SOUND_LABEL_SE_SHOT,		// 弾発射音
		SOUND_LABEL_SE_HIT,			// ヒット音
		SOUND_LABEL_SE_JUMP,		// ジャンプ音
		SOUND_LABEL_SE_ENTER,		// ENTER音
		SOUND_LABEL_SE_EXPLOSION,	// 爆発音
		SOUND_LABEL_SE_MEDIC,
		SOUND_LABEL_SE_SUN,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	CSound();			// コンストラクタ
	~CSound();					// デストラクタ


	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	HRESULT InitSound(HWND hWnd);
	HRESULT PlaySound(SOUND_LABEL label);

	void UninitSound(void);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

protected:

private:

	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX];					// 各音素材のパラメータ														
};

#endif

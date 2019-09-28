// ----------------------------------------------------------------------------
// �T�E���h���� [sound.h]
// Author : Ryouma Inoue
//
// ----------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"


// ----------------------------------------------------------------------------
// �}�N����`
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
// �N���X�̒�`
//
// ==========================================================
class CSound
{
public:

	// ----------------------------------------------------------------------------
	// �T�E���h�t�@�C��
	// ----------------------------------------------------------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_LABEL_BGM003,			// BGM3
		SOUND_LABEL_SE_SHOT,		// �e���ˉ�
		SOUND_LABEL_SE_HIT,			// �q�b�g��
		SOUND_LABEL_SE_JUMP,		// �W�����v��
		SOUND_LABEL_SE_ENTER,		// ENTER��
		SOUND_LABEL_SE_EXPLOSION,	// ������
		SOUND_LABEL_SE_MEDIC,
		SOUND_LABEL_SE_SUN,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	CSound();			// �R���X�g���N�^
	~CSound();					// �f�X�g���N�^


	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	HRESULT InitSound(HWND hWnd);
	HRESULT PlaySound(SOUND_LABEL label);

	void UninitSound(void);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

protected:

private:

	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX];					// �e���f�ނ̃p�����[�^														
};

#endif

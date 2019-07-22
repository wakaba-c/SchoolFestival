//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
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

//=============================================================================
// パラメータ構造体定義
//=============================================================================
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDPARAM;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//=============================================================================
// グローバル変数
//=============================================================================
SOUND g_aSound[SOUND_LABEL_MAX];							// サウンドの音量
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス

IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SubmixVoice		*g_pSubmixVoice = NULL;
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス

BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ

X3DAUDIO_HANDLE		g_X3DInstance;					// 3Dオーディオインスタンスハンドル
X3DAUDIO_LISTENER	g_Listener;						// リスナー（聞く）
X3DAUDIO_EMITTER	g_Emitter;						// エミッター （スピーカー）
X3DAUDIO_DSP_SETTINGS g_DSPSetting = {};					// 計算結果
FLOAT32				*matrix;
XAUDIO2_DEVICE_DETAILS deviceDetails;
DWORD channelMask;

DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

// 各音素材のパラメータ
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{ 
	{"data/sound/title.wav", -1},		// BGM0
	{"data/sound/game.wav", -1},		// BGM1
	{"data/se/jump.wav", 0},		// BGM2
	{ "data/se/Slash.wav", 0 },		// BGM2
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;
	int nCntSound;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// XAudio2オブジェクトの開放
		if(g_pXAudio2)
		{
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	g_pXAudio2->GetDeviceDetails(0, &deviceDetails);
	channelMask = deviceDetails.OutputFormat.dwChannelMask;

	// 3Dサウンドの初期化
	X3DAudioInitialize(channelMask,X3DAUDIO_SPEED_OF_SOUND, g_X3DInstance);

	 matrix = new FLOAT32[deviceDetails.OutputFormat.Format.nChannels];

	 // 計算結果の初期化
	 g_DSPSetting.SrcChannelCount = 1; 
	 g_DSPSetting.DstChannelCount = deviceDetails.OutputFormat.Format.nChannels;
	 g_DSPSetting.pMatrixCoefficients = matrix;

	// リスナーの初期化
	g_Listener = { 0 };

	// エミッターの初期化処理
	g_Emitter = { 0 };
	g_Emitter.ChannelCount = 1;
	g_Emitter.CurveDistanceScaler = FLT_MIN;

	//g_DSPSetting.SrcChannelCount = 1;
	//g_DSPSetting.DstChannelCount = 6;
	//g_DSPSetting.pMatrixCoefficients = matrix;

	// サウンドデータの初期化
	for(nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		// ファイルポインタを先頭に移動
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, _FOURCC_RIFF, &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != _FOURCC_WAVE)
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, _FOURCC_FMT, &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, _FOURCC_DATA, &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aParam[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	// XAudio2オブジェクトの開放
	if(g_pXAudio2)
	{
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// エミッターとリスナーの更新処理
//=============================================================================
void UpdateSound(void)
{
	XAUDIO2_FILTER_PARAMETERS g_FilterParameters;
	XAUDIO2_VOICE_USEFILTER;

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_aSound[nCntSound].bUse)
		{
			//エミッターの更新処理
			g_Emitter.OrientFront = g_aSound[nCntSound].rotation;			//向き
			g_Emitter.OrientTop = g_aSound[nCntSound].normalize;			//上向き
			g_Emitter.Position = g_aSound[nCntSound].position;				//位置
			g_Emitter.Velocity = g_aSound[nCntSound].velocity;				//速度

			//リスナーとエミッターの計算
			X3DAudioCalculate(g_X3DInstance, &g_Listener, &g_Emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER, &g_DSPSetting);

			//g_apSourceVoice[nCntSound]->SetOutputMatrix(g_pMasteringVoice, 1, deviceDetails.OutputFormat.Format.nChannels, g_DSPSetting.pMatrixCoefficients);
			g_apSourceVoice[nCntSound]->SetFrequencyRatio(g_DSPSetting.DopplerFactor);
			g_apSourceVoice[nCntSound]->SetOutputMatrix(g_pSubmixVoice, 1, deviceDetails.OutputFormat.Format.nChannels, g_DSPSetting.pMatrixCoefficients);
			g_FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * g_DSPSetting.LPFDirectCoefficient), 1.0f };
			//g_apSourceVoice[nCntSound]->SetFilterParameters(&g_FilterParameters);
		}
	}
}

//=============================================================================
// リスナーの更新処理
//=============================================================================
void UpdateListener(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 nor)
{
	g_Listener.OrientFront = rot;
	g_Listener.OrientTop = nor;
	g_Listener.Position = pos;
	g_Listener.Velocity = move;
}

//=============================================================================
// エミッターの更新処理
//=============================================================================
void UpdateEmitter(SOUND_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 nor)
{
	g_aSound[label].rotation = rot;
	g_aSound[label].normalize = nor;
	g_aSound[label].position = pos;
	g_aSound[label].velocity = move;
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label, bool Stereophonic)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	//3Dサウンドの有無
	if(Stereophonic)
	{
		g_aSound[label].bUse = Stereophonic;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aParam[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{ // 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	//3Dサウンドの有無
	if (g_aSound[label].bUse)
	{
		g_aSound[label].bUse = false;
	}

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{ // 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
			
			if (g_aSound[nCntSound].bUse)
			{
				g_aSound[nCntSound].bUse = false;
			}
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	// ファイルポインタを先頭に移動
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while(hr == S_OK)
	{
		// チャンクの読み込み
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		// チャンクデータの読み込み
		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case _FOURCC_RIFF:
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			// ファイルタイプの読み込み
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			// ファイルポインタをチャンクデータ分移動
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	// ファイルポインタを指定位置まで移動
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// データの読み込み
	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//=============================================================================
// ボリュームの調整
//=============================================================================
void SetVolume(SOUND_LABEL label, float fVolume)
{
	XAUDIO2_VOICE_STATE xa2state;

	g_aSound[label].SourceVoiceChannelVolumes = fVolume;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{ // 再生中
	  //ボリューム変更
		g_apSourceVoice[label]->SetChannelVolumes(label, &g_aSound[label].SourceVoiceChannelVolumes);
	}
}
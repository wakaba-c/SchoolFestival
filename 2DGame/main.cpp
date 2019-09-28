// ==========================================================
// 
// メイン[main.cpp]
// Author:Ryouma Inoue
// 
// ==========================================================
#include "main.h"
#include "manager.h"

// ==========================================================
// マクロ定義
// ==========================================================
#define CLASS_NAME	"WindowClass"				// ウィンドウクラスの名前
#define WINDOW_NAME	"守護者"					// ウィンドウの名前

// ==========================================================
// プロトタイプ宣言
// ==========================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ==========================================================
// グローバル変数
// ==========================================================
int g_nFpsCnt;

// ==========================================================
// 
// メイン関数
// 
// ==========================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	CManager *pManager;

	pManager = new CManager;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,							// 表示するウィンドウのスタイルを設定
		WindowProc,							// ウィンドウプロシージャのアドレス（関数名）を指定
		0,									// 通常使用しないので0指定
		0,									// 通常使用しないので0指定
		hInstance,							// Windowsの引数のインスタンスハンドル指定
		LoadIcon(NULL, IDI_INFORMATION),	// タスクバーに使用するアイコン指定
		LoadCursor(NULL, IDC_ARROW),		// 使用するマウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 1),			// ウィンドウクライアント領域の色収差
		NULL,								// メニューを指定
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)		// 小さいアイコンの指定
	};
	// ウィンドウハンドル（識別子）
	HWND hWnd;
	// メッセージを格納する変数
	MSG msg;
	// 現在時間
	DWORD dwCurrentTime;
	// 最後に処理した時間
	DWORD dwExeclastTime;
	// 描画数をカウント
	DWORD dwFrameCount;
	// FPSを最後に測定した時刻
	DWORD dwFPSLastTime;
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	// ウィンドの生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	pManager->Init(hInstance,hWnd, TRUE);

	// ウィンドウの表示
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);
	timeBeginPeriod(1);					// 分解能の設定
	dwCurrentTime = 0;
	dwExeclastTime = timeGetTime();		// 現在時間を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();
	// メッセージループ（メッセージキューからメッセージを取得）
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				// ウィンドウプロシージャにメッセージを送る
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
		 // システム時間取得
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
#ifdef _DEBUG
				// FPSを測定
				g_nFpsCnt = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
#endif // _DEBUG
				// FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;
				// カウントをクリア
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExeclastTime) >= (1000 / 60))
			{
				// 処理した時間で保存
				dwExeclastTime = dwCurrentTime;
				// 更新処理
				pManager->Update();
				// 描画処理
				pManager->Draw();
				dwFrameCount++;
			}
		}
	}
	if (pManager != NULL)
	{
		pManager->Uninit();
		
		delete pManager;
		pManager = NULL;
	}
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}

// ==========================================================
// ウィンドウプロシージャ関数
// ==========================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// ウィンドウ破棄メッセージ
	case WM_DESTROY:
		// WM_qUITメッセージを返す
		PostQuitMessage(0);
		break;
		// ✖を押されたとき
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]キーが押された
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	default:
		break;
	}
	// 既定の処理を提供
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ==========================================================
// FPSの取得処理
// ==========================================================
int GetFPS(void)
{
	return g_nFpsCnt;
}
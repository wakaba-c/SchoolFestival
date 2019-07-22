//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "controller.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// プロトタイプ宣言
//=============================================================================
#ifdef _DEBUG
void DrawCameraData(void);
#endif

//=============================================================================
// グローバル変数
//=============================================================================
CAMERA	g_camera;			//カメラ情報

D3DXVECTOR3	g_originPos;

D3DXVECTOR3 g_resultPos;
#ifdef _DEBUG
LPD3DXFONT			g_pCameraFont = NULL;			// フォントへのポインタ
#endif

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3		originPos;

#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &g_pCameraFont);
#endif

	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posV = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	originPos = g_camera.posR - g_camera.posV;

	g_originPos = D3DXVECTOR3(710.0f, 80.0f, -250.0f);
	g_resultPos = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	g_camera.fDistonce = sqrtf(originPos.y * originPos.y + originPos.z * originPos.z + 50);
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
	
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *pPlayer;

	float	fValueH = 0;
	float	fValueV = 0;

	pPlayer = GetPlayer();
	D3DXVECTOR3 Calculation;			//計算用

	g_camera.rotDest.y = pPlayer->rot.y + D3DX_PI;

	Calculation.y = g_camera.rotDest.y - g_camera.rot.y;

	if (GetMode() == MODE_TITLE)
	{
		g_camera.rotDest.y = 0.0f + D3DX_PI;

		g_camera.posR.x = g_originPos.x + sinf(D3DX_PI + 1.25f);
		g_camera.posR.y = g_originPos.y;
		g_camera.posR.z = g_originPos.z + cosf(D3DX_PI + 1.25f);
		g_camera.posV.x = g_originPos.x + sinf(D3DX_PI + 1.25f) * g_camera.fDistonce;
		g_camera.posV.y = g_originPos.y + 40.0f;
		g_camera.posV.z = g_originPos.z + cosf(D3DX_PI + 1.25f) * g_camera.fDistonce;

		g_originPos.z += 0.1f;
	}

	//ゲームモードだった場合
	else if (GetMode() == MODE_GAME)
	{
		g_camera.nCount++;

		if (Calculation.y > D3DX_PI)
		{
			g_camera.rotDest.y -= D3DX_PI * 2;
		}
		else if (Calculation.y < -D3DX_PI)
		{
			g_camera.rotDest.y += D3DX_PI * 2;
		}

		if (fabsf(Calculation.y) < 0.0f)
		{
			Calculation.y = g_camera.rotDest.y;
		}

		//if (g_camera.nCount > 60 && GetKeyboardPress(DIK_L) != 1)
		//{
		//	g_camera.rot.y += (g_camera.rotDest.y - g_camera.rot.y) * 0.02f;
		//}

		//g_camera.rot.y += (g_camera.rotDest.y - g_camera.rot.y) * 0.02f;

		g_camera.posRDest.x = pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) + pPlayer->move.x;
		g_camera.posRDest.y = pPlayer->move.y + 80;
		g_camera.posRDest.z = pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) + pPlayer->move.z;

		g_camera.posVDest.x = pPlayer->pos.x + sinf(D3DX_PI + g_camera.rot.y) * g_camera.fDistonce + pPlayer->move.x;
		g_camera.posVDest.y = 240;
		g_camera.posVDest.z = pPlayer->pos.z + cosf(D3DX_PI + g_camera.rot.y) * g_camera.fDistonce + pPlayer->move.z;

		g_camera.posV += (g_camera.posVDest - g_camera.posV) * 0.2f;

		g_camera.posR += (g_camera.posRDest - g_camera.posR) * 0.2f;
	}

	//モードリザルトだった場合
	else if (GetMode() == MODE_RESULT)
	{
		g_camera.rotDest.y = 0.0f + D3DX_PI;

		g_camera.posR.x = 425.0f + sinf(D3DX_PI + 0.5f);
		g_camera.posR.y = 70;
		g_camera.posR.z = -250.0f + cosf(D3DX_PI + 0.5f);
		g_camera.posV.x = 425.0f + sinf(D3DX_PI + 0.5f) * 50;
		g_camera.posV.y = 70 + 30.0f;
		g_camera.posV.z = -250.0f + cosf(D3DX_PI + 0.5f) * 50;
	}
	
	//モードランキングだった場合
	else if (GetMode() == MODE_RANKING)
	{
		g_camera.rotDest.y = 0.0f + D3DX_PI;

		g_camera.posR.x = g_resultPos.x + sinf(D3DX_PI + 3.14f);
		g_camera.posR.y = g_resultPos.y;
		g_camera.posR.z = g_resultPos.z + cosf(D3DX_PI + 3.14f);
		g_camera.posV.x = g_resultPos.x + sinf(D3DX_PI + 3.14f) * g_camera.fDistonce;
		g_camera.posV.y = g_resultPos.y + 70.0f;
		g_camera.posV.z = g_resultPos.z + cosf(D3DX_PI + 3.14f) * g_camera.fDistonce;

		g_resultPos.z += -0.5f;
	}

	//g_camera.rot -= D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * (1.5f * g_player.nValueV), 0, cosf(D3DX_PI * 1.0f) * (1.5f * g_player.nValueV));

	GetJoypadStickRight(0, &fValueH, &fValueV);

	g_camera.rot.y +=  fValueH * 0.1f;

	//回転
	if (GetKeyboardPress(DIK_E))
	{
		g_camera.rot.y += 0.01f;
	}
	if (GetKeyboardPress(DIK_Q))
	{
		g_camera.rot.y -= 0.01f;
	}

	//回転数のリセット
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y -= D3DX_PI * 2;
	}

	if (g_camera.posV.x < -490)
	{
		g_camera.posV.x = -490;
	}
	else if (g_camera.posV.x > 490)
	{
		g_camera.posV.x = 490;
	}

	if (g_camera.posV.z < -490)
	{
		g_camera.posV.z = -490;
	}
	else if (g_camera.posV.z > 490)
	{
		g_camera.posV.z = 490;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&g_camera.mtxProjection,
		D3DXToRadian(45.0f),								//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			//アスペクト比
		10.0f,												//NearZ値
		10000.0f);											//FarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(
		&g_camera.mtxView,
		&g_camera.posV,			//カメラの視点
		&g_camera.posR,			//カメラの注視点
		&g_camera.vecU);		//カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
#ifdef _DEBUG
	DrawCameraData();
#endif // _DEBUG
}

#ifdef _DEBUG
//=============================================================================
// カメラの情報
//=============================================================================
void DrawCameraData(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	RECT rect = { 800, 300, SCREEN_WIDTH, SCREEN_HEIGHT };

	char sCamera[500];
	char sPosV[128];
	char sPosR[128];
	char sRot[128];

	// テキスト描画(pos)
	sprintf(&sPosV[0], "g_camera.posV X : %.2f Y : %.2f Z : %.2f\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	sprintf(&sPosR[0], "g_camera.posR X : %.2f Y : %.2f Z : %.2f\n", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
	sprintf(&sRot[0], "g_camera.rot X : %.2f Y : %.2f Z : %.2f\n", g_camera.rot.x, g_camera.rot.y, g_camera.rot.z);

	strcpy(&sCamera[0], "==============カメラ===============\n");

	strcat(&sCamera[0], &sPosV[0]);

	strcat(&sCamera[0], &sPosR[0]);

	strcat(&sCamera[0], &sRot[0]);

	// テキスト描画
	g_pCameraFont->DrawText(NULL, &sCamera[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif // _DEBUG

//=============================================================================
// カメラの情報
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_camera;
}
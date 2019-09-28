// ==========================================================
//
// 数字[number.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTex = NULL;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CNumber::CNumber()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CNumber::~CNumber()
{

}

// ==========================================================
// 数字初期化
// ==========================================================
void CNumber::Init(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 頂点座標
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (NUMBERSIZE_X / 2), m_pos.y - (NUMBERSIZE_Y / 2), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (NUMBERSIZE_X / 2), m_pos.y - (NUMBERSIZE_Y / 2), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (NUMBERSIZE_X / 2), m_pos.y + (NUMBERSIZE_Y / 2), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (NUMBERSIZE_X / 2), m_pos.y + (NUMBERSIZE_Y / 2), 0);
	// 1.0で固定、同次座標
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;
	// 頂点カラー
	m_pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 数字終了
// ==========================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ==========================================================
// 数字更新
// ==========================================================
void CNumber::Update(void)
{

}

// ==========================================================
// 数字描画処理
// ==========================================================
void CNumber::Draw(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, m_pTex);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ==========================================================
// 数字生成
// ==========================================================
CNumber *CNumber::Create(void)
{
	// シーン動的に確保
	CNumber *pNumber = new CNumber;

	// シーン初期化
	pNumber->Init();

	// 値を返す
	return pNumber;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CNumber::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE, &m_pTex);
	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CNumber::Unload(void)
{
	// テクスチャ解放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// 数字設定
// ==========================================================
void CNumber::SetNumber(int nNumber)
{
	int nNum = nNumber;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(nNum*0.1f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(nNum *0.1f + 0.1f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(nNum* 0.1f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(nNum * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 数字配置
// ==========================================================
void CNumber::SetPosNum(D3DXVECTOR3 pos, int nSizeX, int nSizeY)
{
	m_pos = pos;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 頂点座標
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (nSizeX / 2), m_pos.y - (nSizeY / 2), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (nSizeX / 2), m_pos.y - (nSizeY / 2), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (nSizeX / 2), m_pos.y + (nSizeY / 2), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (nSizeX / 2), m_pos.y + (nSizeY / 2), 0);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}
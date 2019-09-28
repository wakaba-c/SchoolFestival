// ==========================================================
//
// ポリゴン[polygon.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "polygon.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTex = NULL;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CPolygon::CPolygon()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0, 0, 0, 0);
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CPolygon::~CPolygon()
{

}

// ==========================================================
// 数字初期化
// ==========================================================
void CPolygon::Init(void)
{
	m_col = D3DXCOLOR(0, 0, 0, 0);

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
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y , 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y , 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_nSizeY, 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_nSizeY, 0);
	// 1.0で固定、同次座標
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;
	// 頂点カラー
	m_pVtx[0].col = m_col;
	m_pVtx[1].col = m_col;
	m_pVtx[2].col = m_col;
	m_pVtx[3].col = m_col;
	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 数字終了
// ==========================================================
void CPolygon::Uninit(void)
{
	// テクスチャの開放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

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
void CPolygon::Update(void)
{

}

// ==========================================================
// 数字描画処理
// ==========================================================
void CPolygon::Draw(void)
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
CPolygon *CPolygon::Create(void)
{
	// シーン動的に確保
	CPolygon *pPolygon = new CPolygon;

	// シーン初期化
	pPolygon->Init();

	// 値を返す
	return pPolygon;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CPolygon::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_POLYGON, &m_pTex);
	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CPolygon::Unload(void)
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
void CPolygon::SetPolygon(D3DXVECTOR3 pos, float fWidth, int nSizeY)
{
	m_pos = pos;
	m_fWidth = fWidth;
	m_nSizeY = nSizeY;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 頂点座標
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y , 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x+m_fWidth, m_pos.y , 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y  + m_nSizeY, 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y  + m_nSizeY, 0);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 体力設定
// ==========================================================
void CPolygon::SetLife(float fWidth)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 頂点座標
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y , 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_nSizeY, 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + m_nSizeY, 0);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 生成
// ==========================================================
void CPolygon::SetColPolygon(D3DXCOLOR col)
{
	m_col = col;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 頂点カラー
	m_pVtx[0].col = m_col;
	m_pVtx[1].col = m_col;
	m_pVtx[2].col = m_col;
	m_pVtx[3].col = m_col;

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}
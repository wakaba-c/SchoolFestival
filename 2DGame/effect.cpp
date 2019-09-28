// ==========================================================
//
// エフェクト[effect.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CEffect *CEffect::m_pEffect;
LPDIRECT3DTEXTURE9 CEffect::m_pTex = NULL;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CEffect::CEffect(PRIORITY type = CScene::PRIORITY_EFFECT):CScene2D(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CEffect::~CEffect()
{

}

// ==========================================================
// エフェクトの初期化
// ==========================================================
void CEffect::Init(void)
{
	CScene2D::Init();
	SetAnimation(ATTACKENEMYTEX_X, ATTACKENEMYTEX_Y, 0.0f, 0);
}

// ==========================================================
// エフェクトの終了
// ==========================================================
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// エフェクトの更新
// ==========================================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// 爆発のパターン切り替え
	m_nCounterAnim++;

	if (m_nCounterAnim % EFFECTANIM_INTERVAL == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % EFFECTANIM_INTERVAL;

		SetAnimation(ATTACKENEMYTEX_X, ATTACKENEMYTEX_Y, 0.0f, m_nPatternAnim);

		SetPos(pos, EFFECTSIZE_X, EFFECTSIZE_Y, CENTERPOS_NORMAL);
	}

	CScene2D::Update();
}

// ==========================================================
// エフェクトの描画
// ==========================================================
void CEffect::Draw(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ==========================================================
// エフェクトの生成
// ==========================================================
CEffect *CEffect::Create(void)
{
	// シーン動的に確保
	m_pEffect = new CEffect(CScene::PRIORITY_EFFECT);

	// シーン初期化
	m_pEffect->Init();

	m_pEffect->BindTex(m_pTex);

	// 値を返す
	return m_pEffect;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CEffect::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &m_pTex);
	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CEffect::Unload(void)
{
	// テクスチャ解放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// ポジション移動関数
// ==========================================================
void CEffect::MovePos(D3DXVECTOR3 pos, float fMove)
{
	m_pEffect->SetPos(D3DXVECTOR3(pos.x - fMove, pos.y, pos.z), EFFECTSIZE_X, EFFECTSIZE_Y, CENTERPOS_NORMAL);
}
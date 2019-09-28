// ==========================================================
//
// 爆発[explsion.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "explosion.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CExplosion *CExplosion::m_pExplosion;
LPDIRECT3DTEXTURE9 CExplosion::m_pTex[EXPLOSIONTYPE_MAX] = {};

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CExplosion::CExplosion(PRIORITY type = CScene::PRIORITY_WATERSPOUT):CScene2D(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CExplosion::~CExplosion()
{

}

// ==========================================================
// 爆発の初期化
// ==========================================================
void CExplosion::Init(void)
{
	CScene2D::Init();
	SetAnimation(EXPLOSIONTEX_X, EXPLOSIONTEX_Y, 0.0f, 0);
}

// ==========================================================
// 爆発の終了
// ==========================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// 爆発の更新
// ==========================================================
void CExplosion::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// 爆発のパターン切り替え
	m_nCounterAnim++;

	if (m_nCounterAnim % EXPLOSION_ANIM_INTERVAL == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % EXPLOSION_ANIM_INTERVAL;

		SetAnimation(EXPLOSIONTEX_X, EXPLOSIONTEX_Y, 0.0f, m_nPatternAnim);

		SetPos(pos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);

	/*	if (m_nPatternAnim % EXPLOSION_ANIM_COUNT == 0)
		{
			Release();
		}*/
	}


	CPlayer *pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 playerPos = pPlayer->GetPosition();

	SetPos(playerPos + D3DXVECTOR3(-50.0f, 0.0f, 0.0f), COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);

	CScene2D::Update();
}

// ==========================================================
// 爆発の描画
// ==========================================================
void CExplosion::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// 爆発の生成
// ==========================================================
CExplosion *CExplosion::Create(EXPLOSIONTYPE type)
{
	// シーン動的に確保
	m_pExplosion = new CExplosion(CScene::PRIORITY_WATERSPOUT);

	// シーン初期化
	m_pExplosion->Init();

	m_pExplosion->BindTex(m_pTex[type]);

	// 値を返す
	return m_pExplosion;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CExplosion::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_EFFECT, &m_pTex[EXPLOSIONTYPE_PLAYER_EFFECT]);
	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CExplosion::Unload(void)
{
	for (int nCnt = 0; nCnt < EXPLOSIONTYPE_MAX; nCnt++)
	{
		// テクスチャ解放
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}
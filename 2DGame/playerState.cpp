// ==========================================================
//
// ステート[playerState.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "playerState.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CPlayerState *CPlayerState::m_pPlayerState;
LPDIRECT3DTEXTURE9 CPlayerState::m_pTex[STATETYPE_MAX] = {};
CPlayerState::STATETYPE CPlayerState::m_state = STATETYPE_PLAYER;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CPlayerState::CPlayerState(PRIORITY type = CScene::PRIORITY_EXPLOSION) :CScene2D(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CPlayerState::~CPlayerState()
{

}

// ==========================================================
// 爆発の初期化
// ==========================================================
void CPlayerState::Init(void)
{
	CScene2D::Init();
	
	if (m_state == STATETYPE_UP)
	{
		SetAnimation(STATE_X, STATE_Y, 0.0f, 0);
	}
	else if (m_state == STATETYPE_BARRIER)
	{
		SetAnimation(0.2f, STATE_Y, 0.0f, 0);
	}
}

// ==========================================================
// 爆発の終了
// ==========================================================
void CPlayerState::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// 爆発の更新
// ==========================================================
void CPlayerState::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// 爆発のパターン切り替え
	m_nCounterAnim++;

	if (m_state == STATETYPE_UP)
	{
		if (m_nCounterAnim % EXPLOSION_ANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % EXPLOSION_ANIM_INTERVAL;

			SetAnimation(STATE_X, STATE_Y, 0.0f, m_nPatternAnim);

			SetPos(pos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);

			/*	if (m_nPatternAnim % EXPLOSION_ANIM_COUNT == 0)
			{
			Release();
			}*/
		}

	}
	else if (m_state == STATETYPE_BARRIER)
	{
		if (m_nCounterAnim % EXPLOSION_ANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % EXPLOSION_ANIM_INTERVAL;

			SetAnimation(0.2f, STATE_Y, 0.0f, m_nPatternAnim);

			/*	if (m_nPatternAnim % EXPLOSION_ANIM_COUNT == 0)
			{
			Release();
			}*/
		}

		CPlayer *pPlayer = CGame::GetPlayer();

		D3DXVECTOR3 playerPos = pPlayer->GetPosition();

		SetPos(playerPos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);
	}

	CScene2D::Update();
}

// ==========================================================
// 爆発の描画
// ==========================================================
void CPlayerState::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// 爆発の生成
// ==========================================================
CPlayerState *CPlayerState::Create(STATETYPE type)
{
	m_state = type;
	// シーン動的に確保
	m_pPlayerState = new CPlayerState(CScene::PRIORITY_EXPLOSION);

	// シーン初期化
	m_pPlayerState->Init();

	m_pPlayerState->BindTex(m_pTex[type]);

	// 値を返す
	return m_pPlayerState;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CPlayerState::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_UP, &m_pTex[STATETYPE_UP]);
	D3DXCreateTextureFromFile(pDevice, PLAYER_STATE, &m_pTex[STATETYPE_BARRIER]);
	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CPlayerState::Unload(void)
{
	for (int nCnt = 0; nCnt < STATETYPE_MAX; nCnt++)
	{
		// テクスチャ解放
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}
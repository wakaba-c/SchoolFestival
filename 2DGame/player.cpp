// ==========================================================
//
// プレイヤー[player.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "player.h"
#include "manager.h"
#include "inputController.h"
#include "inputKeyboard.h"
#include "renderer.h"
#include "explosion.h"
#include "life.h"
#include "bg.h"
#include "stock.h"
#include "sound.h"
#include "block.h"
#include "scene.h"
#include "playerState.h"
#include "item.h"
#include "game.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
#define PLAYER_GRAVITY 2.5f	//重力
#define PLAYER_VELOSCITY 70.0f	//ジャンプの高さ

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
int CPlayer::m_nCntWaterspout;
LPDIRECT3DTEXTURE9 CPlayer::m_pTex = NULL;
int CPlayer::m_nLife = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CPlayer::CPlayer(PRIORITY type = CScene::PRIORITY_PLAYER):CScene2D(type)
{
	m_state = PLAYERSTATE_NORMAL;
	m_nCount = 0;
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CPlayer::~CPlayer()
{

}

// ==========================================================
// プレイヤー初期化
// ==========================================================
void CPlayer::Init(void)
{
	// プレイヤー体力初期化
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = PLAYER_LIFE;
	m_fVelocity = PLAYER_VELOSCITY;
	m_bJump = false;
	m_bDoubleJump = false;
	m_nGravity = PLAYER_GRAVITY;
	m_state = PLAYERSTATE_NORMAL;
	m_nCount = 0;

	// スキルカウント初期化
	m_nCntWaterspout = 0;

	CScene2D::Init();

	// アニメーション初期化
	SetAnimation(PLAYERTEX_X, PLAYERTEX_Y, 0.0f, 0);

	SetObjType(OBJTYPE_PLAYER);

	CExplosion *pExplosion = CExplosion::Create(CExplosion::EXPLOSIONTYPE_PLAYER_EFFECT);

	CPlayerState *pPlayerState = CPlayerState::Create(CPlayerState::STATETYPE_BARRIER);

}

// ==========================================================
// プレイヤー終了
// ==========================================================
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// プレイヤー更新
// ==========================================================
void CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();
	CStock *pStock = CStock::GetStock();
	CBlock *pBlock = CBlock::GetBlock();

	// パターン切り替え
	m_nCounterAnim++;
	//m_nPatternAnim++;//カウンター加算

	m_pos = GetPos();

	//現在の位置を前回の位置にする
	m_posOld = m_pos;

	// キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// キーボードCを押すと
	if (pInputKeyboard->GetKeyboardTngger(DIK_N))
	{
		// ストックを減らす
		pStock->AddStock(PLAYER_STOCK);

		// カウントが2より小さい場合
		if (m_nCntWaterspout <= 2)
		{
			// カウントを増やす
			m_nCntWaterspout++;
		}
	}

	if (pInputKeyboard->GetKeyboardPress(DIK_A))
	{// Aボタンを押されたとき
	 // 左
		if (m_state == PLAYERSTATE_NORMAL)
		{
			m_move.x += sinf(-D3DX_PI*0.2f)*4.0f;
			m_move.y += cosf(D3DX_PI*0.2f)*4.0f;
			m_nDirectionMove = 1;
		}

		if (m_state == PLAYERSTATE_DASH)
		{
			m_move.x += sinf(-D3DX_PI*0.2f)*6.0f;
			m_move.y += cosf(D3DX_PI*0.2f)*6.0f;
			m_nDirectionMove = 1;
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_D))
	{// Dボタンを押されたとき
		if (m_state == PLAYERSTATE_NORMAL)
		{
			m_move.x += sinf(D3DX_PI*0.5f)*4.0f;
			m_move.y += cosf(D3DX_PI*0.5f)*4.0f;
			m_nDirectionMove = 1;
		}

		if (m_state == PLAYERSTATE_DASH)
		{
			m_move.x += sinf(D3DX_PI*0.5f)*6.0f;
			m_move.y += cosf(D3DX_PI*0.5f)*6.0f;
			m_nDirectionMove = 1;
		}
	}

	if (m_nDirectionMove == 1)
	{// nDirectionMoveが0のとき
	 // パターン更新
		if (m_nCounterAnim % ANIM_INTERVAL == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % PLAYER_ANIM;

			SetAnimation(PLAYERTEX_X, 1.0, PLAYERTEX_Y, m_nPatternAnim);
		}
	}

	if (pInputKeyboard->GetKeyboardTngger(DIK_W))
	{
		if (m_bJump == false)
		{//一段ジャンプ
			m_bJump = true;
			m_move.y -= m_fVelocity;
		}

		if (m_bJump == true && m_bDoubleJump == false)
		{//二段ジャンプ
			m_bJump = false;
			m_bDoubleJump = true;
			m_move.y = 0;
			m_move.y -= m_fVelocity;
		}
	}

	//アニメーション
	//if (m_nDirectionMove == 0)
	//{
	//	if (m_nCounterAnim % ANIM_INTERVAL == 0)
	//	{
	//		m_nPatternAnim = (m_nPatternAnim + 1) % PLAYER_ANIM;//パターン更新

	//		CScene2D::SetAnimation((float)m_nDirectionMove, 0.25f, 0.5f, m_nPatternAnim);
	//	}
	//}
	//if (m_nDirectionMove == 1)
	//{
	//	if (m_nCounterAnim % ANIM_INTERVAL == 0)
	//	{
	//		m_nPatternAnim = (m_nPatternAnim + 1) % PLAYER_ANIM;//パターン更新

	//		CScene2D::SetAnimation((float)m_nDirectionMove, 0.25f, 0.5f, m_nPatternAnim);
	//	}
	//}
	/////////////////////////

	if (m_state == PLAYERSTATE_DASH)
	{
		m_nCount++;
		if (m_nCount >= 180)
		{
			m_state = PLAYERSTATE_NORMAL;
		}
	}

	//重力 毎フレーム0.5ずつムーブを減算
	m_move.y += m_nGravity;

	// 移動量
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	// 慣性
	m_move.x += (0 - m_move.x) * 0.2f;
	m_move.y += (0 - m_move.y) * 0.2f;

	// 体力取得
	CLife *pLife = CLife::GetLife();
	//体力減少
	m_nLife--;
	// ダメージ量を反映
	pLife->AddLife(m_nLife);

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
		{
			CScene *pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE type;
				type = pScene->GetObjType();

				if (type == OBJTYPE_BLOCK)
				{
					CBlock *pBlock;
					pBlock = (CBlock*)GetScene(nCntPriority, nCnt);

					if (HitBlock(&pBlock->GetPos(), &m_pos, &pBlock->GetPosOld(), &m_posOld, BLOCKSIZE_X / 2, BLOCKSIZE_Y / 2, PLAYERSIZE_X / 2, PLAYERSIZE_Y / 2) == true)
					{
						if (m_bJump == true)
						{
							m_bJump = false;
						}

						if (pInputKeyboard->GetKeyboardTngger(DIK_W))
						{
							if (m_bJump == false)
							{
								m_bJump = true;

								//ジャンプの距離
								m_move.y = PLAYER_VELOSCITY;
							}
						}
					}
				}
			}
		}
	}

	// 反射処理
	if (m_pos.y + PLAYERSIZE_Y / 2 > SCREEN_HEIGHT)
	{
		m_pos.y = SCREEN_HEIGHT - PLAYERSIZE_Y / 2;
		m_bJump = false;
		m_bDoubleJump = false;
	}
	if (m_pos.y - PLAYERSIZE_Y / 2 < ACTION_LIMIT)
	{
		m_pos.y = ACTION_LIMIT + PLAYERSIZE_Y / 2;
	}
	if (m_pos.x + PLAYERSIZE_X / 2 > SCREEN_WIDTH)
	{
		m_pos.x = SCREEN_WIDTH - PLAYERSIZE_X / 2;
	}
	if (m_pos.x - PLAYERSIZE_X / 2 < 0)
	{
		m_pos.x = 0 + PLAYERSIZE_X / 2;
	}

	// 位置情報更新
	SetPos(m_pos, PLAYERSIZE_X, PLAYERSIZE_Y, CENTERPOS_NORMAL);

	CScene2D::Update();
}

// ==========================================================
// プレイヤー描画
// ==========================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// ポリゴン生成
// ==========================================================
CPlayer *CPlayer::Create(void)
{
	// シーン動的に確保
	CPlayer *pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	if (pPlayer != NULL)
	{
		// シーン初期化
		pPlayer->Init();
		pPlayer->BindTex(m_pTex);
	}
	// 値を返す
	return pPlayer;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CPlayer::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTex);

	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CPlayer::Unload(void)
{
	// テクスチャ解放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// プレイヤーの位置取得
// ==========================================================
D3DXVECTOR3 CPlayer::GetPosition(void)
{
	return m_pos;
}

// ==========================================================
// プレイヤーの移動量取得
// ==========================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

// ==========================================================
// プレイヤーの移動量
// ==========================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

// ==========================================================
// プレイヤーのダメージ処理
// ==========================================================
void CPlayer::AddDamage(int nDamage)
{
	CSound *pSound = CManager::GetSound();

	CExplosion *pExplosion;
	m_nLife -= nDamage;

	// 体力取得
	CLife *pLife = CLife::GetLife();

	// ダメージ量を反映
	pLife->AddLife(m_nLife);

	// 体力が０になったら
	if (m_nLife <= 0)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_MEDIC);

		// 爆発生成
		pExplosion = CExplosion::Create(CExplosion::EXPLOSIONTYPE_PLAYER_EFFECT);
		pExplosion->SetPos(m_pos, COORDINATE_X, COORDINATE_Y, CENTERPOS_NORMAL);
		Release();
	}
}

void CPlayer::PLAYERDASH(void)
{
	m_state = PLAYERSTATE_DASH;
}

//ライフ追加
void CPlayer::AddLife(int nLife)
{
	m_nLife += nLife;
}

// ==========================================================
// 体力取得
// ==========================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

// ==========================================================
// 体力取得
// ==========================================================
int CPlayer::GetDirectionMove(void)
{
	return m_nDirectionMove;
}

// ==========================================================
// スキル個数取得
// ==========================================================
int CPlayer::GetCntWaterspout(void)
{
	return m_nCntWaterspout;
}

// ==========================================================
// スキル個数設定
// ==========================================================
void CPlayer::SetCntWaterspout(int nCntWaterspout)
{
	m_nCntWaterspout = nCntWaterspout;
}
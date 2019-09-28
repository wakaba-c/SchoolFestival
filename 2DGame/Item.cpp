// ==========================================================
//
// アイテム[item.cpp]
// Author:Seiya Takahashi
//
// ==========================================================
#include "item.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "effect.h"
#include "player.h"
#include "game.h"
#include "inputKeyboard.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "game.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CItem *CItem::m_pItem;
LPDIRECT3DTEXTURE9 CItem::m_pTex[MAX_ITEMTEX] = {};
int CItem::m_nNumItem = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CItem::CItem(PRIORITY type = CScene::PRIORITY_ITEM) :CScene2D(type)
{
	//m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CItem::~CItem()
{

}

// ==========================================================
// 弾の初期化
// ==========================================================
void CItem::Init(void)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CScene2D::Init();
	SetObjType(OBJTYPE_ITEM);
	m_type = ITEMTYPE_NORMAL;
	m_Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MagneticForce = 0.0f;
	SetType(ITEMTYPE_NORMAL);
}

// ==========================================================
// 弾の終了
// ==========================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// 弾の更新
// ==========================================================
void CItem::Update(void)
{
	// プレイヤー情報取得
	CPlayer *pPlayer = CGame::GetPlayer();
	CScore *pScore = CScore::GetScore();
	D3DXVECTOR3 playerPos = pPlayer->GetPosition();

	// ポジション取得
	m_pos = GetPos();

	m_pos.x -= 5.0f;

	// 位置情報取得
	SetPos(m_pos, ITEMSIZE_X, ITEMSIZE_Y, CENTERPOS_NORMAL);

	SetCol(m_col);


	if (m_type == ITEMTYPE_COIN)
	{//スコア加算
		//pScore->addscore();
	}

	if (m_type == ITEMTYPE_WATER)
	{//体力回復
		//pPlayer->AddLife(50);
	}

	if (m_type == ITEMTYPE_MAGNET)
	{//磁石
		//数秒間コインを引き寄せる
		//プレイヤーの一定範囲内にコインが入ったら

		//CPlayer *pPlayer = CGame::GetPlayer();

		//D3DXVECTOR3 Distance = pPlayer->GetPosition() - GetPosition();

		//float fDistance = Distance.x * Distance.x + Distance.y * Distance.y;

		//float fDistance2 = (float)sqrt(fDistance);

		//if (fDistance2 < 200)
		//{
		//	D3DXVECTOR3 Vector = pPlayer->GetPosition() - GetPosition();

		//	D3DXVec3Normalize(&m_Direction, &Vector);
		//}

		////コイン一つ一つの速度
		//m_MagneticForce += 2;

		//m_pos += m_Direction * m_MagneticForce;
	}

	if (m_type == ITEMTYPE_DASH)
	{//疾走
		//プレイヤーの速度数秒間アップ
		//プレイヤーの加速する関数

		pPlayer->PLAYERDASH();
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_POLGON; nCnt++)
		{
			CScene *pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE type;
				type = pScene->GetObjType();

				if (type == OBJTYPE_ITEM)
				{
					CPlayer *pPlayer;
					pPlayer = (CPlayer*)GetScene(nCntPriority, nCnt);

					if (HitBlock(&m_pos, &pPlayer->GetPosOld(), &pPlayer->GetPosOld(), &m_posOld, ITEMSIZE_X / 2, ITEMSIZE_Y / 2, PLAYERSIZE_X / 2, PLAYERSIZE_Y / 2) == true)
					{
						D3DXVECTOR3 Distance = pPlayer->GetPosition() - GetPosition();

						float fDistance = Distance.x * Distance.x + Distance.y * Distance.y;

						float fDistance2 = (float)sqrt(fDistance);

						switch (m_type)
						{
						case CItem::ITEMTYPE_NORMAL:
							break;
						case CItem::ITEMTYPE_WATER:
							pPlayer->AddLife(50);
							break;
						case CItem::ITEMTYPE_COIN:
							pScore->AddScore(500);
							break;
						case CItem::ITEMTYPE_MAGNET:
							//磁石

							if (fDistance2 < 200)
							{
								D3DXVECTOR3 Vector = pPlayer->GetPosition() - GetPosition();

								D3DXVec3Normalize(&m_Direction, &Vector);
							}

							//コイン一つ一つの速度
							m_MagneticForce += 2;

							m_pos += m_Direction * m_MagneticForce;

							break;
						case CItem::ITEMTYPE_DASH:
							//プレイヤーの速度数秒間アップ
							//プレイヤーの加速する関数


							pPlayer->PLAYERDASH();

							break;
						case CItem::ITEMTYPE_MAX:
							break;
						}
					}
				}
			}
		}
	}

	CScene2D::Update();

}

// ==========================================================
// 弾の描画
// ==========================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// 弾の生成
// ==========================================================
CItem *CItem::Create(ITEMTYPE nType)
{
	// シーン動的に確保
	m_pItem = new CItem(CScene::PRIORITY_ITEM);

	m_pItem->m_type = nType;

	if (m_pItem->m_type == ITEMTYPE_WATER)
	{
		m_pItem->m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	}

	if (m_pItem->m_type == ITEMTYPE_COIN)
	{
		m_pItem->m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}

	// シーン初期化
	m_pItem->Init();

	m_pItem->BindTex(m_pTex[nType]);

	// 値を返す
	return m_pItem;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CItem::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_DRINK, &m_pTex[ITEMTYPE_WATER]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_COIN, &m_pTex[ITEMTYPE_COIN]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_MAGNET, &m_pTex[ITEMTYPE_MAGNET]);

	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CItem::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_ITEMTEX; nCnt++)
	{
		// テクスチャ解放
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

CItem::ITEMTYPE CItem::GetItemType(void)
{
	return m_type;
}

D3DXVECTOR3 CItem::GetPosition(void)
{
	return m_pos;
}

// ==========================================================
// 敵のタイプ設定
// ==========================================================
void CItem::SetType(ITEMTYPE type)
{
	m_type = type;
}

// ==========================================================
// 弾の向き
// ==========================================================
void CItem::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

// ==========================================================
// 弾の向き
// ==========================================================
void CItem::SetItemNum(int nNumItem)
{
	m_nNumItem = nNumItem;
}

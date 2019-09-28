// ==========================================================
//
// ブロック[block.cpp]
// Author:Seiya Takahashi
//
// ==========================================================
#include "block.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "effect.h"
#include "player.h"
#include "game.h"
#include "inputKeyboard.h"
#include "sound.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CBlock *CBlock::m_pBlock;
LPDIRECT3DTEXTURE9 CBlock::m_pTex = NULL;
int CBlock::m_nNumBlock = 0;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CBlock::CBlock(PRIORITY type = CScene::PRIORITY_BLOCK) :CScene2D(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CBlock::~CBlock()
{

}

// ==========================================================
// 弾の初期化
// ==========================================================
void CBlock::Init(void)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CScene2D::Init();
	SetObjType(OBJTYPE_BLOCK);
}

// ==========================================================
// 弾の終了
// ==========================================================
void CBlock::Uninit(void)
{
	CScene2D::Uninit();
}

// ==========================================================
// 弾の更新
// ==========================================================
void CBlock::Update(void)
{
	// プレイヤー情報取得
	CPlayer *pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPosition();

	// ポジション取得
	m_pos = GetPos();

	m_posOld = m_pos;

	m_pos.x -= 5.0f;

	// 位置情報取得
	SetPos(m_pos, BLOCKSIZE_X, BLOCKSIZE_Y, CENTERPOS_NORMAL);

	CScene2D::Update();
}

// ==========================================================
// 弾の描画
// ==========================================================
void CBlock::Draw(void)
{
	CScene2D::Draw();
}

// ==========================================================
// ブロックの情報取得
// ==========================================================
CBlock * CBlock::GetBlock(void)
{
	return m_pBlock;
}

// ==========================================================
// 弾の生成
// ==========================================================
CBlock *CBlock::Create(void)
{
	// シーン動的に確保
	m_pBlock = new CBlock(CScene::PRIORITY_BLOCK);

	// シーン初期化
	m_pBlock->Init();

	m_pBlock->BindTex(m_pTex);

	// 値を返す
	return m_pBlock;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CBlock::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE, &m_pTex);

	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CBlock::Unload(void)
{
	// テクスチャ解放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// 弾の向き
// ==========================================================
void CBlock::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

// ==========================================================
// 弾の向き
// ==========================================================
void CBlock::SetBlockNum(int nNumBlock)
{
	m_nNumBlock = nNumBlock;
}

// ==========================================================
// ブロックの個数を取得
// ==========================================================
int CBlock::GetBlockNum(void)
{
	return m_nNumBlock;
}

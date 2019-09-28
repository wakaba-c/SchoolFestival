// ==========================================================
//
// 描画2D[renderer.cpp]
// Author:Ryouma Inoue
//
// ==========================================================
#include "player.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include"inputController.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
CScene2D *CScene2D::m_pScene2D;

// ==========================================================
// グローバル変数
// ==========================================================

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CScene2D::CScene2D(PRIORITY type) : CScene(type)
{

}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CScene2D::~CScene2D()
{

}

// ==========================================================
// ポリゴン初期化
// ==========================================================
void CScene2D::Init(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ポリゴン初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = atan2f(POLGON_X, POLGON_Y);
	m_fLength = sqrtf(POLGON_X * POLGON_X + POLGON_Y * POLGON_Y);
	m_bLand = false;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);
	
	// 座標の設定
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (POLGON_X / 2), m_pos.y - (POLGON_Y / 2), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (POLGON_X / 2), m_pos.y - (POLGON_Y / 2), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (POLGON_X / 2), m_pos.y + (POLGON_Y / 2), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (POLGON_X / 2), m_pos.y + (POLGON_Y / 2), 0);
	// 1.0で固定、同次座標
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;
	// 頂点カラー
	m_pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// ポリゴン終了
// ==========================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ==========================================================
// ポリゴン更新
// ==========================================================
void CScene2D::Update(void)
{

}

// ==========================================================
// ポリゴンの描画処理
// ==========================================================
void CScene2D::Draw(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, m_pTexturePolygon);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ==========================================================
// テクスチャ生成
// ==========================================================
void CScene2D::BindTex(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexturePolygon = tex;
}

// ==========================================================
// ポリゴンの配置
// ==========================================================
void CScene2D::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pos = pos;
	// 移動座標の設定
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (fSizeX / nCenterPos), m_pos.y - (fSizeY / nCenterPos), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (fSizeX / nCenterPos), m_pos.y - (fSizeY / nCenterPos), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (fSizeX / nCenterPos), m_pos.y + (fSizeY / nCenterPos), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (fSizeX / nCenterPos), m_pos.y + (fSizeY / nCenterPos), 0);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

// ==========================================================
// アニメーション生成
// ==========================================================
void CScene2D::SetAnimation(float fTexX, float fTexY, float fTexY2, int nPatternAnim)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY2);
	m_pVtx[1].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY2);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY);
	m_pVtx[3].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// スクロール生成
// ==========================================================
void CScene2D::SetMove(float fTexX, float fTexY, D3DXVECTOR3 fMove)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);
	
	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(0.0f + fMove.x, 0.0f + fMove.y);
	m_pVtx[1].tex = D3DXVECTOR2(fTexX + fMove.x, 0.0f + fMove.y);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f + fMove.x, fTexY + fMove.y);
	m_pVtx[3].tex = D3DXVECTOR2(fTexX + fMove.x, fTexY + fMove.y);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 色
// ==========================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
   	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);
	// 頂点カラー
  	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ==========================================================
// 生成
// ==========================================================
CScene2D *CScene2D::Create(void)
{
	// シーン動的に確保
	m_pScene2D = new CScene2D(CScene::PRIORITY_PLAYER);

	// シーン初期化
	m_pScene2D->Init();

	// 値を返す
	return m_pScene2D;
}

// ==========================================================
// 当たり判定
// ==========================================================
bool CScene2D::SetCollision(D3DXVECTOR3 pos, int sizeX, int sizeY, D3DXVECTOR3 PartnerPos)
{
	bool bCollide = false;

	//if (type == CEnemy::ENEMYTYPE_SPIDER)
	//{
	//	if ((pos.x - sizeX / 2 <= PartnerPos.x) &&
	//		(pos.x + sizeX / 2 >= PartnerPos.x) &&
	//		(pos.y - SPIDERPOS_Y <= PartnerPos.y) &&
	//		(pos.y + SPIDERPOS_Y2 / 2 >= PartnerPos.y))
	//	{
	//		bCollide = true;
	//	}
	//}

	//else
	//{
		if ((pos.x - sizeX / 2 <= PartnerPos.x) &&
			(pos.x + sizeX / 2 >= PartnerPos.x) &&
			(pos.y - sizeY / 2 <= PartnerPos.y) &&
			(pos.y + sizeY / 2 >= PartnerPos.y))
		{
			bCollide = true;
		}

	//}
	return bCollide;
}

// ==========================================================
// 矩形と矩形の衝撃判定処理
// ==========================================================
bool CScene2D::HitBlock(D3DXVECTOR3 * block, D3DXVECTOR3 * player, D3DXVECTOR3 * BposOld, D3DXVECTOR3 * PposOld, float blockRadias_x, float blockRadias_y, float playerRadias_x, float playerRadias_y)
{
	m_bLand = false;

	//左右
	if (block->x + blockRadias_x >= player->x - playerRadias_x && block->x - blockRadias_x <= player->x + playerRadias_x)
	{
		//上に乗ってる
		if (player->y + playerRadias_y > block->y - blockRadias_y
			&& PposOld->y <= block->y - blockRadias_y)
		{
			player->y = block->y - blockRadias_y - PLAYERSIZE_Y / 2;
			m_bLand = true;
		}

		//頭
		else if (player->y - playerRadias_y <= block->y + blockRadias_y
			&& PposOld->y >= block->y + blockRadias_y)
		{
			player->y = block->y + blockRadias_y + PLAYERSIZE_Y / 2;
			//m_bLand = true;
		}
	}

	//上下
	if (block->y + blockRadias_y > player->y - playerRadias_y && block->y - blockRadias_y < player->y + playerRadias_y)
	{
		//左
		if (player->x + playerRadias_x >= block->x - blockRadias_x
			&& PposOld->x <= block->x - blockRadias_x)
		{
			player->x = block->x - blockRadias_x * 2 - 0.5f;
			//m_bLand = true;
		}
		//右
		else if (player->x - playerRadias_x <= block->x + blockRadias_x
			&& PposOld->x >= block->x + blockRadias_x)
		{
			player->x = block->x + blockRadias_x * 2 + 0.5f;
			//m_bLand = true;
		}
	}

	return m_bLand;
}
// ==========================================================
// ポリゴンの配置
// ==========================================================
void CScene2D::SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pos = pos;
	// 移動座標の設定
	m_pVtx[0].pos = D3DXVECTOR3(pos.x - sinf(fAngle + rot.x) * fLength, pos.y - cosf(fAngle) * fLength, 0.0f);
	m_pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(fAngle + rot.x) * fLength, pos.y - cosf(fAngle) * fLength, 0.0f);
	m_pVtx[2].pos = D3DXVECTOR3(pos.x - sinf(fAngle + rot.x) * fLength, pos.y + cosf(fAngle) * fLength, 0.0f);
	m_pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(fAngle + rot.x) * fLength, pos.y + cosf(fAngle) * fLength, 0.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}
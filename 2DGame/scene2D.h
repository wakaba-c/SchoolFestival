// ==========================================================
//
// 描画2D[scene2D.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

#define POLGON_X (100)			// X座標
#define POLGON_Y (100)			// Y座標
#define CENTERPOS_NORMAL (2)
#define SPIDERPOS_Y (20)
#define SPIDERPOS_Y2 (80)

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CScene2D : public CScene
{
public:
	CScene2D(PRIORITY type);	// コンストラクタ
	~CScene2D();				// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void BindTex(LPDIRECT3DTEXTURE9 tex);
	void SetAnimation(float fTexX, float fTexY,float fTexY2, int nPatternAnim);
	void SetMove(float fTexX, float fTexY, D3DXVECTOR3 fMove);
	void SetCol(D3DXCOLOR col);
	bool SetCollision(D3DXVECTOR3 pos, int sizeX, int sizeY, D3DXVECTOR3 PartnerPos);
	bool HitBlock(D3DXVECTOR3 *block, D3DXVECTOR3 *player, D3DXVECTOR3 *BposOld, D3DXVECTOR3 *PposOld,
					float blockRadias_x, float blockRadias_y, float playerRadias_x, float playerRadias_y);
	void SetPos(D3DXVECTOR3 pos,float fSizeX,float fSizeY,int nCenterPos);
	void SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	static CScene2D *Create(void);

protected:

private:
	static CScene2D *m_pScene2D;

	LPDIRECT3DTEXTURE9 m_pTexturePolygon;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// バッファ
	VERTEX_2D *m_pVtx;						// 頂点格納
	D3DXVECTOR3 m_pos;						// 現在の位置
	D3DXVECTOR3 m_posOld;					//前回の位置
	D3DXVECTOR3 m_rot;						// 回転
	
	float m_fAngle;							// 角度
	float m_fLength;						// 長さ
	bool m_bLand;							//ブロックに立っているかどうか
};
#endif
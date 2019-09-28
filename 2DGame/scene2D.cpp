// ==========================================================
//
// �`��2D[renderer.cpp]
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
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
CScene2D *CScene2D::m_pScene2D;

// ==========================================================
// �O���[�o���ϐ�
// ==========================================================

// ==========================================================
//
// �R���X�g���N�^
//
// ==========================================================
CScene2D::CScene2D(PRIORITY type) : CScene(type)
{

}

// ==========================================================
//
// �f�X�g���N�^
//
// ==========================================================
CScene2D::~CScene2D()
{

}

// ==========================================================
// �|���S��������
// ==========================================================
void CScene2D::Init(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �|���S��������
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = atan2f(POLGON_X, POLGON_Y);
	m_fLength = sqrtf(POLGON_X * POLGON_X + POLGON_Y * POLGON_Y);
	m_bLand = false;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);
	
	// ���W�̐ݒ�
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (POLGON_X / 2), m_pos.y - (POLGON_Y / 2), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (POLGON_X / 2), m_pos.y - (POLGON_Y / 2), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (POLGON_X / 2), m_pos.y + (POLGON_Y / 2), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (POLGON_X / 2), m_pos.y + (POLGON_Y / 2), 0);
	// 1.0�ŌŒ�A�������W
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;
	// ���_�J���[
	m_pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �|���S���I��
// ==========================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ==========================================================
// �|���S���X�V
// ==========================================================
void CScene2D::Update(void)
{

}

// ==========================================================
// �|���S���̕`�揈��
// ==========================================================
void CScene2D::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, m_pTexturePolygon);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ==========================================================
// �e�N�X�`������
// ==========================================================
void CScene2D::BindTex(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexturePolygon = tex;
}

// ==========================================================
// �|���S���̔z�u
// ==========================================================
void CScene2D::SetPos(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCenterPos)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pos = pos;
	// �ړ����W�̐ݒ�
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - (fSizeX / nCenterPos), m_pos.y - (fSizeY / nCenterPos), 0);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x + (fSizeX / nCenterPos), m_pos.y - (fSizeY / nCenterPos), 0);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x - (fSizeX / nCenterPos), m_pos.y + (fSizeY / nCenterPos), 0);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + (fSizeX / nCenterPos), m_pos.y + (fSizeY / nCenterPos), 0);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �A�j���[�V��������
// ==========================================================
void CScene2D::SetAnimation(float fTexX, float fTexY, float fTexY2, int nPatternAnim)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY2);
	m_pVtx[1].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY2);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY);
	m_pVtx[3].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �X�N���[������
// ==========================================================
void CScene2D::SetMove(float fTexX, float fTexY, D3DXVECTOR3 fMove)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);
	
	// �e�N�X�`�����W�̐ݒ�
	m_pVtx[0].tex = D3DXVECTOR2(0.0f + fMove.x, 0.0f + fMove.y);
	m_pVtx[1].tex = D3DXVECTOR2(fTexX + fMove.x, 0.0f + fMove.y);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f + fMove.x, fTexY + fMove.y);
	m_pVtx[3].tex = D3DXVECTOR2(fTexX + fMove.x, fTexY + fMove.y);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// �F
// ==========================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
   	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);
	// ���_�J���[
  	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;
	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ==========================================================
// ����
// ==========================================================
CScene2D *CScene2D::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pScene2D = new CScene2D(CScene::PRIORITY_PLAYER);

	// �V�[��������
	m_pScene2D->Init();

	// �l��Ԃ�
	return m_pScene2D;
}

// ==========================================================
// �����蔻��
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
// ��`�Ƌ�`�̏Ռ����菈��
// ==========================================================
bool CScene2D::HitBlock(D3DXVECTOR3 * block, D3DXVECTOR3 * player, D3DXVECTOR3 * BposOld, D3DXVECTOR3 * PposOld, float blockRadias_x, float blockRadias_y, float playerRadias_x, float playerRadias_y)
{
	m_bLand = false;

	//���E
	if (block->x + blockRadias_x >= player->x - playerRadias_x && block->x - blockRadias_x <= player->x + playerRadias_x)
	{
		//��ɏ���Ă�
		if (player->y + playerRadias_y > block->y - blockRadias_y
			&& PposOld->y <= block->y - blockRadias_y)
		{
			player->y = block->y - blockRadias_y - PLAYERSIZE_Y / 2;
			m_bLand = true;
		}

		//��
		else if (player->y - playerRadias_y <= block->y + blockRadias_y
			&& PposOld->y >= block->y + blockRadias_y)
		{
			player->y = block->y + blockRadias_y + PLAYERSIZE_Y / 2;
			//m_bLand = true;
		}
	}

	//�㉺
	if (block->y + blockRadias_y > player->y - playerRadias_y && block->y - blockRadias_y < player->y + playerRadias_y)
	{
		//��
		if (player->x + playerRadias_x >= block->x - blockRadias_x
			&& PposOld->x <= block->x - blockRadias_x)
		{
			player->x = block->x - blockRadias_x * 2 - 0.5f;
			//m_bLand = true;
		}
		//�E
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
// �|���S���̔z�u
// ==========================================================
void CScene2D::SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pos = pos;
	// �ړ����W�̐ݒ�
	m_pVtx[0].pos = D3DXVECTOR3(pos.x - sinf(fAngle + rot.x) * fLength, pos.y - cosf(fAngle) * fLength, 0.0f);
	m_pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(fAngle + rot.x) * fLength, pos.y - cosf(fAngle) * fLength, 0.0f);
	m_pVtx[2].pos = D3DXVECTOR3(pos.x - sinf(fAngle + rot.x) * fLength, pos.y + cosf(fAngle) * fLength, 0.0f);
	m_pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(fAngle + rot.x) * fLength, pos.y + cosf(fAngle) * fLength, 0.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}
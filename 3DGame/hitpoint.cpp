//=============================================================================
//
// �̗͏��� [hitpoint.cpp]
// Author : Seiya Takahashi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "hitpoint.h"
#include "fade.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTURE_ADD_HPbar	"data/tex/HPbar.png"		//�ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_ADD_HPframe "data/tex/HPFrame.png"		//�ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_HITPOINT (2)									//�e�N�X�`������
#define MAX_WIDTH (700.0f)									//�摜�̕��̍ő�l
#define MAX_HEIGHT (70.0f)									//�摜�̍����̍ő�l

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureHITPOINT[MAX_HITPOINT] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHITPOINT = NULL;

HITPOINT g_HitPoint;
//=============================================================================
// ����������
//=============================================================================
void InitHitPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *pPlayer = GetPlayer();
	
	g_HitPoint.fMaxHp = pPlayer->fLife;
	g_HitPoint.fNowHp = g_HitPoint.fMaxHp;
	g_HitPoint.fWidth = MAX_WIDTH;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HPbar, &g_pTextureHITPOINT[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ADD_HPframe, &g_pTextureHITPOINT[1]);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HITPOINT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHITPOINT,
		NULL);

	//�e�N�X�`���ݒ�
	SetVertexHitPoint(0, D3DXVECTOR3(100, 630, 0), D3DXCOLOR(0.0f, 1.0f, 0.25f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

	SetVertexHitPoint(1, D3DXVECTOR3(100, 630, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

}
//=============================================================================
// �I������
//=============================================================================
void UninitHitPoint(void)
{
	int nCntTex;

	for (nCntTex = 0; nCntTex < MAX_HITPOINT; nCntTex++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureHITPOINT[nCntTex] != NULL)
		{
			g_pTextureHITPOINT[nCntTex]->Release();
			g_pTextureHITPOINT[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffHITPOINT != NULL)
	{
		g_pVtxBuffHITPOINT->Release();
		g_pVtxBuffHITPOINT = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateHitPoint(void)
{
	PLAYER *pPlayer = GetPlayer();
	float NowHP;
	float fWidth;																	//���݂̉摜�̕�

	g_HitPoint.fNowHp = pPlayer->fLife;												//���݂�HP

	NowHP = g_HitPoint.fNowHp / g_HitPoint.fMaxHp;									//HP�̔䗦�v�Z
	fWidth = NowHP * g_HitPoint.fWidth;												//�`�悷�ׂ��摜����������Ȃ̂�

	if (NowHP < 0.25f)
	{
		SetVertexHitPoint(0, D3DXVECTOR3(100, 630, 0), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), fWidth, MAX_HEIGHT);
	}
	else
	{
		SetVertexHitPoint(0, D3DXVECTOR3(100, 630, 0), D3DXCOLOR(0.0f, 1.0f, 0.25f, 1.0f), fWidth, MAX_HEIGHT);
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawHitPoint(void)
{
	int nCntTex;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffHITPOINT, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTex = 0; nCntTex < MAX_HITPOINT; nCntTex++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureHITPOINT[nCntTex]);

		//�|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);

	}

}

//=============================================================================
// ���_�̐ݒ�
//=============================================================================
void SetVertexHitPoint(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;										//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffHITPOINT->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += index * 4;					//���_��4�����Z

	// ���_���̐ݒ�
	//���_���W�̐ݒ�(���x���W + �Ԋu * nCntScore (+ ��), ���y���W)
	pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y, 0);
	pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0);

	//1.0�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[�`���[�g�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	g_pVtxBuffHITPOINT->Unlock();

}

//=============================================================================
// �̗͂̎擾
//=============================================================================
HITPOINT GetHitPoint(void)
{
	return g_HitPoint;
}
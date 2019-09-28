//=============================================================================
//
// ���f������ [enemy.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "load.h"
#include "trajectory.h"
#include "model.h"
#include "player.h"
#include "meshorbit.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "telop.h"
#include "bullet.h"
#include "time.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_GRAVITY 2
#define MAX_ENEMY	1
#define MAX_LIFE 30
#define ENEMY_GRAVITY 0.5

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void AnimationEnemy(float fDifference);
bool CollisionExtinction(int nNum);
void AnimationSwitchEnemy(MOTIONENEMY nType);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;		//�e�N�X�`���ւ̃|�C���^
//float g_fVelocity = 30;
ENEMY g_aEnemy[MAX_ENEMY];										//�v���C���[�̍\����
ENEMYPARTS g_aEnemyParts[30];						//parts�̍\����
int g_nMaxEnemy = 15;								//�p�[�c�̍ő吔
D3DXVECTOR3				g_EffectPos;
D3DXVECTOR3				g_EffectPos1;
D3DXVECTOR3				g_EffectPos2;
D3DXVECTOR3				g_EffectPos3;
D3DXVECTOR3				g_EffectPos4;
D3DXVECTOR3				g_EffectPos5;
D3DXVECTOR3				g_EffectPos6;
D3DXVECTOR3				g_EffectPos7;
D3DXVECTOR3				g_EffectPos8;
int g_Number = 3;									//�G�̐l��
float g_fVelocityEnemy = 10;
int g_bAnimationEnemy = true;							//�A�j���[�V�����̃v���C


//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_EffectPos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_EffectPos1 = D3DXVECTOR3(10.0f, 20.0f, 0.0f);
	g_EffectPos2 = D3DXVECTOR3(-10.0f, 20.0f, 0.0f);
	g_EffectPos3 = D3DXVECTOR3(-15.0f, 20.0f, 0.0f);
	g_EffectPos4 = D3DXVECTOR3(15.0f, 20.0f, 0.0f);
	g_EffectPos5 = D3DXVECTOR3(0.0f, 20.0f, 10.0f);
	g_EffectPos6 = D3DXVECTOR3(0.0f, 20.0f, 15.0f);
	g_EffectPos7 = D3DXVECTOR3(0.0f, 20.0f, -15.0f);
	g_EffectPos8 = D3DXVECTOR3(0.0f, 20.0f, -10.0f);
	for (int nCntModel = 0; nCntModel < MAX_ENEMY; nCntModel++)
	{
		// �ʒu�E�����̏����ݒ�
		for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
		{
			g_aEnemy[nCntModel].nLife = MAX_LIFE;
			g_aEnemy[nCntModel].aModel[nCntEnemy].posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntModel].aModel[nCntEnemy].originPos - D3DXVECTOR3(0, 0, 0);
			g_aEnemy[nCntModel].aModel[nCntEnemy].originRot - D3DXVECTOR3(0, 0, 0);
			g_aEnemy[nCntModel].bJump = false;
			g_aEnemy[nCntModel].bUse = false;

			g_aEnemy[nCntModel].CurrentEnemyFrame = 0;								//���݂̃t���[����
			g_aEnemy[nCntModel].CurrentEnemyKey = 0;								//���݂̃L�[
			g_aEnemy[nCntModel].nAnimationEnemy = 0;								//���݂̃A�j���[�V����
			g_aEnemy[nCntModel].nValueH = 0;										//�R���g���[���[
			g_aEnemy[nCntModel].nValueV = 0;										//�R���g���[���[
			g_aEnemy[nCntModel].nCntAttack = 0;
		}
	}

	//�X�N���v�g�̓ǂݍ���
	g_nMaxEnemy = LoadEnemy();

	for (int nCntEnemy = 0; nCntEnemy < g_nMaxEnemy; nCntEnemy++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(g_aEnemy[0].aModel[nCntEnemy].sAdd, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aEnemyParts[nCntEnemy].pBuffMatEnemy, NULL, &g_aEnemyParts[nCntEnemy].nNumMatEnemy, &g_aEnemyParts[nCntEnemy].pMeshEnemy);
	}

	// �ʒu�E�����̏����ݒ�
	for (int nCntModel = 0; nCntModel < MAX_ENEMY; nCntModel++)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
		{
			g_aEnemy[nCntModel].aModel[nCntEnemy].posEnemy = g_aEnemy[0].aModel[nCntEnemy].originPos;
			g_aEnemy[nCntModel].aModel[nCntEnemy].rotEnemy = g_aEnemy[0].aModel[nCntEnemy].originRot;
		}
		g_aEnemy[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (GetMode() == MODE_GAME && IsFinish())
	{
		SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 25);

		//SetEnemy(D3DXVECTOR3(00.0f, 0.0f, 100.0f), 25);

		//SetEnemy(D3DXVECTOR3(-100.0f, 0.0f, 100.0f), 25);
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < g_nMaxEnemy; nCntEnemy++)
	{
		// ���b�V���̊J��
		if (g_aEnemyParts[nCntEnemy].pMeshEnemy != NULL)
		{
			g_aEnemyParts[nCntEnemy].pMeshEnemy->Release();
			g_aEnemyParts[nCntEnemy].pMeshEnemy = NULL;
		}

		// �}�e���A���̊J��
		if (g_aEnemyParts[nCntEnemy].pBuffMatEnemy != NULL)
		{
			g_aEnemyParts[nCntEnemy].pBuffMatEnemy->Release();
			g_aEnemyParts[nCntEnemy].pBuffMatEnemy = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	float fAngle = 0;
	float fAngle1 = 0;
	float fSpeed = 2.5;

	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();
	BULLET *pBullet = GetBullet();

	float fDifference_x;
	float fDifference_z;
	float fDifference;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (GetTriggerKeyboard(DIK_7))
		{
			g_aEnemy[nCntEnemy].nLife += 1;
		}
		if (GetTriggerKeyboard(DIK_U))
		{
			g_aEnemy[nCntEnemy].nLife += -1;
		}

		//�G���g���Ă�����
		if (g_aEnemy[nCntEnemy].bUse)
		{
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			fDifference_x = g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x;
			fDifference_z = g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z;
			fDifference = (float)sqrt(fDifference_x * fDifference_x + fDifference_z * fDifference_z);

			g_aEnemy[nCntEnemy].nCntAttack++;

			g_aEnemy[nCntEnemy].nWait += 1;

			AnimationEnemy(fDifference);

			//�e���b�v���Ȃ���
			if (GetTelopState() == false)
			{
				if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_1 && g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_2)
				{
					//================�R���g���[���[===================//
					GetJoypadStickLeft(1, &g_aEnemy[nCntEnemy].nValueH, &g_aEnemy[nCntEnemy].nValueV);

					//�p�x
					g_aEnemy[nCntEnemy].dest.y = atan2f(-g_aEnemy[nCntEnemy].move.x, -g_aEnemy[nCntEnemy].move.z);

					//�ړ�
					g_aEnemy[nCntEnemy].move -= D3DXVECTOR3(sinf(D3DX_PI * 1.0f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueV), 0, cosf(D3DX_PI * 1.0f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueV));
					g_aEnemy[nCntEnemy].move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueH), 0, cosf(D3DX_PI * 0.5f + pCamera->rot.y) * (1.5f * g_aEnemy[nCntEnemy].nValueH));

					if (fabs(g_aEnemy[nCntEnemy].move.x) > 1 || fabs(g_aEnemy[nCntEnemy].move.z) > 1)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_RUN)
						{
							AnimationSwitchEnemy(MOTIONENEMY_RUN);
						}
					}

					//���ړ�
					if (GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].dest.y = D3DX_PI * 0.5f + pCamera->rot.y;
						pCamera->nCount = 0;
					}
					//�E�ړ�
					if (GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * 1.0f;
						g_aEnemy[nCntEnemy].dest.y = -D3DX_PI * 0.5f + pCamera->rot.y;
						pCamera->nCount = 0;
					}
					//�U��
					if (GetTriggerKeyboard(DIK_LSHIFT) || GetControllerTrigger(1, JOYPADKEY_X))
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_1 && g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_2)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_1;
							g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
							g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
						}
					}
				}

				//�W�����v
				if (GetTriggerKeyboard(DIK_W) || GetControllerPress(1, JOYPADKEY_A))
				{
					if (g_aEnemy[nCntEnemy].bJump == false)
					{
						g_aEnemy[nCntEnemy].bJump = true;
						g_aEnemy[nCntEnemy].move.y += g_fVelocityEnemy;
					}
				}
			}

			//���W�̈ړ�
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//����
			g_aEnemy[nCntEnemy].move.x += (0 - g_aEnemy[nCntEnemy].move.x) * 0.2f;
			g_aEnemy[nCntEnemy].move.z += (0 - g_aEnemy[nCntEnemy].move.z) * 0.2f;

			//�L�����N�^�[�̐U�����
			g_aEnemy[nCntEnemy].Difference.y = g_aEnemy[nCntEnemy].rot.y - g_aEnemy[nCntEnemy].dest.y;

			if (g_aEnemy[nCntEnemy].Difference.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].Difference.y -= D3DX_PI * 2;
			}
			else if (g_aEnemy[nCntEnemy].Difference.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].Difference.y += D3DX_PI * 2;
			}

			g_aEnemy[nCntEnemy].rot.y -= g_aEnemy[nCntEnemy].Difference.y * 0.1f;

			if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}
			else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}

			if (GetTriggerKeyboard(DIK_F6))
			{
				if (g_bAnimationEnemy)
				{
					g_bAnimationEnemy = false;
				}
				else if (!g_bAnimationEnemy)
				{
					g_bAnimationEnemy = true;
				}
			}

			//�d��
			g_aEnemy[nCntEnemy].move.y -= ENEMY_GRAVITY;

			//���f���̏�ɂ��鎞
			if (CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move) == true)
			{
				g_aEnemy[nCntEnemy].bJump = false;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
			}
			else
			{
				g_aEnemy[nCntEnemy].bJump = true;
			}

			//�v���C���[�����ɒ�������
			if (g_aEnemy[nCntEnemy].pos.y <= 0)
			{
				g_aEnemy[nCntEnemy].pos.y = 0.0f;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
				g_aEnemy[nCntEnemy].bJump = false;
			}

			//�G�̓����蔻��
			if (pBullet->pos.x < g_aEnemy[nCntEnemy].pos.x - 10.0f &&
				pBullet->pos.x > g_aEnemy[nCntEnemy].pos.x + 10.0f &&
				pBullet->pos.z < g_aEnemy[nCntEnemy].pos.z + 40.0f &&
				pBullet->pos.z > g_aEnemy[nCntEnemy].pos.z)
			{
				g_aEnemy[nCntEnemy].nLife -= 5;

				if (g_aEnemy[nCntEnemy].nLife <= 0)
				{
					g_aEnemy[nCntEnemy].bUse = false;
					DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadow);
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize1 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize1 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize = rand() % 10 + 10;
						float fSize2 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize1 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize = rand() % 10 + 10;
						float fSize3 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % 2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(nMoveX, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						float fSize = rand() % 10 + 10;
						float nMoveY = rand() % 2;
						float nMoveX = rand() % -2;
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(-0.5, nMoveY, 0);
						SetEffect(pPlayer->pos + g_EffectPos3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}
					for (int nCnt = 0; nCnt < MAX_LIFE_EFFECT; nCnt++)
					{
						int nLife = (rand() % MAX_RANDOM) * MAX_RATE + MAX_BASE;
						float fSize = rand() % 10 + 10;
						float fSize4 = rand() % 10 + 1;
						fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						fAngle1 = 3.57f * nCnt;
						D3DXVECTOR3 move = D3DXVECTOR3(0, 0.5f, 0);
						SetEffect(pPlayer->pos + g_EffectPos4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), move, fSize, nLife, EFFECTTYPE_LIFE);
					}

				}
			}
			//���f���̓����蔻��
			CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move);

			//�����蔻��
			//if (SphereModel(COLLISIONTYPE_WEAPON, &pBullet->pos, &g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].move, &pPlayer->move, 10, 10))
			//{
			//	g_aEnemy[nCntEnemy].nLife -= 1;
			//}

			//�G���U�����󂯂Ă��邩�ǂ���
			if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_DAMAGE)
			{
				//�v���C���[�̍U�����@���ʏ�U����������
				if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_1)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 5, 5))
					{
						g_aEnemy[nCntEnemy].nLife -= 3;
						g_aEnemy[nCntEnemy].nCntAttack = 0;
						g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_DAMAGE;
					}
				}

				//�v���C���[�̍U�����@���ʏ�U����������
				if (pPlayer->nAnimationType == MOTIONTYPE_ATTACK_2)
				{
					if (SphereModel(COLLISIONTYPE_WEAPON, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &g_aEnemy[nCntEnemy].pos, &pPlayer->move, &g_aEnemy[nCntEnemy].move, 5, 5))
					{
						g_aEnemy[nCntEnemy].nLife -= 5;
						g_aEnemy[nCntEnemy].nCntAttack = 0;
						g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_DAMAGE;
					}
				}

				//�G��HP��0�ɂȂ�����
				if (g_aEnemy[nCntEnemy].nLife <= 0)
				{
					g_aEnemy[nCntEnemy].bUse = false;
					DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadow);

					if (CollisionExtinction(-1))
					{
						//SetModel(ITEMTYPE_CLOUD_3, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);
					}
				}
			}

			//�v���C���[���_���[�W���󂯂Ă��邩�ǂ���
			if (pPlayer->nAnimationType != MOTIONTYPE_DAMAGE)
			{
				//�G�̍U�����U����������
				if (g_aEnemy[nCntEnemy].nAnimationEnemy == MOTIONENEMY_ATTACK_1)
				{
					//�����蔻��
					if (SphereModel(COLLISIONTYPE_WEAPON, &g_aEnemy[nCntEnemy].pos, &D3DXVECTOR3(GetOrbit()->_41, GetOrbit()->_42, GetOrbit()->_43), &pPlayer->move, &g_aEnemy[nCntEnemy].move, 50, 50))
					{
						pPlayer->nLife -= 3;
						pPlayer->nAnimationType = MOTIONTYPE_DAMAGE;

						//�v���C���[�̗̑͂�0�ɂȂ�����
						if (pPlayer->nLife <= 0)
						{
							SetGameState(GAMESTATE_END);
							SetResultState(RESULTSTATE_FAILD);
						}
					}
				}
			}

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);
		}
	}

	//�����蔻��
	if (SphereModel(COLLISIONTYPE_WEAPON, &pBullet->pos, &pPlayer->pos, &pPlayer->move, &pPlayer->move, 10, 10))
	{
		//pPlayer->nLife -= 1;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9	matDef;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);			// ���[���h�}�g���b�N�X�̏�����

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			for (int nCntModel = 0; nCntModel < g_nMaxEnemy; nCntModel++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy);

				// �X�P�[���𔽉f


				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aModel[nCntModel].rotEnemy.y, g_aEnemy[nCntEnemy].aModel[nCntModel].rotEnemy.x, g_aEnemy[nCntEnemy].aModel[nCntModel].rotEnemy.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aModel[nCntModel].posEnemy.x, g_aEnemy[nCntEnemy].aModel[nCntModel].posEnemy.y, g_aEnemy[nCntEnemy].aModel[nCntModel].posEnemy.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &mtxTrans);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);					//���ׂẴf�[�^���^

																						//�e�q�֌W���������Ă���ꍇ
				if (g_aEnemy[nCntEnemy].aModel[nCntModel].nParent == -1)
				{
					//�}�g���b�N�X�̍���
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].mtxWorld);
				}
				else
				{
					//�}�g���b�N�X�̍���
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy, &g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntModel].nParent].mtxWorldEnemy);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorldEnemy);					//���ׂẴf�[�^���^

																													// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aEnemyParts[nCntModel].pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aEnemyParts[nCntModel].nNumMatEnemy; nCntMat++)
				{
					//�F
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �`��
					g_aEnemyParts[nCntModel].pMeshEnemy->DrawSubset(nCntMat);
				}
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �v���C���[�A�j���[�V����
//=============================================================================
void AnimationEnemy(float fDifference)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			if (GetTriggerKeyboard(DIK_5))
			{
				g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_1;
				g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
				g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
			}

			//���[�V�����̍ő�l����
			if (g_aEnemy[nCntEnemy].nAnimationEnemy < 0)
			{
				g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_MAX;
			}
			else if (g_aEnemy[nCntEnemy].nAnimationEnemy > MOTIONENEMY_MAX)
			{
				g_aEnemy[nCntEnemy].nAnimationEnemy = 0;
			}

			//�ŏ��̃t���[���̎�
			if (g_aEnemy[nCntEnemy].CurrentEnemyFrame == 0)
			{
				for (int nCntMotion = 0; nCntMotion < g_nMaxEnemy; nCntMotion++)
				{

					//pos�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos.x = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].pos.x + g_aEnemy[nCntEnemy].aModel[nCntMotion].originPos.x) - g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy.x) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos.y = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].pos.y + g_aEnemy[nCntEnemy].aModel[nCntMotion].originPos.y) - g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy.y) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos.z = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].pos.z + g_aEnemy[nCntEnemy].aModel[nCntMotion].originPos.z) - g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy.z) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;

					//rot�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot.x = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].rot.x + g_aEnemy[nCntEnemy].aModel[nCntMotion].originRot.x) - g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy.x) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot.y = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].rot.y + g_aEnemy[nCntEnemy].aModel[nCntMotion].originRot.y) - g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy.y) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
					g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot.z = ((g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].rot.z + g_aEnemy[nCntEnemy].aModel[nCntMotion].originRot.z) - g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy.z) / g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame;
				}
			}

			//����ȊO�̃t���[��
			if (g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].nMaxKey > g_aEnemy[nCntEnemy].CurrentEnemyKey)
			{
				for (int nCntMotion = 0; nCntMotion < g_nMaxEnemy; nCntMotion++)
				{
					//rot�̈ړ�
					g_aEnemy[nCntEnemy].aModel[nCntMotion].rotEnemy += g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentRot;

					//pos�̈ړ�
					g_aEnemy[nCntEnemy].aModel[nCntMotion].posEnemy += g_aEnemy[nCntEnemy].aModel[nCntMotion].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].CurrentPos;
				}
			}

			//�t���[���̍ő吔�ɖ����Ȃ��ꍇ
			if (g_aEnemy[nCntEnemy].CurrentEnemyFrame < g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame)
			{
				g_aEnemy[nCntEnemy].CurrentEnemyFrame++;
			}

			//�t���[�����̍ő�l�ɒB�����ꍇ
			if (g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].aKey[g_aEnemy[nCntEnemy].CurrentEnemyKey].nFrame <= g_aEnemy[nCntEnemy].CurrentEnemyFrame)
			{
				g_aEnemy[nCntEnemy].CurrentEnemyKey++;
				g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
			}

			//�L�[���̍ő�l�ɒB�����ꍇ
			if (g_aEnemy[nCntEnemy].CurrentEnemyKey == g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].nMaxKey)
			{
				//�A�j���[�V�������A�^�b�N���[�V������������
				if (g_aEnemy[nCntEnemy].nAnimationEnemy == MOTIONENEMY_ATTACK_1)
				{
					if (fDifference < 150)
					{
						if (g_aEnemy[nCntEnemy].nAnimationEnemy != MOTIONENEMY_ATTACK_2)
						{
							g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_ATTACK_2;
							g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
							g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
							g_aEnemy[nCntEnemy].nWait = 0;
						}
					}
				}

				//���[�v���邩�ǂ���
				else if (!g_aEnemy[nCntEnemy].aModel[0].aMotion[g_aEnemy[nCntEnemy].nAnimationEnemy].nLoop)
				{
					g_aEnemy[nCntEnemy].nAnimationEnemy = MOTIONENEMY_NEUTRAL;
					g_aEnemy[nCntEnemy].CurrentEnemyFrame = 0;
				}

				//�L�[�̃��Z�b�g
				g_aEnemy[nCntEnemy].CurrentEnemyKey = 0;
			}
		}
	}
}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nLife)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, D3DXVECTOR3(30.0f, 10.0f, 30.0f));
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
bool CollisionExtinction(int nNum)
{
	bool bDispersion = false;
	g_Number += nNum;

	if (g_Number <= 0)
	{
		SetStageTelop(TELOPTYPE_MESSAGEWINDOW, TELOPPAGE_END);
		bDispersion = true;
	}
	else
	{
		if (g_Number == 2)
		{
			SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 25);
		}
		else if (g_Number == 1)
		{
			SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 25);
		}
	}

	return bDispersion;
}

//=============================================================================
// �A�j���[�V�����̐؂�ւ�
//=============================================================================
void AnimationSwitchEnemy(MOTIONENEMY nType)
{
	g_aEnemy[0].nAnimationEnemy = nType;
	g_aEnemy[0].CurrentEnemyKey = 0;
	g_aEnemy[0].CurrentEnemyFrame = 0;

	//switch (nType)
	//{
	//case MOTIONTYPE_NEUTRAL:
	//	g_nAnimationType = MOTIONTYPE_NEUTRAL;
	//	g_CurrentKey = 0;
	//	g_CurrentFrame = 0;
	//	break;
	//case MOTIONTYPE_ATTACK:
	//	break;
	//case MOTIONTYPE_RUN:
	//	break;
	//}
}

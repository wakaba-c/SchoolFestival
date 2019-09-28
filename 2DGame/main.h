// ==========================================================
// 
// ���C��[main.h]
// Author:Ryouma Inoue
// 
// ==========================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

// �C���N���[�h�t�@�C��
#include <stdio.h>
#include<Windows.h>
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"
#include "xaudio2.h"

// ==========================================================
// ���C�u�����̃����N
// ==========================================================
#pragma comment(lib,"d3d9.lib")					// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")				// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")				// DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")				// �V�X�e�����Ԏ擾�ɕK�v
#pragma comment(lib,"dinput8.lib")				// ���͂ɕK�v

// ==========================================================
// �}�N����`
// ==========================================================
#define SCREEN_WIDTH	(1280)					// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(720)					// �E�B���h�E�̍���
#define MAX_POLGON (500)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 2D�|���S�����
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// 1.0�ŌŒ�A�������W
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_2D;

int GetFPS(void);

#endif
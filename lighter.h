//=============================================================================
//
// �X�R�A���� [lighter.h]
// Author : �S���T��
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LIGHTER_OIL_MAX			(100.0f)		// �X�R�A�̍ő�l
#define LIGHTER_OIL_MIN			(1.0f)			// �X�R�A�̍ŏ��l
#define LIGHTER_OIL_DIGIT		(2)				// ����
#define LIGHTER_TIME			(1)			//���炷���߁A�t���[���̐�	
#define LIGHTER_OIL_REDUCE		(0.1f)			//���点���

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct LIGHTER
{
	BOOL		use;		//�o�����
	BOOL		out;		//�o���Ă���
	float		oil;		//�I�C��
	float		w, h;		// ���ƍ���

	XMFLOAT3	pos;		// �|���S���̈ʒu
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLighter(void);
void UninitLighter(void);
void UpdateLighter(void);
void DrawLighter(void);

void SetLighterOn(BOOL flag);
void SetLighterOff(void);
void AddOil(float n);
void CheckOil(void);
void ReduceOil(void);

LIGHTER *GetLighter(void);
//=============================================================================
//
// �X�R�A���� [score.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_MAX			(99999)		// �X�R�A�̍ő�l
#define SCORE_MIN			(0)			// �X�R�A�̍ŏ��l
#define SCORE_DIGIT			(7)			// ����
#define SCORE_SAVE			(10)		// �X�R�A�̕ۑ���


struct MATCH
{
	BOOL					Use;					// TRUE:�g���Ă���  FALSE:���g�p
	float					w, h;					// ���ƍ���
	XMFLOAT3				Pos;					// �|���S���̍��W
	int						TexNo;					// �e�N�X�`���ԍ�
	int						StandbyTime;			// �\���n�߂Ă���̎���
	int						AblazeTime;				// �R�Ď���
	int						force;					// �}�b�`���C��������
	int						num;					// �}�b�`�̖{��

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMatch(void);
void UninitMatch(void);
void UpdateMatch(void);
void DrawMatch(void);

void StandbyMatch(void);
void SetMatchForce(int force);

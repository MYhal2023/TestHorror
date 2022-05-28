//=============================================================================
//
// �}�b�`���� [match.h]
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

#define MATCH_MOVE			(0.8f)		//�}�b�`�̑���
#define	MODEL_FIRE	"data/MODEL/fire.obj"			// �ǂݍ��ރ��f����

struct MATCH
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	BOOL					Use, Out;					// TRUE:�g���Ă���  FALSE:���g�p
	float					w, h;					// ���ƍ���
	XMFLOAT3				Pos;					// �|���S���̍��W
	int						TexNo;					// �e�N�X�`���ԍ�
	int						StandbyTime;			// �\���n�߂Ă���̎���
	int						AblazeTime;				// �R�Ď���
	int						force;					// �}�b�`���C��������
	int						num;					// �}�b�`�̖{��

};

struct MATCHBOX
{
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	XMFLOAT3			pos;					// �|���S���̍��W
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	BOOL				use;
};
struct FIRE
{
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	XMFLOAT3			pos;					// �|���S���̍��W
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	BOOL				use;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMatch(void);
void UninitMatch(void);
void UpdateMatch(void);
void DrawMatch(void);
void DrawMatchStick(void);
void DrawMatchbox(void);
void DrawFire(void);
void MoveMatch(void);
void MoveMatchbox(void);
void StandbyMatch(void);
void SetMatchForce(int force);
MATCH *GetMatch(void);

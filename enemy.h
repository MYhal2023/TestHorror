//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ENEMY		(4)	// �G�ő吔

#define	ENEMY_WH		(5.0f)	// �����蔻��̑傫��

enum ENEMT_TEXTURE
{
	ENEMY_WOMAN,
	ENEMY_DEBUG,
	ENEMY_BACK,
};

enum ENEMY_STATE
{
	Patrol,
	Chase,
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	rot;			// �p�x
	XMFLOAT3	scl;			// �X�P�[��
	XMVECTOR	moveVec;		//�ړ��x�N�g��
	MATERIAL	material;		// �}�e���A��
	float		spd;			// �ړ���
	float		fWidth;			// ��
	float		fHeight;		// ����
	int			shadowIdx;		// �eID
	int			texNo;
	int			state;			//�X�e�[�g�x�[�XAI�Ɏg�p
	int			memory;
	BOOL		use;			// �g�p���Ă��邩�ǂ���

	INTERPOLATION_DATA	*tbl_adr;			// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	int					tbl_size;			// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;			// ���s����

} ENEMY;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot);
void ResetEnemy(void);
ENEMY *GetEnemy(void);
void EnemyAngleTex(int i);
int StateCheck(int i);
void EnemyInterPoration(int i);
void StateAdjust(int i);
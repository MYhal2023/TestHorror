#pragma once
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ROOM_FURNITURE (2)		//1�����̉Ƌ�̐�
#define UNIQUE_FURNITURE (5)	//�R�s�[�o���Ȃ��Ƌ�̐�
#define MAX_FURNITURE (ROOM_FURNITURE*(ROOM*2)+UNIQUE_FURNITURE)	//�g���I�u�W�F�N�g�̐�

#define BED_SCL		(1.5f)
#define TOILET_SCL	(5.0f)
#define CANDLE_SCL	(3.25f)
#define STAIRS_SCL	(2.0f)
#define CAGE_SCL	(1.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	bool				use;
	bool				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	float				size;				// �����蔻��̑傫��
	BOOL				open;
} FURNITURE;

enum
{
	BED,
	TOILET,
	CANDLE,
	STAIRS,
	CAGE
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFurniture(void);
void UninitFurniture(void);
void UpdateFurniture(void);
void DrawFurniture(void);
void SetFurniture(XMFLOAT3 pos, XMFLOAT3 rot, int type);
FURNITURE *GetFurniture(void);

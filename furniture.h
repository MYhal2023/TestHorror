#pragma once
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FURNITURE (4)	//�g���I�u�W�F�N�g�̐�

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
} FURNITURE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFurniture(void);
void UninitFurniture(void);
void UpdateFurniture(void);
void DrawFurniture(void);
void SetFurnitureDebugStage(void);
FURNITURE *GetFurniture(void);
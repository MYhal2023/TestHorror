#pragma once
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_STAGEFURNITURE (0)	//�g���I�u�W�F�N�g�̐�

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
} STAGEFURNITURE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFurnitureFirStage(void);
void UninitStageFurniture(void);
void UpdateStageFurniture(void);
void DrawStageFurniture(void);
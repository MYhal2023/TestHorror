#pragma once
//=============================================================================
//
// �A�C�e�����f������ [item.h]
// Author : 
//
//=============================================================================

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ITEM (4)	//�g���A�C�e���̐�
#define SIZE_KEY (15)	//���̓����蔻��̑傫��
#define SIZE_MATCH (25)	//���̓����蔻��̑傫��
#define SIZE_LIGHTER (25)	//���̓����蔻��̑傫��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	BOOL				use;
	BOOL				load;
	BOOL				drop;				//�A�C�e�����擾��Ԃ�(TRUE�ŗ����Ă���AFALSE�ŏE���Ă���)
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
	float				size;				// �����蔻��̑傫��
	int					type;				//�A�C�e���̎��
	int					ID;					//���݂̂Ɏg�p�B����ID�ƃh�A��ID����v���Ă���Ύg�p�\�ɂȂ�
} MODEL_ITEM;

enum ITEM_TYPE
{
	ITEM_KEY,
	ITEM_MATCH,
	ITEM_LIGHTER,
	MAXTYPE_ITEM
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void CollisionItem(XMFLOAT3 pos, float size);
BOOL CheckDoorKey(int i);
MODEL_ITEM *GetItem(void);
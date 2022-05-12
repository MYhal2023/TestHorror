//=============================================================================
//
// �A�C�e���{�b�N�X���� [itembox.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************



enum ITEM_TEXTURE
{
	MATCH_ITEM,
	LIGHTER_ITEM,
	KEY,
	RANDOM_ITEM_2,
	EMPTY,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct BOX
{
	BOOL		use;
	BOOL		open;						//�󂢂Ă��邩�ǂ���
	int			slot;						//�ԍ�
	int			TexNo;						//�A�C�e���̃e�N�X�`���ԍ�
	float		w, h, tw, th, tx, ty;		// ���ƍ����A�e�N�X�`�����W
	XMFLOAT3	pos;						// �|���S���̈ʒu
	XMFLOAT4	color;						//�F
};

struct ITEM
{
	BOOL		use;
	int			TexNo;						//�e�N�X�`���ԍ�
	int			id;							//�A�C�e����ID
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItembox(void);
void UninitItembox(void);
void UpdateItembox(void);
void DrawItembox(void);
void AddItembox(int TNum);
void IncreaseItembox(int n);
void MoveSelect();
int	ReturnSelect();
void UseItemSelect();
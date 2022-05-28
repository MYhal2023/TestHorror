//=============================================================================
//
// ���̑�UI�\������ [interface.h]
// Author : 
//
//=============================================================================
#pragma once
#define ITEM_MAX					(6)				//�A�C�e���{�b�N�X�̐�
#define ITEM_MIN					(4)				//�A�C�e���{�b�N�X�̍Œ��

//*****************************************************************************
// �}�N����`
//*****************************************************************************


enum
{
	CONTROL_TEXTURE,
};


enum UI_TEXTURE
{
	HP_RED_BG,
	HP_RED,
	HP_BAR,
	STAM_BAR,
	OIL_RED,
	OIL_BAR,
	MATCH_NUM,
	MATCH_PIC,
	MIND,
	HURT,
	INSANE,
	ITEM_BOX,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct UI_ELEMENT
{
	BOOL		use;
	int			TexNo;						//�e�N�X�`���ԍ�
	float		w, h, tw, th, tx, ty;		// ���ƍ����A�e�N�X�`�����W
	XMFLOAT3	pos;						// �|���S���̈ʒu
	XMFLOAT4	color;						//�F
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInterface(void);
void UninitInterface(void);
void UpdateInterface(void);
void DrawInterface(void);
void GaugeAnimation(void);
UI_ELEMENT *GetItemBox(void);
int GetItemMax(void);
void IncreaseItemmax(void);
void HurtAnimationOn(void);
void HurtAnimation(void);
void SanityCheck(int sanity);
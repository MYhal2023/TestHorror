//=============================================================================
//
// �����e�N�X�`�����̕\������ [text_texture.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum TEXT_TEXTURE_NUM
{
	TEXT_CANT_KEY,
	TEXT_FIRE_CANDLE,
	TEXT_NEED_KEY,
	TEXT_TAKE_KEY,
	TEXT_TAKE_LIGHTER,
	TEXT_TAKE_MATCH,
	TEXT_USE_KEY,
};

struct TEXT_TEXTURE
{
	XMFLOAT3	pos;						// �|���S���̈ʒu
	XMFLOAT4	color;						//�F
	int			TexNo;						//�e�N�X�`���ԍ�
	int			time;						//�\�����Ԃ̃J�E���g�p�ϐ�
	int			timeMax;					//�\������
	float		w, h, tw, th, tx, ty;		// ���ƍ����A�e�N�X�`�����W
	BOOL		use;
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTexttex(void);
void UninitTexttex(void);
void UpdateTexttex(void);
void DrawTexttex(void);
void SetTexture(int i);
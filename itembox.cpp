//=============================================================================
//
// �A�C�e���{�b�N�X���� [itembox.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "interface.h"
#include "itembox.h"
#include "sprite.h"
#include "game.h"
#include "time.h"
#include "player.h"
#include "lighter.h"
#include "match.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�

#define ITEM_TEXTURE_WIDTH		(40)	// �e�N�X�`���T�C�Y
#define ITEM_TEXTURE_HEIGHT		(35)	// 

#define HUNDRED						(100.0f)				//�t���[�g�^100

#define WAIT_TIME					(50)					//�Q�[�W�A�j���[�V������҂����鎞��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/match.png",
	"data/TEXTURE/lighter.png",
	"data/TEXTURE/enemy_debug.png",
	"data/TEXTURE/enemy.png",
	"data/TEXTURE/itembox.png",
};


static ITEM			g_Item[TEXTURE_MAX];							//�A�C�e���\���̃f�[�^
static BOX			g_Box[ITEM_MAX];								//�A�C�e���{�b�N�X�\��
static int			g_items = ITEM_MIN;								//�A�C�e���̐���
static int			g_Select;										//���ǂ��ɑI�����Ă���
static UI_ELEMENT	g_Selectbox;									//�I�����Ă��邽�߂̕\��

static BOOL			g_Load = FALSE;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitItembox(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Item[i].use = TRUE;
		g_Item[i].TexNo = i;
		g_Item[i].id = i;
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (i < ITEM_MIN)
		{
			g_Box[i].use = TRUE;
		}
		else
		{
			g_Box[i].use = FALSE;
		}

		g_Box[i].open = TRUE;
		g_Box[i].TexNo = 3;
		g_Box[i].slot = i;
		UI_ELEMENT	*itembox = GetItemBox();
		g_Box[i].pos.x = itembox[i].pos.x;
		g_Box[i].pos.y = itembox[i].pos.y;
		g_Box[i].pos.z = 0.0f;
		g_Box[i].w = ITEM_TEXTURE_WIDTH;
		g_Box[i].h = ITEM_TEXTURE_HEIGHT;
		g_Box[i].tw = 1.0f;		// �e�N�X�`���̕�
		g_Box[i].th = 1.0f;		// �e�N�X�`���̍���
		g_Box[i].tx = 0.0f;			// �e�N�X�`���̍���X���W
		g_Box[i].ty = 0.0f;			// �e�N�X�`���̍���Y���W
		g_Box[i].color = { 1.0f,1.0f,1.0f,0.0f };
	}


	g_Selectbox.use = TRUE;
	g_Selectbox.TexNo = EMPTY;
	g_Selectbox.w = ITEM_TEXTURE_WIDTH;
	g_Selectbox.h = ITEM_TEXTURE_HEIGHT;
	g_Selectbox.tw = 1.0f;		// �e�N�X�`���̕�
	g_Selectbox.th = 1.0f;		// �e�N�X�`���̍���
	g_Selectbox.tx = 0.0f;			// �e�N�X�`���̍���X���W
	g_Selectbox.ty = 0.0f;			// �e�N�X�`���̍���Y���W
	UI_ELEMENT	*itembox = GetItemBox();
	g_Selectbox.pos.x = itembox[0].pos.x;
	g_Selectbox.pos.y = itembox[0].pos.y;
	g_Selectbox.pos.z = 0.0f;
	g_Selectbox.color = { 1.0f,0.0f,0.0f,1.0f };


	g_Select = 0;
	// ������



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItembox(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItembox(void)
{
	if (GetItemMax() != g_items)
	{
		g_items = GetItemMax();
	}

	MoveSelect();

#ifdef _DEBUG	// �f�o�b�O����\������
	if (GetKeyboardTrigger(DIK_9))
	{
		AddItembox(g_Item[MATCH_ITEM].TexNo);
	}
	if (GetKeyboardTrigger(DIK_0))
	{
		AddItembox(g_Item[LIGHTER_ITEM].TexNo);
	}
	if (GetKeyboardTrigger(DIK_8))
	{
		AddItembox(g_Item[RANDOM_ITEM_1].TexNo);
	}
	if (GetKeyboardTrigger(DIK_7))
	{
		AddItembox(g_Item[RANDOM_ITEM_2].TexNo);
	}
	if (GetKeyboardTrigger(DIK_N))
	{
		IncreaseItembox(1);
	}

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItembox(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_Box[i].use == FALSE)
			continue;
		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		// �}�g���N�X�ݒ�
		SetWorldViewProjection2D();

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �}�e���A���ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Box[i].TexNo]);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Box[i].pos.x;	// �\���ʒuX
		float py = g_Box[i].pos.y;			// �\���ʒuY
		float pw = g_Box[i].w;				// �\����
		float ph = g_Box[i].h;				// �\������

		float tw = g_Box[i].tw;		// �e�N�X�`���̕�
		float th = g_Box[i].th;		// �e�N�X�`���̍���
		float tx = g_Box[i].tx;		// �e�N�X�`���̍���X���W
		float ty = g_Box[i].ty;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_Box[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	//�I���{�b�N�X

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Selectbox.TexNo]);

	// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = g_Selectbox.pos.x;	// �\���ʒuX
	float py = g_Selectbox.pos.y;			// �\���ʒuY
	float pw = g_Selectbox.w;				// �\����
	float ph = g_Selectbox.h;				// �\������

	float tw = g_Selectbox.tw;		// �e�N�X�`���̕�
	float th = g_Selectbox.th;		// �e�N�X�`���̍���
	float tx = g_Selectbox.tx;		// �e�N�X�`���̍���X���W
	float ty = g_Selectbox.ty;		// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		g_Selectbox.color);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// �A�C�e���{�b�N�X�ɃA�C�e����ǉ�
//=============================================================================
void AddItembox(int TNum)
{
	for (int i = 0; i < g_items; i++)
	{
		if (g_Box[i].use == FALSE || g_Box[i].open == FALSE)
			continue;
		g_Box[i].TexNo = TNum;
		g_Box[i].open = FALSE;
		g_Box[i].color = { 1.0f,1.0f,1.0f,1.0f };
		break;
	}
	return;
}
//=============================================================================
// �A�C�e���{�b�N�X�ɋ󂢂Ă���Ƃ����ǉ�
//=============================================================================
void IncreaseItembox(int n)
{
	int m = 0;
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_Box[i].use == TRUE)
			continue;
		if (m < n)
		{
			UI_ELEMENT	*itembox = GetItemBox();
			itembox[i].use = TRUE;
			g_Box[i].use = TRUE;
			IncreaseItemmax();
			m++;
		}
	}
	return;
}


void MoveSelect()		//C��		A��
{
	if (IsButtonTriggered(0, BUTTON_C) && g_Select < g_items - 1)
	{
		g_Select++;
		g_Selectbox.pos.x = g_Box[g_Select].pos.x;
	}
	if (IsButtonTriggered(0, BUTTON_A) && g_Select > 0)
	{
		g_Select--;
		g_Selectbox.pos.x = g_Box[g_Select].pos.x;
	}
}

int	ReturnSelect()		//���I�����Ă���A�C�e����ID��߂�
{
	return g_Box[g_Select].TexNo;
}
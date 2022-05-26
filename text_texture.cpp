//=============================================================================
//
// �����e�N�X�`�����̕\������ [text_texture.cpp]
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
#include "text_texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(12)				// �e�N�X�`���̐�
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/hpbarred.png",
};
static BOOL						g_Load = FALSE;
static TEXT_TEXTURE				g_Texttex[TEXTURE_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTexttex(void)
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
		g_Texttex[i].use = FALSE;
		g_Texttex[i].TexNo = i;						//�e�N�X�`���ԍ�
		g_Texttex[i].w = 0;
		g_Texttex[i].h = 0;
		g_Texttex[i].tw = 1.0f;
		g_Texttex[i].th = 1.0f;
		g_Texttex[i].tx = 0.0f;
		g_Texttex[i].ty = 0.0f;		// ���ƍ����A�e�N�X�`�����W
		g_Texttex[i].pos = {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f};						// �|���S���̈ʒu
		g_Texttex[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };						//�F
		g_Texttex[i].time = 0;					
		g_Texttex[i].timeMax = 180;				
	}

	// ������
	g_Load = TRUE;
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitTexttex(void)
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
void UpdateTexttex(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texttex[i].use != TRUE)continue;

		const float fade = 0.04f;
		if (g_Texttex[i].color.w < 1.0f && g_Texttex[i].time < g_Texttex[i].timeMax)
		{//�\���J�n
			g_Texttex[i].color.w += fade;
		}
		else if (g_Texttex[i].color.w >= 1.0f && g_Texttex[i].time < g_Texttex[i].timeMax)
		{//�\�����������̂ł�������莞�Ԃ܂ňێ�
			g_Texttex[i].time++;
		}
		else if (g_Texttex[i].color.w > 0.0f && g_Texttex[i].time >= g_Texttex[i].timeMax)
		{//�����J�n
			g_Texttex[i].color.w -= fade;
		}
		else if (g_Texttex[i].color.w <= 0.0f && g_Texttex[i].time >= g_Texttex[i].timeMax)
		{//�����������߁A�S�ď�����
			g_Texttex[i].use = FALSE;
			g_Texttex[i].color.w = 0.0f;
			g_Texttex[i].time = 0;
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawTexttex(void)
{
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

	float px = 0.0f;	// �\���ʒuX
	float py = 0.0f;			// �\���ʒuY
	float pw = 0.0f;				// �\����
	float ph = 0.0f;				// �\������

	float tw = 0.0f;		// �e�N�X�`���̕�
	float th = 0.0f;		// �e�N�X�`���̍���
	float tx = 0.0f;		// �e�N�X�`���̍���X���W
	float ty = 0.0f;		// �e�N�X�`���̍���Y���W

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texttex[i].use != TRUE)continue;

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Texttex[i].TexNo]);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		px = g_Texttex[i].pos.x;			// �\���ʒuX
		py = g_Texttex[i].pos.y;			// �\���ʒuY
		pw = g_Texttex[i].w;				// �\����
		ph = g_Texttex[i].h;				// �\������

		tw = g_Texttex[i].tw;		// �e�N�X�`���̕�
		th = g_Texttex[i].th;		// �e�N�X�`���̍���
		tx = g_Texttex[i].tx;		// �e�N�X�`���̍���X���W
		ty = g_Texttex[i].ty;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_Texttex[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

void SetTexture(int i)
{
	if (g_Texttex[i].use != TRUE)
		g_Texttex[i].use = TRUE;
}
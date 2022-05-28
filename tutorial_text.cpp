//=============================================================================
//
// �����e�N�X�`�����̕\������ [tutorial_text.cpp]
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
#include "tutorial_text.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(2)				// �e�N�X�`���̐�
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial_lighter.png",
	"data/TEXTURE/tutorial_matchi.png",
};
static BOOL						g_Load = FALSE;
static TEXT_TEXTURE				g_TutorialTex[TEXTURE_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorialTex(void)
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

	const float mag = 0.25f;
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_TutorialTex[i].use = FALSE;
		g_TutorialTex[i].TexNo = i;						//�e�N�X�`���ԍ�
		g_TutorialTex[i].w = SCREEN_WIDTH * 0.8f;
		g_TutorialTex[i].h = SCREEN_HEIGHT * 0.8f;
		g_TutorialTex[i].tw = 1.0f;
		g_TutorialTex[i].th = 1.0f;
		g_TutorialTex[i].tx = 0.0f;
		g_TutorialTex[i].ty = 0.0f;		// ���ƍ����A�e�N�X�`�����W
		g_TutorialTex[i].pos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };						// �|���S���̈ʒu
		g_TutorialTex[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };						//�F
		g_TutorialTex[i].time = 0;
		g_TutorialTex[i].timeMax = 210;

		switch (i)
		{
		case TUTORIAL_LIGHTER:
			break;
		case TUTORIAL_MATCHI:
			break;
		}
	}

	// ������
	g_Load = TRUE;
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitTutorialTex(void)
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
void UpdateTutorialTex(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_TutorialTex[i].use != TRUE)continue;

		const float fade = 1.0f;
		if (g_TutorialTex[i].color.w < 1.0f)
		{//�\���J�n
			g_TutorialTex[i].color.w += fade;
		}
		else if (g_TutorialTex[i].color.w >= 1.0f && (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN)))
		{//�����J�n
			g_TutorialTex[i].use = FALSE;
			g_TutorialTex[i].color.w = 0.0f;
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorialTex(void)
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
		if (g_TutorialTex[i].use != TRUE)continue;

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TutorialTex[i].TexNo]);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		px = g_TutorialTex[i].pos.x;			// �\���ʒuX
		py = g_TutorialTex[i].pos.y;			// �\���ʒuY
		pw = g_TutorialTex[i].w;				// �\����
		ph = g_TutorialTex[i].h;				// �\������

		tw = g_TutorialTex[i].tw;		// �e�N�X�`���̕�
		th = g_TutorialTex[i].th;		// �e�N�X�`���̍���
		tx = g_TutorialTex[i].tx;		// �e�N�X�`���̍���X���W
		ty = g_TutorialTex[i].ty;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_TutorialTex[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}

void SetTutorialTexture(int i)
{
	if (g_TutorialTex[i].time == 1)return;

	for (int k = 0; k < TEXTURE_MAX; k++)
	{
		if (g_TutorialTex[k].use)
		{
			g_TutorialTex[k].use = FALSE;
			g_TutorialTex[k].color.w = 0.0f;
			g_TutorialTex[k].time = 0;
			break;
		}
	}

	if (g_TutorialTex[i].use != TRUE)
	{
		StopSound();
		g_TutorialTex[i].use = TRUE;
		g_TutorialTex[i].color.w = 0.0f;
		g_TutorialTex[i].time = 0;
	}
}

//�`���[�g�A���e�N�X�`�����\�����Ȃ̂�
BOOL GetTutorialTex(void)
{
	BOOL ans = FALSE;
	for (int k = 0; k < TEXTURE_MAX; k++)
	{
		if (g_TutorialTex[k].use)
		{
			ans = TRUE;	//1�ł��g�p���Ȃ�TRUE��Ԃ�
			break;
		}
	}
	return ans;
}
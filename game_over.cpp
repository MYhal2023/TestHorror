//=============================================================================
//
// �Q�[���I�[�o�[��ʏ��� [game_over.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "game_over.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 
#define	ENEMY_TEXMAG	(0.05f)							// ���摜�ɑ΂���{��
#define	ENEMY_WIDTH		(650.0f * ENEMY_TEXMAG)			// ���_�T�C�Y
#define	ENEMY_HEIGHT	(812.0f * ENEMY_TEXMAG)			// ���_�T�C�Y
#define ENEMY_SPOWN	(330)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/gameover.png",
	"data/TEXTURE/enemy.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static int						g_Event;
static float					g_w, g_h;					// ���ƍ���
static float					g_Ew, g_Eh;					// ���ƍ���
static float					g_Tw, g_Th;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static XMFLOAT3					g_EPos;						// �G�l�~�[�|���S���̍��W
static int						g_Count;
static int						g_TexNo;					// �e�N�X�`���ԍ�
static XMFLOAT4					g_Color[TEXTURE_MAX];
static BOOL						g_Load = FALSE;
static float					alpha = 0.0f;
static float					Ealpha = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame_over(void)
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


	// �ϐ��̏�����
	g_Use = FALSE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_Ew = ENEMY_WIDTH;
	g_Eh = ENEMY_HEIGHT;
	g_Tw = 947.0f * 0.5f;
	g_Th = 400.0f * 0.5f;
	g_EPos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	// BGM�Đ�
	//PlaySound(SOUND_LABEL_BGM_sample000);
	g_Event = 0;
	g_Count = 0;
	alpha = 0.0f;
	Ealpha = 0.0f;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame_over(void)
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
void UpdateGame_over(void)
{
	if (!g_Use)return;

	switch (g_Event)
	{
	case Start_BGM:	//BGM���܂��炷
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GameOver);
		g_Event++;
		break;
	case Enemy_Dash:	//�K�؂ȃ^�C�~���O�ŃG�l�~�[���߂Â��Ă���
		g_Count++;
		if (g_Count >= ENEMY_SPOWN)
		{
			g_Ew *= 1.1f;
			g_Eh *= 1.1f;
			Ealpha = 1.0f;
		}

		if (g_Count >= ENEMY_SPOWN + 30)
		{
			g_Event++;
			g_Count = 0;
		}
		break;
	case Dead:	//�G�l�~�[�������A�����𕂂��яオ�点��B
		if (g_Count == 0)
		{
			Ealpha = 0.0f;
		}
		else if (g_Count >= 60 && alpha < 1.0f)
		{
			alpha += 0.02f;
		}
		else if (g_Count == 220)
		{
			SetFade(FADE_OUT, MODE_GAME);	//�������o���烂�[�h���Z�b�g
		}
		g_Count++;
		break;
	}



#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame_over(void)
{
	if (!g_Use)return;

	for (int i = 0; i < TEXTURE_MAX; i++)
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

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		switch (i)	//�����Ńe�N�X�`�����ɍ��W�Ƒ傫�����w��
		{
		case GAME_OVER_BG:
			g_w = TEXTURE_WIDTH;
			g_h = TEXTURE_HEIGHT;
			g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
			g_Color[i] = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case GAME_OVER_TEXT:
			g_w = g_Tw;
			g_h = g_Th;
			g_Pos = XMFLOAT3(TEXTURE_WIDTH / 2, TEXTURE_HEIGHT * 0.5f, 0.0f);
			g_Color[i] = XMFLOAT4{ 1.0f, 1.0f, 1.0f, alpha };
			break;
		case GAME_OVER_ENEMY:
			g_w = g_Ew;
			g_h = g_Eh;
			g_Pos = XMFLOAT3(TEXTURE_WIDTH / 2, TEXTURE_HEIGHT * 0.5f, 0.0f);
			g_Color[i] = XMFLOAT4{ 1.0f, 1.0f, 1.0f,Ealpha};
			break;
		}

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f,
			g_Color[i]);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}

void SetGameOver(void)
{
	g_Use = TRUE;
}
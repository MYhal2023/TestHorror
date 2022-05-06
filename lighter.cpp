//=============================================================================
//
// ���C�^�[���� [lighter.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "interface.h"
#include "lighter.h"
#include "sprite.h"
#include "sound.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(160)	// �e�N�X�`���T�C�Y
#define TEXTURE_HEIGHT				(200)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

#define SCREEN_WIDTH	(960)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(540)			// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define LIGHTER_POS_X	(SCREEN_CENTER_X + 250)
#define LIGHTER_POS_Y	(SCREEN_CENTER_Y + 100)



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/lighter.png",
};

static int						g_TexNo;					// �e�N�X�`���ԍ�
static LIGHTER					g_Lighter;					

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitLighter(void)
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


	// ������
	g_Lighter.w     = TEXTURE_WIDTH;
	g_Lighter.h     = TEXTURE_HEIGHT;
	g_Lighter.pos   = { LIGHTER_POS_X, LIGHTER_POS_Y, 0.0f };
	g_TexNo = 0;

	g_Lighter.use = TRUE;
	g_Lighter.oil = LIGHTER_OIL_MAX;
	g_Lighter.out = FALSE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLighter(void)
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
void UpdateLighter(void)
{
#ifdef _DEBUG	// �f�o�b�O����\������

	if (GetKeyboardTrigger(DIK_L))
	{
		SetLighterOn(TRUE);
	}
	if (GetKeyboardTrigger(DIK_J))
	{
		SetLighterOff();
	}
	if (GetKeyboardTrigger(DIK_I))
	{
		AddOil(10.0f);
	}
#endif

	CheckOil();
	if (!g_Lighter.use || !g_Lighter.out)
		return;
	ReduceOil();

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	PrintDebugProc("�c��I�C���ʁF%f / 100", g_Lighter.oil);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLighter(void)
{
	if (!g_Lighter.out)
		return;

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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = g_Lighter.pos.x;	// �X�R�A�̕\���ʒuX
	float py = g_Lighter.pos.y;			// �X�R�A�̕\���ʒuY
	float pw = g_Lighter.w;				// �X�R�A�̕\����
	float ph = g_Lighter.h;				// �X�R�A�̕\������

	float tw = 1.0f;		// �e�N�X�`���̕�
	float th = 1.0f;		// �e�N�X�`���̍���
	float tx = 0.0f;			// �e�N�X�`���̍���X���W
	float ty = 0.0f;			// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// ���C�^�[ON
//=============================================================================

void SetLighterOn(BOOL flag)
{
	//CheckOil();
	if (!g_Lighter.use)		return;
	PlaySound(SOUND_LABEL_SE_LighterON);
	g_Lighter.out = flag;
}

//=============================================================================
// ���C�^�[OFF
//=============================================================================
void SetLighterOff()
{
	if (!g_Lighter.use)	return;
	g_Lighter.out = FALSE;
}

//=============================================================================
// �I�C���ʂ�ǉ�
//=============================================================================
void AddOil(float n)
{
	if (g_Lighter.oil + n > LIGHTER_OIL_MAX)
	{
		g_Lighter.oil = LIGHTER_OIL_MAX;
	}
	else
	{
		g_Lighter.oil += n;

	}
	if (g_Lighter.use == FALSE)
	{
		g_Lighter.use = TRUE;
	}
}

//=============================================================================
// �I�C���ʃ`�F�b�N
//=============================================================================
void CheckOil()
{
	if (g_Lighter.oil >= LIGHTER_OIL_MIN || g_Lighter.use == FALSE)
		return;
	SetLighterOff();
	g_Lighter.use = FALSE;
}

//=============================================================================
// �I�C���ʌ��炷
//=============================================================================
void ReduceOil()
{
	g_Lighter.oil -= LIGHTER_OIL_REDUCE;
}

LIGHTER *GetLighter()
{
	return &g_Lighter;
}
//=============================================================================
//
// ���̑�UI�\������ [interface.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "interface.h"
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
#define HP_TEXTURE_WIDTH				(800/5)	// �e�N�X�`���T�C�Y
#define HP_TEXTURE_HEIGHT				(200/5)	// 
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�
#define	HP_X						(80)			// �e�N�X�`�����W
#define HP_Y						(200)
#define OIL_X						(SCREEN_WIDTH-100)	// �e�N�X�`�����W
#define OIL_Y						(SCREEN_HEIGHT-150)					// 
#define MATCH_X						(50)
#define MATCH_Y						(SCREEN_HEIGHT-80)
#define MATCH_TEXTURE_WIDTH			(40*2)
#define MATCH_TEXTURE_HEIGHT		(20*2)
#define MATCH_NUM_TEXTURE_WIDTH		(25)	// �e�N�X�`���T�C�Y
#define MATCH_NUM_TEXTURE_HEIGHT	(50)	// 

#define MATCH_DIGIT			(2)			// ����

#define HUNDRED						(100.0f)				//�t���[�g�^100
#define	DEVIATION					(0.8f)					//���C�^�[�̃Q�[�W�̈ړ���������邽��


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
	"data/TEXTURE/hpbar.png",
	"data/TEXTURE/oilbarred.png",
	"data/TEXTURE/oilbar.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/match_ui.png",
};


UI_ELEMENT	g_UI[TEXTURE_MAX];

static BOOL						g_Load = FALSE;
static int						g_match = 0;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitInterface(void)
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
		g_UI[i].use = TRUE;
		g_UI[i].TexNo = i;
		g_UI[i].tw = 1.0f;		// �e�N�X�`���̕�
		g_UI[i].th = 1.0f;		// �e�N�X�`���̍���
		g_UI[i].tx = 0.0f;			// �e�N�X�`���̍���X���W
		g_UI[i].ty = 0.0f;			// �e�N�X�`���̍���Y���W
	}
	g_UI[HP_BAR].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_BAR].w = HP_TEXTURE_WIDTH;
	g_UI[HP_BAR].h = HP_TEXTURE_HEIGHT;

	g_UI[HP_RED].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_RED].w = HP_TEXTURE_WIDTH;
	g_UI[HP_RED].h = HP_TEXTURE_HEIGHT;

	g_UI[OIL_BAR].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_BAR].w = HP_TEXTURE_HEIGHT;
	g_UI[OIL_BAR].h = HP_TEXTURE_WIDTH;

	g_UI[OIL_RED].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_RED].w = HP_TEXTURE_HEIGHT;
	g_UI[OIL_RED].h = HP_TEXTURE_WIDTH;

	g_UI[MATCH_NUM].pos = { MATCH_X+ MATCH_TEXTURE_WIDTH, MATCH_Y, 0.0f };
	g_UI[MATCH_NUM].w = MATCH_NUM_TEXTURE_WIDTH;
	g_UI[MATCH_NUM].h = MATCH_NUM_TEXTURE_HEIGHT;

	g_UI[MATCH_PIC].pos = { MATCH_X, MATCH_Y, 0.0f };
	g_UI[MATCH_PIC].w = MATCH_TEXTURE_WIDTH;
	g_UI[MATCH_PIC].h = MATCH_TEXTURE_HEIGHT;
	// �v���C���[�̏�����



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitInterface(void)
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
void UpdateInterface(void)
{
	PLAYER player = *GetPlayer();
	LIGHTER lighter = *GetLighter();
	MATCH match = *GetMatch();

	g_UI[OIL_RED].h = HP_TEXTURE_WIDTH * (lighter.oil/HUNDRED);				//�I�C�������Ȃ��Ȃ��āA�Q�[�W���������Ȃ�
	g_UI[OIL_RED].pos.y = OIL_Y + (HUNDRED-lighter.oil)*DEVIATION;			//����������΁A���Ɉړ�����
	g_match = match.num;													//�}�b�`�̐�����Ⴄ


#ifdef _DEBUG	// �f�o�b�O����\������

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawInterface(void)
{
	for(int i=0;i<TEXTURE_MAX;i++)
	{
		if (i == MATCH_NUM)																//�}�b�`�̐�����ʂň��
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].TexNo]);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_UI[i].pos.x;	// �\���ʒuX
		float py = g_UI[i].pos.y;			// �\���ʒuY
		float pw = g_UI[i].w;				// �\����
		float ph = g_UI[i].h;				// �\������

		float tw = g_UI[i].tw;		// �e�N�X�`���̕�
		float th = g_UI[i].th;		// �e�N�X�`���̍���
		float tx = g_UI[i].tx;		// �e�N�X�`���̍���X���W
		float ty = g_UI[i].ty;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	//�}�b�`�̐���

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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[MATCH_NUM].TexNo]);

	// ��������������
	int number = g_match;
	for (int i = 0; i < MATCH_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(number % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_UI[MATCH_NUM].pos.x - g_UI[MATCH_NUM].w * i;	// �X�R�A�̕\���ʒuX
		float py = g_UI[MATCH_NUM].pos.y;			// �X�R�A�̕\���ʒuY
		float pw = g_UI[MATCH_NUM].w;				// �X�R�A�̕\����
		float ph = g_UI[MATCH_NUM].h;				// �X�R�A�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = x * tw;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		number /= 10;
	}
}

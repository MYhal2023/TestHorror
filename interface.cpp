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
#define TEXTURE_MAX					(12)				// �e�N�X�`���̐�
#define WIDTH						(1920.0f)
#define HEIGHT						(1080.0f)

#define PLAYER_HURT_FRAME			(120)			// �v���C���[���G�t���[��


#define HP_TEXTURE_WIDTH			(3200.0f * 0.2f)	// �e�N�X�`���T�C�Y
#define HP_TEXTURE_HEIGHT			(80.0f * 0.2f)	// 
#define	HP_X						(80.0f)			// �e�N�X�`�����W
#define HP_Y						(50.0f)

#define OIL_TEXTURE_WIDTH			(100.0f * 0.2f)	// �e�N�X�`���T�C�Y
#define OIL_TEXTURE_HEIGHT			(600.0f * 0.2f)	// 
#define OIL_X						(WIDTH-50.0f)	// �e�N�X�`�����W
#define OIL_Y						(HEIGHT-100.0f)					// 

#define STAM_TEXTURE_WIDTH			(WIDTH * 0.8f)	// �e�N�X�`���T�C�Y
#define STAM_TEXTURE_HEIGHT			(7.0f)	// 
#define	STAM_X						(WIDTH * 0.5f)			// �e�N�X�`�����W
#define STAM_Y						(21.0f)

#define MIND_TEXTURE_WIDTH			(700)	// �e�N�X�`���T�C�Y
#define MIND_TEXTURE_HEIGHT			(40.0f)	// 
#define	MIND_X						(HP_X)			// �e�N�X�`�����W
#define MIND_Y						(HP_Y+40.0f)

#define ITEM_BOX_TEXTURE_WIDTH		(80.0f)	// �e�N�X�`���T�C�Y
#define ITEM_BOX_TEXTURE_HEIGHT		(70.0f)	// 
#define	ITEM_BOX_X					(864.0f)		// �e�N�X�`�����W
#define ITEM_BOX_Y					(1000.0f)

#define MATCH_X						(1700.0f)
#define MATCH_Y						(980.0f)

#define MATCH_TEXTURE_WIDTH			(40.0f * 2.0f)
#define MATCH_TEXTURE_HEIGHT		(20.0f * 2.0f)
#define MATCH_NUM_TEXTURE_WIDTH		(25.0f)	// �e�N�X�`���T�C�Y
#define MATCH_NUM_TEXTURE_HEIGHT	(50.0f)	// 

#define MATCH_DIGIT					(2)			// ����

#define HUNDRED						(100.0f)				//�t���[�g�^100
#define	DEVIATION_HP				(0.8f)					//�Q�[�W�̈ړ���������邽��
#define DEVIATION_OIL				(0.6f)
#define DEVIATION_STAM				(0.8f)
#define DEVIATION_MIND				(0.6f)

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
	"data/TEXTURE/hpbarred.png",
	"data/TEXTURE/hpbarred.png",
	"data/TEXTURE/hpbar.png",
	"data/TEXTURE/stamina1.png",				
	"data/TEXTURE/oilbarred.png",
	"data/TEXTURE/oilbar.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/match_ui.png",
	"data/TEXTURE/mind.png",
	"data/TEXTURE/hurt.png",
	"data/TEXTURE/lowmind.png",
	"data/TEXTURE/itembox.png",
};


static UI_ELEMENT	g_UI[TEXTURE_MAX - 1];
static UI_ELEMENT	g_ItemBox[ITEM_MAX];

static BOOL						g_Load = FALSE;
static int						g_match = 0;
static int						g_time = 0;
static int						g_hurt_time = 0;
static int						g_ItemMax = ITEM_MIN;			//���݃A�C�e�������Ă���E


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

	for (int i = 0; i < TEXTURE_MAX - 1; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].TexNo = i;
		g_UI[i].tw = 1.0f;		// �e�N�X�`���̕�
		g_UI[i].th = 1.0f;		// �e�N�X�`���̍���
		g_UI[i].tx = 0.0f;			// �e�N�X�`���̍���X���W
		g_UI[i].ty = 0.0f;			// �e�N�X�`���̍���Y���W
		g_UI[i].color = { 1.0f,1.0f,1.0f,1.0f };
		g_UI[i].pos = { 0.0f, 0.0f, 0.0f };
	}
	//HP�Q�[�W
	g_UI[HP_BAR].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_BAR].w = HP_TEXTURE_WIDTH;
	g_UI[HP_BAR].h = HP_TEXTURE_HEIGHT;

	//HP�@�w�i
	g_UI[HP_RED_BG].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_RED_BG].w = HP_TEXTURE_WIDTH;
	g_UI[HP_RED_BG].h = HP_TEXTURE_HEIGHT;
	g_UI[HP_RED_BG].color = { 15.0f,0.7f,1.0f,1.0f };

	//HP�@�F
	g_UI[HP_RED].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_RED].w = HP_TEXTURE_WIDTH;
	g_UI[HP_RED].h = HP_TEXTURE_HEIGHT;
	g_UI[HP_RED].color = { 0.0f,5.0f,2.0f,1.0f };

	//�I�C���Q�[�W
	g_UI[OIL_BAR].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_BAR].w = OIL_TEXTURE_WIDTH;
	g_UI[OIL_BAR].h = OIL_TEXTURE_HEIGHT;

	//�I�C���F
	g_UI[OIL_RED].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_RED].w = OIL_TEXTURE_WIDTH;
	g_UI[OIL_RED].h = OIL_TEXTURE_HEIGHT;

	//�}�b�`����
	g_UI[MATCH_NUM].pos = { MATCH_X + MATCH_TEXTURE_WIDTH, MATCH_Y, 0.0f };
	g_UI[MATCH_NUM].w = MATCH_NUM_TEXTURE_WIDTH;
	g_UI[MATCH_NUM].h = MATCH_NUM_TEXTURE_HEIGHT;

	//�}�b�`UI
	g_UI[MATCH_PIC].pos = { MATCH_X, MATCH_Y, 0.0f };
	g_UI[MATCH_PIC].w = MATCH_TEXTURE_WIDTH;
	g_UI[MATCH_PIC].h = MATCH_TEXTURE_HEIGHT;

	//Sanity�Q�[�W
	g_UI[MIND].pos = { MIND_X, MIND_Y, 0.0f };
	g_UI[MIND].w = MIND_TEXTURE_WIDTH;
	g_UI[MIND].h = MIND_TEXTURE_HEIGHT;

	//�ɂ݃G�t�F�N�g
	g_UI[HURT].use = FALSE;
	g_UI[HURT].pos = { WIDTH * 0.5f, HEIGHT * 0.5f, 0.0f };
	g_UI[HURT].w = WIDTH;
	g_UI[HURT].h = HEIGHT;
	g_UI[HURT].color = { 0.3f,0.0f,0.0f,0.0f };

	//Sanity�G�t�F�N�g
	g_UI[INSANE].use = TRUE;
	g_UI[INSANE].pos = { WIDTH * 0.5f, HEIGHT * 0.5f, 0.0f };
	g_UI[INSANE].w = WIDTH;
	g_UI[INSANE].h = HEIGHT;
	g_UI[INSANE].color = { 0.1f,0.0f,0.0f,0.0f };

	//�X�^�~�i�o�[
	g_UI[STAM_BAR].pos = { STAM_X , STAM_Y , 0.0f };
	g_UI[STAM_BAR].w = STAM_TEXTURE_WIDTH;
	g_UI[STAM_BAR].h = STAM_TEXTURE_HEIGHT;
	g_UI[STAM_BAR].color = { 0.0f,1.0f,0.0f,1.0f };

	const float mag = 0.15f;
	g_UI[MIND].pos = { WIDTH * 0.10f, HEIGHT * 0.88f, 0.0f };
	g_UI[MIND].w = 1300 * mag;
	g_UI[MIND].h = 1300 * mag;
	g_UI[MIND].color = { 1.0f,1.0f,1.0f,1.0f };

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (i >= g_ItemMax)
		{
			g_ItemBox[i].use = FALSE;
		}
		else
		{
			g_ItemBox[i].use = TRUE;
		}
		g_ItemBox[i].TexNo = ITEM_BOX;
		g_ItemBox[i].tw = 1.0f;		// �e�N�X�`���̕�
		g_ItemBox[i].th = 1.0f;		// �e�N�X�`���̍���
		g_ItemBox[i].tx = 0.0f;			// �e�N�X�`���̍���X���W
		g_ItemBox[i].ty = 0.0f;			// �e�N�X�`���̍���Y���W
		float x = ITEM_BOX_X + (ITEM_BOX_TEXTURE_WIDTH * (float)(i));
		float y = ITEM_BOX_Y;
		g_ItemBox[i].pos = { x , y, 0.0f };
		g_ItemBox[i].w = ITEM_BOX_TEXTURE_WIDTH;
		g_ItemBox[i].h = ITEM_BOX_TEXTURE_HEIGHT;
		g_ItemBox[i].color = { 1.0f,1.0f,1.0f,1.0f };
	}


	// ������
	g_ItemMax = ITEM_MIN;
	g_match = 0;
	g_time = 0;
	g_hurt_time = 0;


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

	//HP 
	g_UI[HP_RED].w = (HP_TEXTURE_WIDTH * ((float)(player.life) / HUNDRED));
	g_UI[HP_RED].pos.x = (HP_X - (HUNDRED - (float)(player.life))*DEVIATION_HP);

	//�X�^�~�i
	g_UI[STAM_BAR].w = STAM_TEXTURE_WIDTH * ((float)(player.stamina) / HUNDRED);
	g_UI[STAM_BAR].pos.x = STAM_X - (HUNDRED - ((float)player.stamina)) * DEVIATION_STAM;
	g_UI[STAM_BAR].color = { 1.0f - ((float)player.stamina) / HUNDRED , (float)(player.stamina) / HUNDRED , 0.0f,1.0f };

	//���C�^�[
	g_UI[OIL_RED].h = OIL_TEXTURE_HEIGHT * (lighter.oil / HUNDRED);				//�I�C�������Ȃ��Ȃ��āA�Q�[�W���������Ȃ�
	g_UI[OIL_RED].pos.y = OIL_Y + (HUNDRED - lighter.oil)*DEVIATION_OIL;			//����������΁A���Ɉړ�����

	const int buffer = 25;
	g_UI[MIND].color.y = FloatClamp((((float)((player.sanity - 20) / (float)(player.sanityMax)))), 0.0f, 1.0f);
	g_UI[MIND].color.z = FloatClamp((((float)((player.sanity - 20) / (float)(player.sanityMax)))), 0.0f, 1.0f);
	//Sanity �G�t�F�N�g
	SanityCheck(player.sanity);


	g_match = match.num;													//�}�b�`�̐�����Ⴄ

	GaugeAnimation();
	HurtAnimation();

#ifdef _DEBUG	// �f�o�b�O����\������
	if (GetKeyboardTrigger(DIK_K))
	{
		for (int i = 0; i < ITEM_MAX; i++)
		{
			if (g_ItemBox[i].use == FALSE)
			{
				g_ItemBox[i].use = TRUE;
				break;
			}
		}
	}
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawInterface(void)
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

		//�A�C�e���{�b�N�X
	for (int i = 0; i < g_ItemMax; i++)
	{

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_ItemBox[i].pos.x;			// �\���ʒuX
		float py = g_ItemBox[i].pos.y;			// �\���ʒuY
		float pw = g_ItemBox[i].w;				// �\����
		float ph = g_ItemBox[i].h;				// �\������

		float tw = g_ItemBox[i].tw;		// �e�N�X�`���̕�
		float th = g_ItemBox[i].th;		// �e�N�X�`���̍���
		float tx = g_ItemBox[i].tx;		// �e�N�X�`���̍���X���W
		float ty = g_ItemBox[i].ty;		// �e�N�X�`���̍���Y���W

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_ItemBox[i].TexNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_ItemBox[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	for (int i = 0; i < TEXTURE_MAX - 1; i++)
	{
		if (i == HP_BAR || i == HP_RED_BG || i == HP_RED)continue;
		if (i == MATCH_NUM || i == 0)																//�}�b�`�̐�����ʂň��
			continue;
		if (g_UI[i].use == FALSE)
			continue;
		if (i == HURT || i == INSANE)
		{
			SetBlendState(BLEND_MODE_ADD);
		}

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].TexNo]);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		px = g_UI[i].pos.x;			// �\���ʒuX
		py = g_UI[i].pos.y;			// �\���ʒuY
		pw = g_UI[i].w;				// �\����
		ph = g_UI[i].h;				// �\������

		tw = g_UI[i].tw;		// �e�N�X�`���̕�
		th = g_UI[i].th;		// �e�N�X�`���̍���
		tx = g_UI[i].tx;		// �e�N�X�`���̍���X���W
		ty = g_UI[i].ty;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_UI[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		if (i == HURT || i== INSANE)
		{
			SetBlendState(BLEND_MODE_NONE);
		}
	}

	//�}�b�`�̐���
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

//=============================================================================
// �Q�[�W���炷�A�j���[�V��������
//=============================================================================
void GaugeAnimation()
{
	if (g_UI[HP_RED].w == g_UI[HP_RED_BG].w)
		return;

	if (g_UI[HP_RED].w > g_UI[HP_RED_BG].w)
	{
		g_UI[HP_RED_BG].w = g_UI[HP_RED].w;
		g_UI[HP_RED_BG].pos.x = g_UI[HP_RED].pos.x;
		g_time = 0;
		return;
	}
	if (g_time > WAIT_TIME)
	{
		g_UI[HP_RED_BG].w -= 1.5f;
		g_UI[HP_RED_BG].pos.x -= 1.5f;
	}
	else
	{
		g_time++;
	}

	return;
}


//=============================================================================
// �A�C�e���{�b�N�X��߂�
//=============================================================================
UI_ELEMENT *GetItemBox()
{
	return &g_ItemBox[0];
}
//=============================================================================
// �����_�ŃA�C�e���{�b�N�X�̍ő�l
//=============================================================================
int GetItemMax()
{
	return g_ItemMax;
}
//=============================================================================
// �A�C�e���{�b�N�X�̍ő�l���{�P����
//=============================================================================
void IncreaseItemmax()
{
	if (g_ItemMax >= ITEM_MAX)
		return;
	g_ItemMax++;
}

//=============================================================================
// �_���[�W���󂯂�ƃA�j���[�V������ON�ɂ���
//=============================================================================
void HurtAnimationOn()
{
	g_UI[HURT].use = TRUE;
}

//=============================================================================
// �_���[�W���󂯂�Ƃ̃A�j���[�V����
//=============================================================================
void HurtAnimation()
{
	if (g_UI[HURT].use == FALSE)
		return;
	if (g_hurt_time >= PLAYER_HURT_FRAME)
	{
		g_hurt_time = 0;
		g_UI[HURT].use = FALSE;
		g_UI[HURT].color.w = 0.0f;
		return;
	}
	g_hurt_time++;
	if (g_UI[HURT].color.w < 1.0f)
	{
		g_UI[HURT].color.w += 0.08f;
	}
	else
	{
		g_UI[HURT].color.w -= 0.08f;
	}
	return;
}

//Sanity �G�t�F�N�g
void SanityCheck(int sanity)
{
	if (sanity > 75) { g_UI[INSANE].color.w = 0.0f; };
	if (sanity > 50 && sanity < 75) { g_UI[INSANE].color.w = 0.2f; };
	if (sanity > 25 && sanity < 50) { g_UI[INSANE].color.w = 0.4f; };
	if (sanity < 25) { g_UI[INSANE].color.w = 0.5f; };
	return;
}
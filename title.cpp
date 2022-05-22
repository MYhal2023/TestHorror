//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 
#define TEXTURE_HEIGHT_PER			(0.15f)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TITLE_TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TITLE_TEXTURE_MAX] = {
	"data/TEXTURE/result001.jpg",
	"data/TEXTURE/cursol.png",
	"data/TEXTURE/endgame.png",
	"data/TEXTURE/newgame.png",
	"data/TEXTURE/loadgame.png",
};

static TITLE					g_Title[TITLE_TEXTURE_MAX];
static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
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
	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
	{
		g_Title[i].use = TRUE;
		//g_Title[i].w = TEXTURE_WIDTH;
		//g_Title[i].h = TEXTURE_HEIGHT;
		//g_Title[i].pos = XMFLOAT3(g_Title[i].w / 2, g_Title[i].h / 2, 0.0f);
		g_Title[i].texNo = i;
		const float mag = 0.1f;
		const float Cmag = 0.04f;
		switch (i)	//�����Ńe�N�X�`�����ɍ��W�Ƒ傫�����w��
		{
		case TITLE_BG:
			g_Title[i].w = TEXTURE_WIDTH;
			g_Title[i].h = TEXTURE_HEIGHT;
			g_Title[i].pos.x = g_Title[i].w / 2;
			g_Title[i].pos.y = g_Title[i].h / 2;
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_CURSOL:
			g_Title[i].w = 1600.0f * Cmag;
			g_Title[i].h = 1200.0f * Cmag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.35f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * 0.6f + TEXTURE_HEIGHT_PER;
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_NEW:
			g_Title[i].w = 1936.0f * mag;
			g_Title[i].h = 412.0f * mag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.5f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * 0.6f;
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_LOAD:
			g_Title[i].w = 1996.0f * mag;
			g_Title[i].h = 412.0f * mag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.5f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * (0.6f+ TEXTURE_HEIGHT_PER);
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_END:
			g_Title[i].w = 1789.0f * mag;
			g_Title[i].h = 412.0f * mag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.5f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * (0.6f + TEXTURE_HEIGHT_PER * 2.0f);
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		}

	}

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_Normal);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
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
void UpdateTitle(void)
{
	if ((GetKeyboardTrigger(DIK_RETURN) ||
		IsButtonTriggered(0, BUTTON_START) ||
		IsButtonTriggered(0, BUTTON_B))
		&& g_Title[TITLE_CURSOL].pos.y - 1.0f <= g_Title[TITLE_NEW].pos.y)
	{// Enter��������A�X�e�[�W��؂�ւ���
		PlaySound(SOUND_LABEL_SE_Cursol_On);
		SetFade(FADE_OUT, MODE_GAME);
	}
	else if((GetKeyboardTrigger(DIK_RETURN) ||
		IsButtonTriggered(0, BUTTON_START) ||
		IsButtonTriggered(0, BUTTON_B))
		&& g_Title[TITLE_CURSOL].pos.y >= g_Title[TITLE_END].pos.y)
	{
		PlaySound(SOUND_LABEL_SE_Cursol_On);
		SetFade(FADE_OUT, MODE_MAX);
	}
	else if((GetKeyboardTrigger(DIK_RETURN) ||
		IsButtonTriggered(0, BUTTON_START) ||
		IsButtonTriggered(0, BUTTON_B)))
	{
		PlaySound(SOUND_LABEL_SE_Door_Close);
	}

	if ((GetKeyboardTrigger(DIK_UP) ||
		IsButtonTriggered(0, BUTTON_UP))
		&& g_Title[TITLE_CURSOL].pos.y > g_Title[TITLE_NEW].pos.y + 1.0f)//float�l����ɂ��덷�������邽�߁A�]���݂���
	{
		PlaySound(SOUND_LABEL_SE_Cursol_Move);
		g_Title[TITLE_CURSOL].pos.y -= 540.0f * TEXTURE_HEIGHT_PER;
	}
	else if ((GetKeyboardTrigger(DIK_DOWN) ||
		IsButtonTriggered(0, BUTTON_DOWN))
		&& g_Title[TITLE_CURSOL].pos.y < g_Title[TITLE_END].pos.y)
	{
		PlaySound(SOUND_LABEL_SE_Cursol_Move);
		g_Title[TITLE_CURSOL].pos.y += 540.0f * TEXTURE_HEIGHT_PER;
	}



#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
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

	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
	{
		if (!g_Title[i].use)continue;
		float px = 0.0f;
		float py = 0.0f;
		float tw = 0.0f;
		float th = 0.0f;
		tw = g_Title[i].w;
		th = g_Title[i].h;
		px = g_Title[i].pos.x;
		py = g_Title[i].pos.y;

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[i].texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, tw, th, 0.0f, 0.0f, 1.0f, 1.0f, g_Title[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

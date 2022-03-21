//=============================================================================
//
// �}�b�`���� [match.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "input.h"
#include "match.h"
#include "debugproc.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// �e�N�X�`���T�C�Y
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define MATCH_POS_X					(600)		// �}�b�`��\������ꏊ(X��)
#define DEFAULTMATCH_POS_Y			(600.0f)	// �ʏ펞�̃}�b�`�̏ꏊ(Y��)
#define STANDBYMATCH_POS_Y			(500.0f)	// �X�^���o�C���̃}�b�`�̏ꏊ(Y��)
#define STANDBYTIME					(10)		// �}�b�`���\����܂ł̎���
#define STANDBY_MOVE_FRAME			((STANDBYMATCH_POS_Y-DEFAULTMATCH_POS_Y)/STANDBYTIME)//�\���鎞��1�t���[��������̈ړ���
#define MATCH_NUM					(10)			// �}�b�`�̖{��(����)
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
	"data/TEXTURE/fire.png",

};




static MATCH						g_Match;

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMatch(void)
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


	// �}�b�`�̏�����
	g_Match.Use   = TRUE;
	g_Match.w     = TEXTURE_WIDTH;
	g_Match.h     = TEXTURE_HEIGHT;
	g_Match.Pos   = { MATCH_POS_X, DEFAULTMATCH_POS_Y, 0.0f };
	g_Match.TexNo = 0;
	g_Match.StandbyTime = 0;
	g_Match.num = MATCH_NUM;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMatch(void)
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
void UpdateMatch(void)
{

	StandbyMatch();


#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	PrintDebugProc("Match.num %d\n", g_Match.num);
	PrintDebugProc("Match.AblazeTime %d\n", g_Match.AblazeTime);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMatch(void)
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

	// �}�b�`��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, g_Match.Pos.x, g_Match.Pos.y, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// ����`��
	if(g_Match.AblazeTime > 0)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, g_Match.Pos.x, g_Match.Pos.y-50, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}

//
void StandbyMatch(void)
{
	if(g_Match.AblazeTime>0)g_Match.AblazeTime--;
	if (g_Match.AblazeTime != 0)return;
	
	//�}�b�`���\�������
	if (IsButtonPressed(0, BUTTON_L)&&(0 < g_Match.num))
	{

		//�\������܂ł̎���
		if (g_Match.StandbyTime < STANDBYTIME)
		{
			g_Match.StandbyTime++;
			g_Match.Pos.y = STANDBY_MOVE_FRAME* g_Match.StandbyTime + DEFAULTMATCH_POS_Y;
			return;
		}

		//switch (g_Match.force)
		switch (IsButtonForce(0))
		{
		case FORCE_NON:
			break;
		case FORCE_SLOW:
			g_Match.num--;
			g_Match.AblazeTime = 300;
			break;
		case FORCE_MIDDLE:
			g_Match.num--;
			g_Match.AblazeTime = 600;
			break;
		case FORCE_FAST:
			g_Match.num--;
			g_Match.AblazeTime = 100;
			break;
		}

	}
	else
	{
		//���낵�I���܂ł̎���
		if (0 < g_Match.StandbyTime)
		{
			g_Match.StandbyTime--;
			g_Match.Pos.y = STANDBY_MOVE_FRAME * g_Match.StandbyTime + DEFAULTMATCH_POS_Y;
			return;
		}
	}

}


void SetMatchForce(int force)
{
	g_Match.force = force;
}
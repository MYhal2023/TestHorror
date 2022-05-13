//=============================================================================
//
// ���C�^�[���� [lighter.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "model.h"
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
//#define	MODEL_???			"data/MODEL/???"		// �ǂݍ��ރ��f����



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
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	//if (g_Lighter.use == false) return;

	//// ���[���h�}�g���b�N�X�̏�����
	//mtxWorld = XMMatrixIdentity();

	//// �X�P�[���𔽉f
	//mtxScl = XMMatrixScaling(g_Lighter.scl.x, g_Lighter.scl.y, g_Lighter.scl.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// ��]�𔽉f
	//mtxRot = XMMatrixRotationRollPitchYaw(g_Lighter.rot.x, g_Lighter.rot.y, g_Lighter.rot.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//// �ړ��𔽉f
	//mtxTranslate = XMMatrixTranslation(g_Lighter.pos.x, g_Lighter.pos.y, g_Lighter.pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//SetWorldMatrix(&mtxWorld);

	//XMStoreFloat4x4(&g_Lighter.mtxWorld, mtxWorld);

	//// ���f���`��
	//DrawModel(&g_Lighter.model);

	//// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);
}



//=============================================================================
// ���C�^�[ON
//=============================================================================

void SetLighterOn(BOOL flag)
{
	//CheckOil();
	if (!g_Lighter.use)		return;

	PlaySound(SOUND_LABEL_SE_LighterON);		//

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
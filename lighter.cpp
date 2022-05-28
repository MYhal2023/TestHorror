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
#include "camera.h"
#include "time.h"
#include "game.h"
#include "itembox.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_LIGHTER		"data/MODEL/lighter.obj"			// �ǂݍ��ރ��f����

#define DISTANCE_X					(25.0f)
#define DISTANCE_Y					(20.0f)
#define DISTANCE_Z					(25.0f)

#define NO_USE_Y						(25.0f)

#define LIGHTER_SCALE						(0.3f)

#define TEXTURE_WIDTH				(160)	// �e�N�X�`���T�C�Y
#define TEXTURE_HEIGHT				(200)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

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

static int						g_TexNo;					// �e�N�X�`���ԍ�
static LIGHTER					g_Lighter;					

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitLighter(void)
{
	ID3D11Device *pDevice = GetDevice();

	LoadModel(MODEL_LIGHTER, &g_Lighter.model);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// ������
	CAMERA		camera = *GetCamera();

	g_Lighter.pos = { camera.pos.x + sinf(camera.rot.y)*DISTANCE_X, camera.pos.y - NO_USE_Y  , camera.pos.z + cosf(camera.rot.y)*DISTANCE_Z };
	g_Lighter.rot = { camera.rot.x , camera.rot.y + XM_PI*0.5f ,camera.rot.z };
	g_Lighter.scl = { LIGHTER_SCALE , LIGHTER_SCALE , LIGHTER_SCALE};
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

	UnloadModel(&g_Lighter.model);

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLighter(void)
{


#ifdef _DEBUG	// �f�o�b�O����\������

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	SetLighterOn(TRUE);
	//}
	//if (GetKeyboardTrigger(DIK_J))
	//{
	//	SetLighterOff();
	//}
	if (GetKeyboardTrigger(DIK_I))
	{
		AddOil(10.0f);
	}
#endif

	CheckOil();
	MoveLighter();
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
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	if (g_Lighter.use == FALSE || g_Lighter.pos.y <= GAME_Y_CAM - NO_USE_Y) return;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Lighter.scl.x, g_Lighter.scl.y, g_Lighter.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Lighter.rot.x, g_Lighter.rot.y, g_Lighter.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Lighter.pos.x, g_Lighter.pos.y, g_Lighter.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Lighter.mtxWorld, mtxWorld);

	// ���f���`��
	DrawModel(&g_Lighter.model);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}



//=============================================================================
// ���C�^�[ON
//=============================================================================

void SetLighterOn(BOOL flag)
{
	//CheckOil();
	if (!g_Lighter.use)		return;
	if(g_Lighter.out == FALSE && flag == TRUE)PlaySound(SOUND_LABEL_SE_LighterON);		//

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

void MoveLighter()
{
	CAMERA		camera = *GetCamera();
	g_Lighter.pos.x = camera.pos.x + sinf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_X;
	g_Lighter.pos.z = camera.pos.z + cosf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_Z;
	if (g_Lighter.out == FALSE && g_Lighter.pos.y > camera.pos.y - NO_USE_Y)
	{
		g_Lighter.pos.y -= LIGHTER_MOVE;
	}

	if (g_Lighter.out == TRUE && g_Lighter.pos.y < camera.pos.y - DISTANCE_Y)
	{
		g_Lighter.pos.y += LIGHTER_MOVE;
	}

	g_Lighter.rot.y = camera.rot.y + XM_PI * 0.5f;


}
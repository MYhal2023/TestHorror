//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "time.h"
#include "debugproc.h"
#include "match.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	POS_X_CAM		(0.0f)			// �J�����̏����ʒu(X���W)
//#define	POS_Y_CAM		(200.0f)		// �J�����̏����ʒu(Y���W)
//#define	POS_Z_CAM		(-400.0f)		// �J�����̏����ʒu(Z���W)


#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.015f)								// �J�����̉�]��

typedef enum
{
	RUN,
	BETWEEN,
	WALK
}CAMERA_PHASE;
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Cam;		// �J�����f�[�^

static int				g_ViewPortType = TYPE_FULL_SCREEN;

static float			g_WalkCamera;	//1f������̃J�����̕ω���
static float			g_CamPosY;
static int				g_Prevrun;
static float			g_CamBetweenpos;
static float			g_CamInt;
static XMFLOAT3			g_oldpos;
static XMFLOAT3			g_oldat;
static BOOL				g_PosYTrigger = FALSE;
static BOOL				g_PosYSwitch = TRUE;
//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Cam.pos = { GAME_X_CAM, GAME_Y_CAM, GAME_Z_CAM };
	g_Cam.at = { 0.0f, 0.0f, 0.0f };
	g_Cam.atPos = { 0.0f, GAME_Y_CAM, 0.0f };
	g_Cam.up  = { 0.0f, 1.0f, 0.0f };
	g_Cam.rot = { 0.0f, 0.0f, 0.0f };

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = g_Cam.pos.x - g_Cam.at.x;
	vz = g_Cam.pos.z - g_Cam.at.z;
	g_Cam.len = sqrtf(vx * vx + vz * vz);
	
	g_Cam.tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Cam.move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
	g_Cam.tbl_size = NULL;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// �r���[�|�[�g�^�C�v�̏�����
	g_ViewPortType = TYPE_FULL_SCREEN;

	g_CamBetweenpos = 0.0f;
	g_CamPosY = 10.0f;
	g_PosYTrigger = TRUE;
	g_PosYSwitch = FALSE;
	switch (GetPlayStage())
	{
	case PRISON_STAGE:
		g_Cam.rot = { 0.0f, XM_PI * 1.0f, 0.0f };
		break;
	}
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player = GetPlayer();

	if (GetKeyboardPress(DIK_Q) || IsButtonPressed(0, BUTTON_R_LEFT))
	{// �����_����u���v
		g_Cam.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_Cam.rot.y < -XM_PI)
		{
			g_Cam.rot.y += XM_PI * 2.0f;
		}

		g_Cam.at.x = g_Cam.pos.x + sinf(g_Cam.rot.y) * g_Cam.len;
		g_Cam.at.z = g_Cam.pos.z + cosf(g_Cam.rot.y) * g_Cam.len;
	}

	if (GetKeyboardPress(DIK_E) || IsButtonPressed(0, BUTTON_R_RIGHT))
	{// �����_����u�E�v
		g_Cam.rot.y += VALUE_ROTATE_CAMERA;
		if (g_Cam.rot.y > XM_PI)
		{
			g_Cam.rot.y -= XM_PI * 2.0f;
		}

		g_Cam.at.x = g_Cam.pos.x + sinf(g_Cam.rot.y) * g_Cam.len;
		g_Cam.at.z = g_Cam.pos.z + cosf(g_Cam.rot.y) * g_Cam.len;
	}


	if (g_Cam.tbl_adr != NULL)
	{
		// �ړ�����
		int		index = (int)g_Cam.move_time;
		float	time = g_Cam.move_time - index;
		int		size = g_Cam.tbl_size;

		float dt = 1.0f / g_Cam.tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
		g_Cam.move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂��B���`��ԃf�[�^������
		{
			g_Cam.move_time = 0.0f;
			index = 0;
			g_Cam.tbl_adr = NULL;
			return;
		}

		// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
		XMVECTOR p1 = XMLoadFloat3(&g_Cam.tbl_adr[index + 1].pos);	// ���̏ꏊ
		XMVECTOR p0 = XMLoadFloat3(&g_Cam.tbl_adr[index + 0].pos);	// ���݂̏ꏊ
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Cam.pos, p0 + vec * time);

		// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
		XMVECTOR r1 = XMLoadFloat3(&g_Cam.tbl_adr[index + 1].rot);	// ���̊p�x
		XMVECTOR r0 = XMLoadFloat3(&g_Cam.tbl_adr[index + 0].rot);	// ���݂̊p�x
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Cam.rot, r0 + rot * time);

		// �����_�̕ύX�ʂ����߂� S = StartX + (EndX - StartX) * ���̎���
		XMVECTOR s1 = XMLoadFloat3(&g_Cam.tbl_adr[index + 1].scl);	// ���̒����_
		XMVECTOR s0 = XMLoadFloat3(&g_Cam.tbl_adr[index + 0].scl);	// ���݂̒����_
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Cam.atPos, s0 + scl * time);

	}


#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Camera:ZC QE TB YN UM R\n");
#endif
}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Cam.pos), XMLoadFloat3(&g_Cam.at), XMLoadFloat3(&g_Cam.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Cam.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Cam.mtxInvView, mtxInvView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Cam.mtxProjection, mtxProjection);

	SetShaderCamera(g_Cam.pos);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Cam;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	//ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	//D3D11_VIEWPORT vp;

	//g_ViewPortType = type;

	//// �r���[�|�[�g�ݒ�
	//switch (g_ViewPortType)
	//{
	//case TYPE_FULL_SCREEN:
	//case TYPE_LIGHT_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_LEFT_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH / 2;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_RIGHT_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH / 2;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_UP_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_DOWN_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
	//	break;

	//}
	//g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}



// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(XMFLOAT3 pos)
{
	PLAYER *player = GetPlayer();
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_X))
		player->stamina = 10000;
#endif

	//�����̍��W�ɕύX�ʂ����Z�B
	pos.x += g_Cam.atPos.x;
	pos.y += g_Cam.atPos.y;
	pos.z += g_Cam.atPos.z;
	// �J�����̒����_���Z�b�g
	g_Cam.at = { pos.x,pos.y + g_CamPosY,pos.z };

	// �J�����̎��_���J������Y����]�ɑΉ������Ă���
	g_Cam.pos.x = g_Cam.at.x - sinf(g_Cam.rot.y) * g_Cam.len;
	g_Cam.pos.z = g_Cam.at.z - cosf(g_Cam.rot.y) * g_Cam.len;
	g_Cam.pos.y = pos.y + g_CamPosY;


	//�����Ă鎞�̉�ʂ̗h��
	if (player->dash == TRUE)
	{
		RunningCamera();
	}
	else// if (g_Prevrun==RUN)
	{
		//���ԕ�ԃv���O����
		CameraBetween();
	}
}

//�����Ă鎞�̃J�����̗h��
void RunningCamera(void)
{
	//g_Prevrun = RUN;
	g_WalkCamera+=0.1f;
	g_CamBetweenpos = 1.0f;
	g_Cam.pos.y += sinf(g_WalkCamera)*0.025f;
	g_Cam.at.y  += sinf(g_WalkCamera)*0.2f;
	g_oldpos = g_Cam.pos;
	g_oldat = g_Cam.at;
}


void CameraBetween(void)
{
	if (g_CamBetweenpos < 0.0f)return;
	g_CamBetweenpos -= 0.025f;
	
	//g_Cam.pos.x = (g_oldpos.x * g_CamBetweenpos + g_Cam.pos.x * (1.0f - g_CamBetweenpos));
	g_Cam.pos.y = (g_oldpos.y * g_CamBetweenpos + g_Cam.pos.y * (1.0f - g_CamBetweenpos));
	//g_Cam.pos.z = (g_oldpos.z * g_CamBetweenpos + g_Cam.pos.z * (1.0f - g_CamBetweenpos));

	//g_Cam.at.x = (g_oldat.x * g_CamBetweenpos + g_Cam.at.x * (1.0f - g_CamBetweenpos));
	g_Cam.at.y = (g_oldat.y * g_CamBetweenpos + g_Cam.at.y * (1.0f - g_CamBetweenpos));
	//g_Cam.at.z = (g_oldat.z * g_CamBetweenpos + g_Cam.at.z * (1.0f - g_CamBetweenpos));

}

BOOL GetCameraPos(void)
{
	return g_PosYTrigger;
}

void SetChangeCamera(void)
{
	PLAYER *player = GetPlayer();
	MATCH *match = GetMatch();
	if (CheckLightOn() == TRUE || match->Out)return;
	if (GetKeyboardTrigger(DIK_C) || IsButtonTriggered(0, BUTTON_R))
	{
		//g_PosYTrigger == TRUE ? g_PosYTrigger = FALSE : g_PosYTrigger = TRUE;
		g_PosYSwitch = TRUE;
	}
	if (g_PosYSwitch)
	{
		player->spd = 0.0f;
		if (g_PosYTrigger)
		{
			const float minLine = -20.0f;
			g_CamPosY -= 1.0f;
			if (g_CamPosY <= minLine) {
				g_CamPosY = minLine;
				g_PosYTrigger = FALSE;
				g_PosYSwitch = FALSE;
			}
		}
		else
		{
			const float maxLine = 10.0f;
			g_CamPosY += 1.0f;
			if (g_CamPosY >= maxLine) {
				g_CamPosY = maxLine;
				g_PosYTrigger = TRUE;
				g_PosYSwitch = FALSE;
			}
		}
	}

}
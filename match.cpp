//=============================================================================
//
// �}�b�`���� [match.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "input.h"
#include "match.h"
#include "itembox.h"
#include "debugproc.h"
#include "sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	MODEL_MATCH		"data/MODEL/match.obj"			// �ǂݍ��ރ��f����


#define TEXTURE_WIDTH				(100)	// �e�N�X�`���T�C�Y
#define TEXTURE_HEIGHT				(100)	// 

#define DISTANCE_X					(25.0f)
#define DISTANCE_Y					(10.0f)
#define DISTANCE_Z					(25.0f)

#define NO_USE_Y					(25.0f)

#define MATCH_SCALE					(0.5f)

#define MATCH_POS_X					(600)		// �}�b�`��\������ꏊ(X��)
#define DEFAULTMATCH_POS_Y			(600.0f)	// �ʏ펞�̃}�b�`�̏ꏊ(Y��)
#define STANDBYMATCH_POS_Y			(500.0f)	// �X�^���o�C���̃}�b�`�̏ꏊ(Y��)
#define STANDBYTIME					(10)		// �}�b�`���\����܂ł̎���
#define STANDBY_MOVE_FRAME			((STANDBYMATCH_POS_Y-DEFAULTMATCH_POS_Y)/STANDBYTIME)//�\���鎞��1�t���[��������̈ړ���
#define MATCH_NUM					(10)			// �}�b�`�̖{��(����)

#define MATCH_SLOW					(800)
#define MATCH_MIDDLE				(1200)
#define MATCH_FAST					(500)
//#define	MODEL_???			"data/MODEL/???"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���





static MATCH						g_Match;

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMatch(void)
{
	ID3D11Device *pDevice = GetDevice();

	LoadModel(MODEL_MATCH, &g_Match.model);


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

	CAMERA		camera = *GetCamera();

	g_Match.Pos = { camera.pos.x + sinf(camera.rot.y)*DISTANCE_X, camera.pos.y - NO_USE_Y  , camera.pos.z + cosf(camera.rot.y)*DISTANCE_Z };
	g_Match.rot = { camera.rot.x , camera.rot.y + XM_PI ,camera.rot.z + XM_PI*0.15f };
	g_Match.scl = { MATCH_SCALE , MATCH_SCALE , MATCH_SCALE };

	g_Match.Out = FALSE;

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

	UnloadModel(&g_Match.model);

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMatch(void)
{

	StandbyMatch();



#ifdef _DEBUG	// �f�o�b�O����\������
	//�}�b�`�̔R�Ď��Ԃ�ݒ�
	if (GetKeyboardTrigger(DIK_M))
	{
		g_Match.AblazeTime = 5000;
	}
	if (GetKeyboardTrigger(DIK_N))
	{
		g_Match.AblazeTime = 0;
	}

	PrintDebugProc("Match.num %d\n", g_Match.num);
	PrintDebugProc("Match.AblazeTime %d\n", g_Match.AblazeTime);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMatch(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	if (g_Match.Use == FALSE || g_Match.Pos.y <= GAME_Y_CAM - NO_USE_Y) return;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Match.scl.x, g_Match.scl.y, g_Match.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Match.rot.x, g_Match.rot.y, g_Match.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Match.Pos.x, g_Match.Pos.y, g_Match.Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Match.mtxWorld, mtxWorld);

	// ���f���`��
	DrawModel(&g_Match.model);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//
void StandbyMatch(void)
{
	//�}�b�`���R���Ă��鎞�̏���
	int oldBtime = g_Match.AblazeTime;				//�ߋ��̎c�R�Ď��Ԃ�ۑ�
	if (g_Match.AblazeTime > 0)g_Match.AblazeTime--;//�R�Ď��Ԃ��c���Ă�Ȃ猸�Z
	int nowBtime = g_Match.AblazeTime;				//���Z��̎c�R�Ď��Ԃ�ۑ�
	//�R���s������}�b�`�̃X�^���o�C���Ԃ����Z�b�g����
	if (nowBtime <= 0 && oldBtime > 0) 	//�ߋ��c��1�ȏ�A���ݎc��0�ȉ����������R���s�����^�C�~���O�I
	{
		g_Match.StandbyTime = 0;
		g_Match.Out = FALSE;
	}
	MoveMatch();

	//�}�b�`���R���ĂȂ��Ȃ炱���ŏ������I���
	if (g_Match.AblazeTime > 0)return;
	
	//�}�b�`���R���Ă��Ȃ����̏���
	//�}�b�`���\�������
	if (!GetCameraPos())return;
	if (IsButtonPressed(0, BUTTON_L)&&(0 < g_Match.num) && ReturnSelect() == MATCH_ITEM)
	{
		g_Match.Out = TRUE;
		//�\������܂ł̎���
		if (g_Match.StandbyTime < STANDBYTIME)
		{
			g_Match.StandbyTime++;
			return;
		}

		if(IsButtonPressed(0, BUTTON_R)) SetForceState(TRUE);
		else SetForceState(FALSE);

		switch (IsButtonForce(0))
		{
		case FORCE_NON:
			break;
		case FORCE_SLOW:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_SLOW;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		case FORCE_MIDDLE:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_MIDDLE;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		case FORCE_FAST:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_FAST;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		}
		InitForce(0);
	}
	else
	{
		//���낵�I���܂ł̎���
		if (0 < g_Match.StandbyTime)
		{
			g_Match.StandbyTime--;
			return;
		}
	}

}

void MoveMatch()
{
	CAMERA		camera = *GetCamera();
	g_Match.Pos.x = camera.pos.x + sinf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_X;
	g_Match.Pos.z = camera.pos.z + cosf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_Z;
	if (g_Match.Out == FALSE && g_Match.Pos.y > camera.pos.y - NO_USE_Y)
	{
		g_Match.Pos.y -= MATCH_MOVE;
	}

	if (g_Match.Out == TRUE && g_Match.Pos.y < camera.pos.y - DISTANCE_Y)
	{
		g_Match.Pos.y += MATCH_MOVE;
	}

	g_Match.rot.y = camera.rot.y + XM_PI * 0.5f;
}


void SetMatchForce(int force)
{
	g_Match.force = force;
}

MATCH *GetMatch(void)
{
	return &g_Match;
}
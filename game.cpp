//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "title.h"

#include "player.h"
#include "enemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "score.h"
#include "interface.h"
#include "life.h"
#include "collision.h"
#include "debugproc.h"
#include "file.h"
#include "particle.h"

#include "lighter.h"
#include "match.h"
#include "check_game.h"
#include "itembox.h"
#include "furniture.h"
#include "stagefurniture.h"
#include "item.h"
#include "stage.h"
#include "tutorial.h"
#include "game_over.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF
static int	g_PlayMode = MAIN_GAME;
static int	g_PlayStage = FIRST_STAGE;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	InitSystem();
	switch (GetMode())
	{
	case MODE_TITLE:
		break;
	case MODE_GAME:
		InitStage(g_PlayStage);
		break;
	case MODE_RESULT:
		break;
	}

	return S_OK;
}

void InitSystem(void)
{
	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	InitEnemy();

	InitFurniture();

	InitItem();

	InitMatch();

	InitLighter();

	// �X�R�A�̏�����
	InitScore();

	// ���C�t�̏�����
	InitLife();

	//UI�\��������
	InitInterface();

	InitItembox();

	InitParticle();
	InitGame_over();

}
//�X�e�[�W�ʂ̏���������
void InitStage(int stageNum)
{
	switch (stageNum)
	{
	case DEBUG_STAGE:
		InitDebugStage();
		break;
	case PRISON_STAGE:
		InitFastStage();
		break;
	case FIRST_STAGE:
		InitSecondStage();
		break;
	case CLEAR_STAGE:
		InitClearStage();
		break;
	case MAX_STAGE:
		break;
	}
}

//�f�o�b�O�X�e�[�W�̏�����
void InitDebugStage(void)
{
	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), FIELD_X, FIELD_Z, BLOCK_SIZE, BLOCK_SIZE, WATER);
	//�}�b�v�Ɏg���ǂ̏�����
	InitFieldMeshWall();
}

//�S���X�e�[�W������(���X�e�[�W�H)
void InitFastStage(void)
{
	//�}�b�v�Ɏg�����b�V���A�I�u�W�F�N�g�̏�����
	InitSetTutorial();
}

void InitSecondStage(void)
{
	InitFurnitureFirStage();

	//�}�b�v�Ɏg���ǂ̏�����
	InitSetStage();
}

void InitClearStage(void)
{
	//�}�b�v�Ɏg�����b�V���A�I�u�W�F�N�g�̏�����
	InitSetClearStage();
}
//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	UninitParticle();

	UninitEnemy();

	//UI�\���I������
	UninitInterface();

	UninitItembox();

	UninitMatch();

	UninitLighter();

	// �X�R�A�̏I������
	UninitScore();

	// ���C�t�̏I������
	UninitLife();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	UninitFurniture();

	UninitStageFurniture();

	UninitItem();

	// �e�̏I������
	UninitShadow();
	UninitGame_over();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif
	UpdateGame_over();
	if (CheckGameover() == TRUE)return;	//�Q�[���I�[�o�[�Ȃ�X�V���Ȃ�

	if(g_bPause == FALSE)
		return;
	PLAYER *player = GetPlayer();

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	UpdateFurniture();

	UpdateItem();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	UpdateEnemy();

	UpdateLight();

	//�Q�[�������ł̂����
	CheckGame();

	UpdateItembox();

	// �e�̍X�V����
	UpdateShadow();

	UpdateFog();

	UpdateMatch();

	UpdateLighter();

	UpdateParticle();

	// �X�R�A�̍X�V����
	UpdateScore();

	//UI�\���X�V����
	UpdateInterface();

	// ���C�t�̍X�V����
	UpdateLife();
	
	UpdateSound();

	CheckModeChange();
}

void UpdateFirstGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif
	UpdateGame_over();
	if (CheckGameover() == TRUE)return;	//�Q�[���I�[�o�[�Ȃ�X�V���Ȃ�

	if (g_bPause == FALSE)
		return;
	PLAYER *player = GetPlayer();

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	UpdateStageFurniture();

	UpdateItem();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	UpdateEnemy();

	UpdateLight();

	//�Q�[�������ł̂����
	CheckGame();

	UpdateItembox();

	// �e�̍X�V����
	UpdateShadow();

	UpdateFog();

	UpdateMatch();

	UpdateLighter();

	UpdateParticle();

	// �X�R�A�̍X�V����
	UpdateScore();

	//UI�\���X�V����
	UpdateInterface();

	// ���C�t�̍X�V����
	UpdateLife();

	UpdateSound();

	CheckModeChange();

}
//=============================================================================
// �`�揈��(�J�����ڐ�)
//=============================================================================
void DrawGame0(void)
{
	PLAYER *player = GetPlayer();
	//�V�F�[�_�[�Ǘ�
	//�|�X�g�G�t�F�N�g��������ꍇ�͂�������
	int ans = MODE_PLANE;
	SwapShader(ans);

	DrawGame_over();
	if (CheckGameover() == TRUE)return;	//�Q�[���I�[�o�[�Ȃ�X�V���Ȃ�

	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��
	DrawMeshField();

	DrawFurniture();

	DrawItem();

	// �e�̕`�揈��
	DrawShadow();


	// �v���C���[�̕`�揈��
	DrawPlayer();

	DrawMatch();

	DrawLighter();

	DrawParticle();

	DrawEnemy();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


	//// �X�R�A�̕`�揈��
	//DrawScore();

	// ���C�t�̕`�揈��
	DrawLife();

	DrawItembox();

	////UI�\���`�揈��
	DrawInterface();

	DrawGame_over();
	//�V�F�[�_�[�Ǘ�
	//�V�F�[�_�[�����ɖ߂��B�|�X�g�G�t�F�N�g�͂����܂�
	ans = MODE_PLANE;
	SwapShader(ans);

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}

void DrawFirstStageGame(void)
{
	PLAYER *player = GetPlayer();
	//�V�F�[�_�[�Ǘ�
	//�|�X�g�G�t�F�N�g��������ꍇ�͂�������
	int ans = MODE_PLANE;
	SwapShader(ans);

	DrawGame_over();
	if (CheckGameover() == TRUE)return;	//�Q�[���I�[�o�[�Ȃ�X�V���Ȃ�

	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��
	DrawMeshField();

	DrawStageFurniture();

	DrawItem();

	// �e�̕`�揈��
	DrawShadow();


	// �v���C���[�̕`�揈��
	DrawPlayer();

	DrawMatch();

	DrawLighter();

	DrawParticle();

	DrawEnemy();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


	//// �X�R�A�̕`�揈��
	//DrawScore();

	// ���C�t�̕`�揈��
	DrawLife();

	DrawItembox();

	////UI�\���`�揈��
	DrawInterface();

	//�V�F�[�_�[�Ǘ�
	//�V�F�[�_�[�����ɖ߂��B�|�X�g�G�t�F�N�g�͂����܂�
	ans = MODE_PLANE;
	SwapShader(ans);

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);

}

//=============================================================================
// �`�揈��(���C�g�ڐ��B�����������e�Ɋւ���I�u�W�F�N�g�������w��)
//=============================================================================
void DrawGame1(void)
{
	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �v���C���[�̕`�揈��
	DrawPlayer();

}


void DrawGame(void)
{


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif
	XMFLOAT3 pos;

	// �v���C���[���_
	pos = GetPlayer()->pos;
	//pos = GetEnemy()->pos;	//�f�o�b�O�p
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		switch (g_PlayStage)
		{
		case DEBUG_STAGE:
		case PRISON_STAGE:
		case CLEAR_STAGE:
			SetViewPort(TYPE_FULL_SCREEN);
			DrawGame0();
			break;
		case FIRST_STAGE:
			SetViewPort(TYPE_FULL_SCREEN);
			DrawFirstStageGame();
		}
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		break;

	case TYPE_LIGHT_SCREEN:	//���C�g�ڐ��̕`������邽�߂̃r���[�|�[�g�^�C�v
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame1();
		break;

	}

}



void CheckModeChange(void)
{

}

void SetPlayMode(int playMode)
{
	g_PlayMode = playMode;
}

int GetPlayMode(void)
{
	return g_PlayMode;
}

void SetPlayStage(int stageNum)
{
	g_PlayStage = stageNum;
}

int GetPlayStage(void)
{
	return g_PlayStage;
}

void SetViewPortType(int viewport)
{
	g_ViewPortType_Game = viewport;
}

int GetViewPortTypeGame(void)
{
	return g_ViewPortType_Game;
}

//���l�̃N�����v�B����1:�N�����v���������l�A����2:�ŏ��l�A����3:�ő�l
float FloatClamp(float x, float a, float b)
{
	if (x < a)
	{
		return a;
	}
	else if (x > b)
	{
		return b;
	}
	else
	{
		return x;
	}
}

//float�^�̐��l�̔�r������Bflag��FALSE�Ȃ珬���������ATRUE�Ȃ�傫������Ԃ�
float FloatCompare(BOOL flag, float a, float b)
{
	float ans = 0.0f;//�ꉞ0.0f���Z�b�g���Ă���
	switch (flag)
	{
	case FALSE:
		if (a > b)return b;
		else	  return a;
		break;
	case TRUE:
		if (a > b)return a;
		else	  return b;
		break;
	}

	return ans;
}

//���C�g���t���Ă��邩�ۂ��BTRUE�ŕt���Ă�BFALSE�ŕt���ĂȂ��B
BOOL CheckLightOn(void)
{
	BOOL ans = FALSE;
	MATCH *match = GetMatch();
	LIGHTER *lighter = GetLighter();
	//�}�b�`�R�Ď��Ԃ�0�ȏ�A�܂��̓��C�^�[�������o���Ă��鎞�A�������鈵��
	if (match->AblazeTime > 0 ||
		lighter->out == TRUE)
		ans = TRUE;

	return ans;
}

BOOL CheckGameover(void)
{
	BOOL ans = FALSE;
	PLAYER *player = GetPlayer();
	if (player->life <= 0)
		ans = TRUE;

	if (ans)
		SetGameOver();
	return ans;
}
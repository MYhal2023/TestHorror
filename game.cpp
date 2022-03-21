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

#include "player.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "score.h"
#include "interface.h"
#include "life.h"
#include "time.h"
#include "collision.h"
#include "debugproc.h"
#include "file.h"

#include "lighter.h"
#include "match.h"

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
//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	switch (GetMode())
	{
	case MODE_TITLE:
	case MODE_GAME:
		g_ViewPortType_Game = TYPE_FULL_SCREEN;


		// �t�B�[���h�̏�����
		InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), FIELD_X, FIELD_Z, BLOCK_SIZE, BLOCK_SIZE, WATER);

		// ���C�g��L����	// �e�̏���������
		InitShadow();

		// �v���C���[�̏�����
		InitPlayer();

		// �ǂ̏�����
		InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, FIELD_Z_LIMIT), XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);
		InitMeshWall(XMFLOAT3(-FIELD_X_LIMIT, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);
		InitMeshWall(XMFLOAT3(FIELD_X_LIMIT, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);
		InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, -FIELD_Z_LIMIT), XMFLOAT3(0.0f, XM_PI, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);

		// ��(�����p�̔�����)
		InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, FIELD_Z_LIMIT), XMFLOAT3(0.0f, XM_PI, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);
		InitMeshWall(XMFLOAT3(-FIELD_X_LIMIT, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);
		InitMeshWall(XMFLOAT3(FIELD_X_LIMIT, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);
		InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, -FIELD_Z_LIMIT), XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), WALL_XZ, WALL_Y, WALL_BLOCK_SIZE_XZ, WALL_BLOCK_SIZE_Y);

		InitMatch();

		InitLighter();

		// �X�R�A�̏�����
		InitScore();

		// ���C�t�̏�����
		InitLife();

		// �������Ԃ̏�����
		InitTime();

		//UI�\��������
		InitInterface();



		////BGM�Đ�
		//PlaySound(SOUND_LABEL_BGM_title);
		break;

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{

	//UI�\���I������
	UninitInterface();

	UninitMatch();

	UninitLighter();

	// �X�R�A�̏I������
	UninitScore();

	// ���C�t�̏I������
	UninitLife();

	// �������Ԃ̏I������
	UninitTime();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();


	// �e�̏I������
	UninitShadow();

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

	if(g_bPause == FALSE)
		return;
	PLAYER *player = GetPlayer();

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	UpdateLight();

	// �e�̍X�V����
	UpdateShadow();


	if (g_PlayMode <= TUTORIAL)	//�^�C�g���Ȃ炱���܂ōX�V
		return;

	UpdateMatch();

	UpdateLighter();


	// �X�R�A�̍X�V����
	UpdateScore();

	//UI�\���X�V����
	UpdateInterface();

	// ���C�t�̍X�V����
	UpdateLife();

	// �������Ԃ̍X�V����
	UpdateTime();
	
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

	// 3D�̕���`�悷�鏈��
	//�V�F�[�_�[�؂�ւ��B���ʂ̕`�揈��
	ans = MODE_SPECULAR;
	if (GetTime() <= 0 || player->life <= 0)
		ans = MODE_MONO;

	SwapShader(ans);
	// �n�ʂ̕`�揈��
	DrawMeshField();

	//�V�F�[�_�[�؂�ւ��B�m�[�}���̕��ɖ߂�
	ans = MODE_PLANE;
	if (GetTime() <= 0 || player->life <= 0)
		ans = MODE_MONO;

	SwapShader(ans);

	// �e�̕`�揈��
	DrawShadow();


	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	if (g_PlayMode > TUTORIAL)
	{
		DrawMatch();

		DrawLighter();

		// �X�R�A�̕`�揈��
		DrawScore();

		// ���C�t�̕`�揈��
		DrawLife();

		// �������Ԃ̕`�揈��
		DrawTime();

		////UI�\���`�揈��
		//DrawInterface();
	}

	//�V�F�[�_�[�Ǘ�
	//�V�F�[�_�[�����ɖ߂��B�|�X�g�G�t�F�N�g�͂����܂�
	ans = MODE_PLANE;
	SwapShader(ans);
	if (GetTime() <= 0 || player->life <= 0)
		DrawGameOver();

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
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
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
	PLAYER *player = GetPlayer();
	if (GetPlayMode() == TUTORIAL_GAME)
	{
		if (GetKeyboardTrigger(DIK_X) || IsButtonTriggered(0, BUTTON_B))
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}


	//���Ԑ؂ꂩ���C�t�؂�Ń��U���g��
	if (GetTime() > 0 && player->life > 0)	
		return;

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

void SetPlayMode(int playMode)
{
	g_PlayMode = playMode;
}

int GetPlayMode(void)
{
	return g_PlayMode;
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

//float�^�̐��l�̔�r������Bfalg��FALSE�Ȃ珬���������ATRUE�Ȃ�傫������Ԃ�
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
//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "time.h"
#include "light.h"
#include "debugproc.h"
#include "input.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define TIME_SEPARATE	(3)		//���Ԃ̋�؂�
#define LIGHT_TYPE		(3)		//���Ԃ̋�؂�
#define CHANGE_ANGLE		(0.0005f)		//���Ԃ̋�؂�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;
//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{

	//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position = XMFLOAT3(100.0f, 500.0f, 100.0f);
		g_Light[i].Direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Light[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_Light[i].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		g_Light[i].Attenuation = 100.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = FALSE;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// �����̐ݒ�i���E���Ƃ炷���j
	g_Light[0].Position = XMFLOAT3(500.0f, 500.0f, 500.0f);
	g_Light[0].Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);
	float diffuse = 0.1f;
	if (GetPlayStage() == CLEAR_STAGE)diffuse = 0.8f;
	g_Light[0].Diffuse = XMFLOAT4(diffuse, diffuse, diffuse, 1.0f);
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	g_Light[0].Enable = TRUE;									// ���̃��C�g��ON
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���

		// �����̐ݒ�i���E���Ƃ炷���j
	g_Light[1].Position = XMFLOAT3(0.0f, 50.0f, 0.0f);
	g_Light[1].Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);
	diffuse = 1.0f;
	g_Light[1].Diffuse = XMFLOAT4(diffuse, diffuse, diffuse, 1.0f);
	g_Light[1].Attenuation = LIGHT_LINE_OFF;	// ��������
	g_Light[1].Type = LIGHT_TYPE_POINT;							// ���s����
	g_Light[1].Enable = TRUE;									// ���̃��C�g��ON
	SetLight(1, &g_Light[1]);									// ����Őݒ肵�Ă���

	//g_Light[2].Position = XMFLOAT3(100.0f, 100.0f, 0.0f);
	//g_Light[2].Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);
	//diffuse = 1.0f;
	//g_Light[2].Diffuse = XMFLOAT4(diffuse, diffuse, diffuse, 1.0f);
	//g_Light[2].Attenuation = 300.0f;	// ��������
	//g_Light[2].Type = LIGHT_TYPE_POINT;							// ���s����
	//g_Light[2].Enable = TRUE;									// ���̃��C�g��ON
	//SetLight(2, &g_Light[2]);									// ����Őݒ肵�Ă���

	// �t�H�O�̏������i���̌��ʁj
	g_Fog.FogStart = GAME_FOG_ST;									// ���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd   = GAME_FOG_ED;									// �����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// �t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(TRUE);		// ���̏ꏊ���`�F�b�N���� shadow
}




//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}

//���C�g�̒���
void UpdateLight(void)
{
	PLAYER *player = GetPlayer();
	g_Light[0].Position = XMFLOAT3(player->pos.x + 0.0f, 500.0f, player->pos.z + 200.0f);
	g_Light[0].Direction = XMFLOAT3(0.5f, -1.0f, 0.0f);
#ifdef _DEBUG //�f�o�b�O���̂݃L�[�{�[�h����Ŗ��Ð؂�ւ������R��
	if (GetKeyboardTrigger(DIK_H))
	{
		float diffuse = (g_Light[0].Diffuse.x == 0.1f) ? 1.0f : 0.1f;
		g_Light[0].Diffuse = XMFLOAT4(diffuse, diffuse, diffuse, 1.0f);
	}
#endif
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���

	g_Light[1].Position = XMFLOAT3(player->pos.x, player->pos.y + 50.0f, player->pos.z);
	if (CheckLightOn() == TRUE)
	{
		g_Light[1].Attenuation = LIGHT_LINE_ON;
	}
	else
	{
		g_Light[1].Attenuation = LIGHT_LINE_OFF;
	}
	SetLight(1, &g_Light[1]);
}

void UpdateFog(void)
{
//	BOOL debugSW = FALSE;	//�f�o�b�O���f�p�ϐ�
//#ifdef _DEBUG //�f�o�b�O���̂݃L�[�{�[�h����Ńt�H�O��������
//	if (GetKeyboardTrigger(DIK_H))
//	{
//		g_Fog.FogStart = (g_Fog.FogStart == GAME_FOG_ST) ? DEBUG_FOG_ST : GAME_FOG_ST;
//		g_Fog.FogEnd = (g_Fog.FogEnd == GAME_FOG_ED) ? DEBUG_FOG_ED : GAME_FOG_ED;
//		SetFog(&g_Fog);	//�t�H�O���Z�b�g
//	}
//#endif
//	//debugSW�ϐ��ŁA�f�o�b�O�����ۂ��𔻒f�B���f�̓t�H�O�̊J�n�ʒu�ōs��
//	(g_Fog.FogStart == DEBUG_FOG_ST) ? debugSW = TRUE : debugSW = FALSE;
//
//	//�f�o�b�O���̃t�H�O���g�p���Ă���Ȃ�A�����[�X�ł̃t�H�O���X���[
//	if (debugSW == TRUE)return;
//
//	//���C�g��ԂɓK���Ȃ��t�H�O�̏ꍇ�A��Ԃɍ����悤�ɍăZ�b�g
//	if (CheckLightOn() == TRUE && g_Fog.FogStart != GAME_LFOG_ST)
//	{
//		g_Fog.FogStart = GAME_LFOG_ST;
//		g_Fog.FogEnd = GAME_LFOG_ED;
//		SetFog(&g_Fog);	//�t�H�O���Z�b�g
//	}
//	else if (CheckLightOn() != TRUE && g_Fog.FogStart == GAME_LFOG_ST)
//	{
//		g_Fog.FogStart = GAME_FOG_ST;
//		g_Fog.FogEnd = GAME_FOG_ED;
//		SetFog(&g_Fog);	//�t�H�O���Z�b�g
//	}
}
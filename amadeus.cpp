//=============================================================================
//
// �G�l�~�[AI���� [amadeus.cpp]
// Author : �đq�r�N
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "amadeus.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "time.h"
#include "score.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EYE_VALUE		(3.0f)				//���_�̏�Z�]���l
#define BACK_VALUE		(0.5f)				//�w�ʂ̏�Z�]���l
//��������AI�Ɏg�p�����`
#define EYE_ANGLE		(XM_PI * 0.25f)		//1�����̊p�x
#define ENEMY_VISION	(0.25f)				//�G�l�~�[�̎��E
#define ANGLE_TYPE		(4)					//���p�̐�
#define AI_ROCK_SIZE	(4)					//��̐N���s�̃T�C�Y
//TCP�p�̒萔����
#define AI_WAYAREA_SIZE	(3)					//�E�F�C�|�C���g�����p�萔
#define WAYAREA_SIZE	((AI_WAYAREA_SIZE * 2) - 1)	//�E�F�C�|�C���g�����G���A�̈�ӂ̃|�C���g��(�O���b�h��̐����`)
#define NEAR_ENEMY		(40.0f)				//�E�F�C�|�C���g�ɋ߂��Ƃ͂ǂꂭ�炢�̋������w���̂���`
#define VALUE_VAR		(3)					//�]���Ώۂ̐�
#define EYE_VALUE_TPS	(1.2f)				//���_�̕]���W��
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static WAYPOINT			g_WayPoint[WAYAREA_SIZE][WAYAREA_SIZE];// �E�F�C�|�C���g

static BOOL				g_Load = FALSE;

static float			valueCoe[VALUE_VAR];					//�]���W���̕ۑ���
//=============================================================================
// ����������(�S�Ă̏����������̌�Ɏ��s�����邱��)
//=============================================================================
HRESULT InitAmadeus(void)
{
	for (int i = 0; i < WAYAREA_SIZE; i++)
	{
		for (int k = 0; k < WAYAREA_SIZE; k++)
		{
			g_WayPoint[i][k].pos = { 0.0f, 0.0f, 0.0f };			//���W
			g_WayPoint[i][k].rot = { 0.0f, 0.0f, 0.0f };			// �p�x
			g_WayPoint[i][k].scl = { 1.0f, 1.0f, 1.0f };			// �X�P�[��
			g_WayPoint[i][k].value = 1.0f;			//�]���l
			g_WayPoint[i][k].use = FALSE;			//�g�p�̉�(TRUE�Ŏg�p��)
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAmadeus(void)
{
	if (g_Load == FALSE) return;


	g_Load = FALSE;
}

//========================================================================================================================
// �Ώۂ�ڎ��o���Ă���̂�(����1:�����̈ʒu,����2:�Ώۂ̈ʒu,����3:�����̊p�x,����4:�ǂꂭ�炢�̋����Ȃ�ڎ��ł���̂�)
//========================================================================================================================
BOOL Visibility(XMFLOAT3 pos1, XMFLOAT3 pos2, float rot, float dist)
{
	BOOL ans = FALSE;

	float len = (dist) * (dist);		// ���a��2�悵����
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2�_�Ԃ̋����i2�悵�����j
	float lenSq = 0.0f;
	XMStoreFloat(&lenSq, temp);
	// ���a��2�悵������苗��������
	if (len <= lenSq)
	{
		return ans;	// �ڎ��O�ɂ��邽�߂��̎��_�Ōv�Z�I��
	}

	//�ΏۂƂ̊p�x���Z�o
	float angleX = pos2.x - pos1.x;
	float angleZ = pos2.z - pos1.z;
	float angle = atan2f(angleX, angleZ);
	//���g�̊p�x��8�����ŎZ�o
	float stVision = 0.0f;
	float edVision = 0.0f;
	//angle��XM_PI�`-XMPI�Ȃ̂ł���ɍ��킹��
	//���E���l�̐؂�ڂɋ��܂�Ə����ʓ|�ȏ������K�v
	if ((rot > XM_PI * (1.0f - ENEMY_VISION)) || (rot < XM_PI * (ENEMY_VISION - 1.0f)))
	{
		if (rot > XM_PI * (1.0f - ENEMY_VISION))
		{
			stVision = rot - XM_PI * ENEMY_VISION;
			edVision = (rot + XM_PI * ENEMY_VISION) - (XM_PI * 2.0f);
		}
		else if (rot < XM_PI * (ENEMY_VISION - 1.0f))
		{
			stVision = rot - XM_PI * ENEMY_VISION + (XM_PI * 2.0f);
			edVision = rot + XM_PI * ENEMY_VISION;
		}

		if (angle >= stVision || angle <= edVision)
		{
			ans = TRUE;
		}
	}
	else
	{
		stVision = rot - XM_PI * ENEMY_VISION;
		edVision = rot + XM_PI * ENEMY_VISION;
		if (angle >= stVision && angle <= edVision)
		{
			ans = TRUE;
		}
	}

	return ans;
}

XMVECTOR TacticalPointSystem(int i)	//��p�ʒu��̓V�X�e���B�z��ԍ��������Ă��邱��
{
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();
	int stWayArea = 1 - AI_WAYAREA_SIZE;
	int edWayArea = AI_WAYAREA_SIZE;
	//��������E��ɂ����ăE�F�C�|�C���g������B������
	for (int h = stWayArea; h < edWayArea; h++)
	{
		for (int k = stWayArea; k < edWayArea; k++)
		{
			int a = h + (-stWayArea);
			int b = k + (-stWayArea);
			float x = enemy[i].pos.x + POINT_INTERVAL * k;
			float y = 0.0f;
			float z = enemy[i].pos.z + POINT_INTERVAL * h;
			g_WayPoint[a][b].pos = { x, y, z };
			//���W����Ƌ��Ƀt�B���^�����O�t�F�[�Y��
			g_WayPoint[a][b].use = FilterWayPoint(g_WayPoint[a][b].pos, enemy[i].pos, i);
			g_WayPoint[a][b].value = 1.0f;


			//�E�F�C�|�C���g�]���t�F�[�Y
			if (g_WayPoint[a][b].use == TRUE)
				g_WayPoint[a][b].value = ValueWayPoint(g_WayPoint[a][b].pos, 0.0f, i);

			////�f�o�b�O���A������₷���悤�ɐF�o��ς���
			//if (g_WayPoint[a][b].use == TRUE)
			//	g_WayPoint[a][b].material.Diffuse = { 0.0f + g_WayPoint[a][b].value, 0.0f, 0.0f, 1.0f };
			//else
			//	g_WayPoint[a][b].material.Diffuse = { 0.0f, 0.0f, 1.0f, 1.0f };
		}
	}

	//�c�����E�F�C�|�C���g�̕]���l���r
	float valueMax = 0.0f;		//�ő�]���l��ۑ�
	int value_x = 0;			//�ő�]���l�̔z��ԍ���ۑ�
	int value_z = 0;			//
	for (int a = 0; a < WAYAREA_SIZE; a++)
	{
		for (int b = 0; b < WAYAREA_SIZE; b++)
		{
			if (g_WayPoint[a][b].use == FALSE)continue;	//FALSE�̏ꍇ�A�폜�����E�F�C�|�C���g�Ȃ̂ŏ������X�L�b�v

			if (valueMax < g_WayPoint[a][b].value)
			{
				valueMax = g_WayPoint[a][b].value;
				value_z = a;
				value_x = b;
			}
		}
	}
	//g_WayPoint[value_z][value_x].material.Diffuse = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMVECTOR vec = { 0.0f, 0.0f, 0.0f };
	if (valueMax == 0.0f) //�ǂ��ɂ��ړ���₪�����ꍇ�͂��̂܂ܕԂ�
		return vec;

	//�ړ���
	float vecX = g_WayPoint[value_z][value_x].pos.x - enemy[i].pos.x;
	float vecZ = g_WayPoint[value_z][value_x].pos.z - enemy[i].pos.z;
	//X����Z�������ł̈ړ��x�N�g�����v�Z
	g_WayPoint[value_z][value_x].pos.y = 0.0f;
	enemy[i].pos.y = 0.0f;
	vec = XMLoadFloat3(&g_WayPoint[value_z][value_x].pos) - XMLoadFloat3(&enemy[i].pos);
	//�P�ʃx�N�g�����B����ɃG�l�~�[�̃X�s�[�h��������
	vec = XMVector3Normalize(vec);
	enemy[i].moveVec = vec;

	//�p�x���Z�o���ύX
	float angle = atan2f(vecX, vecZ);
	enemy[i].rot.y = angle;
	return vec;
}

//�E�F�C�|�C���g���t�B���^�����O����֐��B�|�C���g�̍��W�Ǝ��g�̔z��ԍ��������Ă���
BOOL FilterWayPoint(XMFLOAT3 pos, XMFLOAT3 pos2, int k)		//�E�F�C�|�C���g�A���g�̌����W�A���g�̔z��ԍ�
{
	BOOL ans = TRUE;							//�܂���TRUE���Z�b�g�B�������牽�ɂ�����������Ȃ����TRUE���Ԃ�A�E�F�C�|�C���g�Ƃ��Ďg�p����
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();
	XMFLOAT3 checkPos = {player->pos.x, 0.0f, player->pos.z};


	//�N���s�̏ꏊ���폜
	for (int i = 0; i < GetMeshWallNum(); i++)
	{

		if (CheckCrossLine(pos2, pos, GetMeshWallStPosition(i), GetMeshWallEdPosition(i)) == TRUE)
		{
			return ans = FALSE;
		}
	}
	return ans;
}

//�E�F�C�|�C���g��]������֐��B���g�̔z��ԍ��������Ă���
//����1:�E�F�C�|�C���g���W�A����2:�ŏ��ɂ���]���l�A����3:�z��ԍ�
float ValueWayPoint(XMFLOAT3 pos, float value, int k)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();

	XMVECTOR disP = XMLoadFloat3(&player->pos) - XMLoadFloat3(&pos);
	disP = XMVector3Length(disP);				//������2��
	float point;
	XMStoreFloat(&point, disP);
	if (point < 1.0f)				//���W������1.0�ȉ��Ȃ�]���ő�l���o��
	{
		value += 1.0f;
	}
	else
	{
		point = 1.0f / point;						//����Ă���قǌW�����������Ȃ�悤�ɂ���B
		value += 1.0f * point;		//�]���W���Ƌ����̕]������Z
	}

	//if (Visibility(enemy[k].pos, pos, enemy[k].rot.y, POINT_INTERVAL * POINT_INTERVAL) == TRUE)
	//{
	//	value *= EYE_VALUE_TPS;
	//}

	return value;
}
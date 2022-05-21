//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : ���c �E�l
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "meshwall.h"
#include "meshfield.h"
#include "furniture.h"
void InitSetTutorial(void)
{
	//�}�b�v�̏�
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), (int)(TUTORIALWALL_SIZE_XZ / FLOOR_BLOCK_SIZE) + 1, (int)(TUTORIALWALL_SIZE_XZ / FLOOR_BLOCK_SIZE) + 5, FLOOR_BLOCK_SIZE, FLOOR_BLOCK_SIZE, GROUND);
	//�}�b�v�̓V��
	InitMeshWall(XMFLOAT3(TUTORIALWALL_SIZE_XZ*0.5f, TUTORIALWALL_SIZE_Y, 0.0f), XMFLOAT3(XM_PI*0.5f, 0.0f, XM_PI*0.5f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, TUTORIALWALL_SIZE_XZ, TUTORIALWALL_SIZE_XZ, WALL_GRAY);
	//�}�b�v�̕�(���̂����)
	InitMeshWall(XMFLOAT3(TUTORIALWALL_SIZE_XZ * 0.25f + DOOR_SIZE * 0.25f, 1.0f, TUTORIALWALL_SIZE_XZ * 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, TUTORIALWALL_SIZE_XZ*0.5f - DOOR_SIZE * 0.5f, TUTORIALWALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-TUTORIALWALL_SIZE_XZ * 0.25f - DOOR_SIZE * 0.25f, 1.0f, TUTORIALWALL_SIZE_XZ * 0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, TUTORIALWALL_SIZE_XZ*0.5f - DOOR_SIZE * 0.5f, TUTORIALWALL_SIZE_Y, WALL_BLACK);

	//�}�b�v�̕�(���E)
	InitMeshWall(XMFLOAT3(-TUTORIALWALL_SIZE_XZ * 0.5f, 1.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), ROOM, WALL_Y, TUTORIALWALL_SIZE_XZ / ROOM, TUTORIALWALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(TUTORIALWALL_SIZE_XZ * 0.5f, 1.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), ROOM, WALL_Y, TUTORIALWALL_SIZE_XZ / ROOM, TUTORIALWALL_SIZE_Y, WALL_BLACK);
	//�}�b�v�̕�(���̂Ȃ���)
	InitMeshWall(XMFLOAT3(0.0f, 1.0f, -TUTORIALWALL_SIZE_XZ * 0.5f), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, TUTORIALWALL_SIZE_XZ, TUTORIALWALL_SIZE_Y, WALL_BLACK);
	//�K�i�̐ݒu
	InitMeshWall(XMFLOAT3(-DOOR_SIZE * 0.5f, 1.0f, TUTORIALWALL_SIZE_XZ*0.5f + STAIRS_SIZE_XZ * 0.5f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, STAIRS_SIZE_XZ, TUTORIALWALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(DOOR_SIZE*0.5f, 1.0f, TUTORIALWALL_SIZE_XZ*0.5f + STAIRS_SIZE_XZ * 0.5f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, STAIRS_SIZE_XZ, TUTORIALWALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f, 1.0f, TUTORIALWALL_SIZE_XZ*0.5f + STAIRS_SIZE_XZ), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, DOOR_SIZE, TUTORIALWALL_SIZE_Y, WALL_BLACK);

	SetFurniture(XMFLOAT3(0.0f, 0.0f, TUTORIALWALL_SIZE_XZ*0.5f + STAIRS_SHIFT_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), STAIRS);
	SetFurniture(XMFLOAT3(DOOR_SIZE*0.6f, 0.0f, TUTORIALWALL_SIZE_XZ*0.5f - CANDLE_SCL * 0.75f), XMFLOAT3(0.0f, 0.0f, 0.0f), CANDLE);

	float room = TUTORIALWALL_SIZE_XZ / ROOM;

	for (int i = 0; i < ROOM; i++)
	{
		//�S���̕�(����)
		InitMeshWall(XMFLOAT3(-TUTORIALWALL_SIZE_XZ * 0.5f + PRISONSIDE_SIZE_XZ, 1.0f, room * i + room * 0.5f - (TUTORIALWALL_SIZE_XZ * 0.5f) - (DOOR_SIZE*0.5f)), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, room - DOOR_SIZE, TUTORIALWALL_SIZE_Y, WALL_BLACK);
		InitMeshWall(XMFLOAT3(TUTORIALWALL_SIZE_XZ * 0.5f - PRISONSIDE_SIZE_XZ, 1.0f, room * i + room * 0.5f - (TUTORIALWALL_SIZE_XZ * 0.5f) + (DOOR_SIZE*0.5f)), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, room - DOOR_SIZE, TUTORIALWALL_SIZE_Y, WALL_BLACK);
		//�����̃I�u�W�F�N�g�ݒu
		SetFurniture(XMFLOAT3(-TUTORIALWALL_SIZE_XZ * 0.5f + PRISONSIDE_SIZE_XZ - BED_SHIFT_X, 1.0f, room * i + room * 0.5f - (TUTORIALWALL_SIZE_XZ * 0.5f) - (DOOR_SIZE*0.5f) + BED_SHIFT_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), BED);
		SetFurniture(XMFLOAT3(TUTORIALWALL_SIZE_XZ * 0.5f - PRISONSIDE_SIZE_XZ + BED_SHIFT_X, 1.0f, room * i + room * 0.5f - (TUTORIALWALL_SIZE_XZ * 0.5f) + (DOOR_SIZE*0.5f) - BED_SHIFT_Z), XMFLOAT3(0.0f, -XM_PI, 0.0f), BED);
		SetFurniture(XMFLOAT3(-TUTORIALWALL_SIZE_XZ * 0.5f + PRISONSIDE_SIZE_XZ - TOILET_SHIFT_X, 1.0f, room * i + room * 0.5f - (TUTORIALWALL_SIZE_XZ * 0.5f) - (DOOR_SIZE*0.5f) + TOILET_SHIFT_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), TOILET);
		SetFurniture(XMFLOAT3(TUTORIALWALL_SIZE_XZ * 0.5f - PRISONSIDE_SIZE_XZ + TOILET_SHIFT_X, 1.0f, room * i + room * 0.5f - (TUTORIALWALL_SIZE_XZ * 0.5f) + (DOOR_SIZE*0.5f) - TOILET_SHIFT_Z), XMFLOAT3(0.0f, -XM_PI, 0.0f), TOILET);

		if (i == 0)continue;//�����̐����d�؂�̐��̕�������Ȃ�����
		//�S���̕�(�d�؂�)
		InitMeshWall(XMFLOAT3(TUTORIALWALL_SIZE_XZ * 0.5f - PRISONSIDE_SIZE_XZ * 0.5f, 1.0f, room * i - (TUTORIALWALL_SIZE_XZ * 0.5f)), XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, PRISONSIDE_SIZE_XZ, TUTORIALWALL_SIZE_Y, WALL_BLACK);
		InitMeshWall(XMFLOAT3(-(TUTORIALWALL_SIZE_XZ * 0.5f - PRISONSIDE_SIZE_XZ * 0.5f), 1.0f, room * i - (TUTORIALWALL_SIZE_XZ * 0.5f)), XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, PRISONSIDE_SIZE_XZ, TUTORIALWALL_SIZE_Y, WALL_BLACK);

	}
}

void InitSetClearStage(void)
{

}
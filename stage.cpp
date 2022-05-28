//=============================================================================
//
// �`���[�g���A������ [stage.cpp]
// Author : BURRUEL LEANOS GONZALO
//
//=============================================================================

#include "main.h"
#include "meshwall.h"
#include "meshfield.h"
#include "stage.h"
#include "stagefurniture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	ADJUST_X				(500.0f)		//���b�V���t�B�[���h�̍��W�����킹�邽��
#define ADJUST_Z				(600.0f)		
#define MAX_KAGU				(2)				//�Ƌ�̐�

void InitSetStage()
{
	//��
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), (int)(FLOOR_X / FLOOR_BLOCK_SIZE), (int)(FLOOR_Z / FLOOR_BLOCK_SIZE), FLOOR_BLOCK_SIZE, FLOOR_BLOCK_SIZE, 1);

	//��

	//�����@
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 225 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 250.0f, WALL_SIZE_Y, WALL_BLACK); //�v���C���[�ɂԂ����Ă�
	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(237.5f - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(437.5f - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 462.5 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(200 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 400 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 162.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);


	//�����A
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(650 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(837 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(800 - ADJUST_X, 1.0f, 50 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);


	//�����B
	InitMeshWall(XMFLOAT3(212.5f - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 225.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(450 - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 900 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);

	//�����C
	InitMeshWall(XMFLOAT3(637.5f - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(825 - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 150.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 900 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	//25

	//�����D
	InitMeshWall(XMFLOAT3(162.5f - ADJUST_X, 1.0f, 1000 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 1000 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 600.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 1012.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 25.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 1200 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(800 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(700 - ADJUST_X, 1.0f, 1150 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 1112.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 25.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(225 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 150.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);

	//���ӂ̕�
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5 - 100.0f - ADJUST_X, 1.0f, 0.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X - 200.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5f - 100.0f - ADJUST_X, 1.0f, FLOOR_Z - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, WALL_SIZE_Y, 0.0f - ADJUST_Z), XMFLOAT3(XM_PI*0.5f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), (int)(FLOOR_X / FLOOR_BLOCK_SIZE), (int)(FLOOR_Z / FLOOR_BLOCK_SIZE), WALL_SIZE_XZ, WALL_SIZE_XZ, WALL_GRAY);
	//40

	//�h�A�[
	SetStageFurniture(DOOR, XMFLOAT3{ -175.0f,0.0f,0.0f }, XMFLOAT3{ 0.0f,XM_PI*0.5f,0.0f });		//�@
	SetStageFurniture(DOOR, XMFLOAT3{ -200.0f,0.0f,-375.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//�A
	SetStageFurniture(DOOR, XMFLOAT3{ 0.0f,0.0f,-250.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });				//�B
	SetStageFurniture(DOOR, XMFLOAT3{ -125.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI*0.5f,0.0f });	//�C
	SetStageFurniture(DOOR, XMFLOAT3{ -317.5f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI * 0.5f,0.0f });	//�D
	SetStageFurniture(DOOR, XMFLOAT3{ 275.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI * 0.5f,0.0f });	//�E
	SetStageFurniture(DOOR, XMFLOAT3{ 175.0f,0.0f, 0.0f }, XMFLOAT3{ 0.0f,XM_PI * 0.5f,0.0f });		//�F
	SetStageFurniture(DOOR, XMFLOAT3{ 400.0f,0.0f, 525.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//�G
	SetStageFurniture(DOOR, XMFLOAT3{ 400.0f,0.0f, 425.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//�H
	SetStageFurniture(DOOR, XMFLOAT3{ -275.0f,0.0f, 400.0f }, XMFLOAT3{ 0.0f,XM_PI * 0.5f,0.0f });	//�I

	//�h�A�[�̓���MeshWall(40�`49)
	InitMeshWall(XMFLOAT3{ -137.5f,0.0f,0.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);					//�@
	InitMeshWall(XMFLOAT3{ -200.0f,0.0f,-337.5f }, XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//�A
	InitMeshWall(XMFLOAT3{ 0.0f,0.0f,-217.5f }, XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//�B
	InitMeshWall(XMFLOAT3{ -157.5f,0.0f,-100.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);				//�C
	InitMeshWall(XMFLOAT3{ -357.5f,0.0f,-100.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);				//�D
	InitMeshWall(XMFLOAT3{ 242.5f,0.0f,-100.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);				//�E
	InitMeshWall(XMFLOAT3{ 207.5f,0.0f,0.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);					//�F
	InitMeshWall(XMFLOAT3{ 400.0f,0.0f,557.5f }, XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//�G
	InitMeshWall(XMFLOAT3{ 400.0f,0.0f,457.5f }, XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//�H
	InitMeshWall(XMFLOAT3{ -232.5f,0.0f,400.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);					//�I


	//�P�[�W
	SetStageFurniture(CAGe, XMFLOAT3{ 403,0.0f,-320 }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//�P�[�W�����蔻��
	InitMeshWall(XMFLOAT3{ 450.0f,0.0f,-320.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_RAY);					//�I

	//�x�b�h
	SetStageFurniture(BEd, XMFLOAT3{ -150.0f,0.0f,550.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//�g�C��
	SetStageFurniture(TOILEt, XMFLOAT3{ ADJUST_X - 280.0f,0.0f,ADJUST_Z - 50.0f }, XMFLOAT3{ 0.0f,XM_PI*0.5f,0.0f });

	//�e�[�u��
	SetStageFurniture(TABLE, XMFLOAT3{ 250.0f,0.0f,150.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//���ʏ�
	SetStageFurniture(WASHLET, XMFLOAT3{ -400.0f,0.0f,550.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });
	SetStageFurniture(WASHLET, XMFLOAT3{ 370.0f,0.0f,-500.0f }, XMFLOAT3{ 0.0f,-XM_PI*0.5,0.0f });


	//�K�i
	SetStageFurniture(STAIRs, XMFLOAT3{ 450.0f,1.0f, -350.0f }, XMFLOAT3{ 0.0f,XM_PI,0.0f });
	SetStageFurniture(STAIRs, XMFLOAT3{ 450.0f,45.0f,-430.0f }, XMFLOAT3{ 0.0f,XM_PI,0.0f });


}
//=============================================================================
//
// チュートリアル処理 [stage.cpp]
// Author : BURRUEL LEANOS GONZALO
//
//=============================================================================

#include "main.h"
#include "meshwall.h"
#include "meshfield.h"
#include "stage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ADJUST_X				(500.0f)		//メッシュフィールドの座標を合わせるため
#define ADJUST_Z				(600.0f)		
#define MAX_KAGU				(2)				//家具の数

void InitSetStage()
{
	//床
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), (int)(FLOOR_X / FLOOR_BLOCK_SIZE), (int)(FLOOR_Z / FLOOR_BLOCK_SIZE), FLOOR_BLOCK_SIZE, FLOOR_BLOCK_SIZE, 1);

	//壁

	//部屋①
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 225 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 250.0f, WALL_SIZE_Y, WALL_BLACK); //プレイヤーにぶつかってる
	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(237.5f - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(437.5f - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 475 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 50.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(200 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 400 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 162.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);


	//部屋②
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);

	//部屋③
	InitMeshWall(XMFLOAT3(212.5f - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 225.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(450 - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 900 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);

	//部屋④
	InitMeshWall(XMFLOAT3(637.5f - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(825 - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 150.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 900 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);

	//部屋⑤
	InitMeshWall(XMFLOAT3(162.5f - ADJUST_X, 1.0f, 1000 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 1000 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 600.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 1050 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 1200 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(800 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(700 - ADJUST_X, 1.0f, 1150 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 1112.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 25.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(225 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 150.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 1150 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);

	//周辺の壁
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5 - 100.0f - ADJUST_X, 1.0f, 0.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X - 200.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5f - 100.0f - ADJUST_X, 1.0f, FLOOR_Z - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);
}
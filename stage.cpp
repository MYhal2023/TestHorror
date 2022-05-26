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
#include "stagefurniture.h"

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
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 462.5 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(200 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 400 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 162.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);


	//部屋②
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(650 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(837 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(800 - ADJUST_X, 1.0f, 50 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);


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
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 1012.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 25.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 1200 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(800 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(700 - ADJUST_X, 1.0f, 1150 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 1112.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 25.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(225 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 150.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 1100 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);

	//周辺の壁
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5 - 100.0f - ADJUST_X, 1.0f, 0.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X - 200.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5f - 100.0f - ADJUST_X, 1.0f, FLOOR_Z - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, WALL_SIZE_Y, 0.0f - ADJUST_Z), XMFLOAT3(XM_PI*0.5f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), (int)(FLOOR_X / FLOOR_BLOCK_SIZE), (int)(FLOOR_Z / FLOOR_BLOCK_SIZE), WALL_SIZE_XZ, WALL_SIZE_XZ, WALL_GRAY);

	//ドアー
	SetStageFurniture(DOOR, XMFLOAT3{ -140.0f,0.0f,0.0f }, XMFLOAT3{ 0.0f,XM_PI*0.5f,0.0f });		//①
	SetStageFurniture(DOOR, XMFLOAT3{ -200.0f,0.0f,-340.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//②
	SetStageFurniture(DOOR, XMFLOAT3{ 0.0f,0.0f,-210.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });				//③
	SetStageFurniture(DOOR, XMFLOAT3{ -160.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI*0.5f,0.0f });	//④
	SetStageFurniture(DOOR, XMFLOAT3{ -360.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI * 0.5f,0.0f });	//⑤
	SetStageFurniture(DOOR, XMFLOAT3{ 235.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI * 0.5f,0.0f });	//⑥
	SetStageFurniture(DOOR, XMFLOAT3{ 210.0f,0.0f, 0.0f }, XMFLOAT3{ 0.0f,XM_PI * 0.5f,0.0f });		//⑦
	SetStageFurniture(DOOR, XMFLOAT3{ 400.0f,0.0f, 560.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//⑧
	SetStageFurniture(DOOR, XMFLOAT3{ 400.0f,0.0f, 460.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//⑨
	SetStageFurniture(DOOR, XMFLOAT3{ -235.0f,0.0f, 400.0f }, XMFLOAT3{ 0.0f,XM_PI * 0.5f,0.0f });	//⑩

	//ドアーの透明MeshWall
	//InitMeshWall(XMFLOAT3{ -140.0f,0.0f,0.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);		//①

	//ケージ
	SetStageFurniture(CAGe, XMFLOAT3{ 403,0.0f,-320 }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//ベッド
	SetStageFurniture(BEd, XMFLOAT3{ -150.0f,0.0f,550.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//トイレ
	SetStageFurniture(TOILEt, XMFLOAT3{ ADJUST_X - 280.0f,0.0f,ADJUST_Z - 50.0f }, XMFLOAT3{ 0.0f,XM_PI*0.5f,0.0f });

	//テーブル
	SetStageFurniture(TABLE, XMFLOAT3{ 250.0f,0.0f,150.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//洗面所
	SetStageFurniture(WASHLET, XMFLOAT3{ -400.0f,0.0f,550.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });
	SetStageFurniture(WASHLET, XMFLOAT3{ 370.0f,0.0f,-500.0f }, XMFLOAT3{ 0.0f,-XM_PI*0.5,0.0f });


	//階段
	SetStageFurniture(STAIRs, XMFLOAT3{ 450.0f,1.0f, -350.0f }, XMFLOAT3{ 0.0f,XM_PI,0.0f });
	SetStageFurniture(STAIRs, XMFLOAT3{ 450.0f,45.0f,-430.0f }, XMFLOAT3{ 0.0f,XM_PI,0.0f });


}
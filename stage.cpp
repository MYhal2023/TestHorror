//=============================================================================
//
// É`ÉÖÅ[ÉgÉäÉAÉãèàóù [stage.cpp]
// Author : BURRUEL LEANOS GONZALO
//
//=============================================================================

#include "main.h"
#include "meshwall.h"
#include "meshfield.h"
#include "stage.h"
#include "stagefurniture.h"

//*****************************************************************************
// É}ÉNÉçíËã`
//*****************************************************************************
#define	ADJUST_X				(500.0f)		//ÉÅÉbÉVÉÖÉtÉBÅ[ÉãÉhÇÃç¿ïWÇçáÇÌÇπÇÈÇΩÇﬂ
#define ADJUST_Z				(600.0f)		
#define MAX_KAGU				(2)				//â∆ãÔÇÃêî

void InitSetStage()
{
	//è∞
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), (int)(FLOOR_X / FLOOR_BLOCK_SIZE), (int)(FLOOR_Z / FLOOR_BLOCK_SIZE), FLOOR_BLOCK_SIZE, FLOOR_BLOCK_SIZE, 1);

	//ï«

	//ïîâÆá@
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 225 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 250.0f, WALL_SIZE_Y, WALL_BLACK); //ÉvÉåÉCÉÑÅ[Ç…Ç‘Ç¬Ç©Ç¡ÇƒÇÈ
	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(237.5f - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(437.5f - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 462.5 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(200 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 400 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 200.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 162.5f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.5f, WALL_SIZE_Y, WALL_BLACK);


	//ïîâÆáA
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 100 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 300 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(650 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(837 - ADJUST_X, 1.0f, 500 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 125.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(800 - ADJUST_X, 1.0f, 50 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);


	//ïîâÆáB
	InitMeshWall(XMFLOAT3(212.5f - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 225.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(450 - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(100 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(500 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(300 - ADJUST_X, 1.0f, 900 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 400.0f, WALL_SIZE_Y, WALL_BLACK);

	//ïîâÆáC
	InitMeshWall(XMFLOAT3(637.5f - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(825 - ADJUST_X, 1.0f, 600 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 150.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(600 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(900 - ADJUST_X, 1.0f, 750 - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(750 - ADJUST_X, 1.0f, 900 - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 300.0f, WALL_SIZE_Y, WALL_BLACK);
	//25

	//ïîâÆáD
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

	//é¸ï”ÇÃï«
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5 - 100.0f - ADJUST_X, 1.0f, 0.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X - 200.0f - ADJUST_X, 1.0f, FLOOR_Z*0.5f - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, XM_PI*0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1400.0f, WALL_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(FLOOR_X*0.5f - 100.0f - ADJUST_X, 1.0f, FLOOR_Z - 200.0f - ADJUST_Z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 1000.0f, WALL_SIZE_Y, WALL_BLACK);

	InitMeshWall(XMFLOAT3(0.0f - ADJUST_X, WALL_SIZE_Y, 0.0f - ADJUST_Z), XMFLOAT3(XM_PI*0.5f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), (int)(FLOOR_X / FLOOR_BLOCK_SIZE), (int)(FLOOR_Z / FLOOR_BLOCK_SIZE), WALL_SIZE_XZ, WALL_SIZE_XZ, WALL_GRAY);
	//40

	//ÉhÉAÅ[
	SetStageFurniture(DOOR, XMFLOAT3{ -175.0f,0.0f,0.0f }, XMFLOAT3{ 0.0f,XM_PI*0.5f,0.0f });		//á@
	SetStageFurniture(DOOR, XMFLOAT3{ -200.0f,0.0f,-375.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//áA
	SetStageFurniture(DOOR, XMFLOAT3{ 0.0f,0.0f,-250.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });				//áB
	SetStageFurniture(DOOR, XMFLOAT3{ -125.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI*0.5f,0.0f });	//áC
	SetStageFurniture(DOOR, XMFLOAT3{ -317.5f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI * 0.5f,0.0f });	//áD
	SetStageFurniture(DOOR, XMFLOAT3{ 275.0f,0.0f,-100.0f }, XMFLOAT3{ 0.0f,-XM_PI * 0.5f,0.0f });	//áE
	SetStageFurniture(DOOR, XMFLOAT3{ 175.0f,0.0f, 0.0f }, XMFLOAT3{ 0.0f,XM_PI * 0.5f,0.0f });		//áF
	SetStageFurniture(DOOR, XMFLOAT3{ 400.0f,0.0f, 525.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//áG
	SetStageFurniture(DOOR, XMFLOAT3{ 400.0f,0.0f, 425.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });			//áH
	SetStageFurniture(DOOR, XMFLOAT3{ -275.0f,0.0f, 400.0f }, XMFLOAT3{ 0.0f,XM_PI * 0.5f,0.0f });	//áI

	//ÉhÉAÅ[ÇÃìßñæMeshWall(40Å`49)
	InitMeshWall(XMFLOAT3{ -137.5f,0.0f,0.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);					//á@
	InitMeshWall(XMFLOAT3{ -200.0f,0.0f,-337.5f }, XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//áA
	InitMeshWall(XMFLOAT3{ 0.0f,0.0f,-217.5f }, XMFLOAT3(0.0f, XM_PI*0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//áB
	InitMeshWall(XMFLOAT3{ -157.5f,0.0f,-100.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);				//áC
	InitMeshWall(XMFLOAT3{ -357.5f,0.0f,-100.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);				//áD
	InitMeshWall(XMFLOAT3{ 242.5f,0.0f,-100.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);				//áE
	InitMeshWall(XMFLOAT3{ 207.5f,0.0f,0.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);					//áF
	InitMeshWall(XMFLOAT3{ 400.0f,0.0f,557.5f }, XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//áG
	InitMeshWall(XMFLOAT3{ 400.0f,0.0f,457.5f }, XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);			//áH
	InitMeshWall(XMFLOAT3{ -232.5f,0.0f,400.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 75.0f, WALL_SIZE_Y, WALL_RAY);					//áI


	//ÉPÅ[ÉW
	SetStageFurniture(CAGe, XMFLOAT3{ 403,0.0f,-320 }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//ÉPÅ[ÉWìñÇΩÇËîªíË
	InitMeshWall(XMFLOAT3{ 450.0f,0.0f,-320.0f }, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, WALL_SIZE_Y, WALL_RAY);					//áI

	//ÉxÉbÉh
	SetStageFurniture(BEd, XMFLOAT3{ -150.0f,0.0f,550.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//ÉgÉCÉå
	SetStageFurniture(TOILEt, XMFLOAT3{ ADJUST_X - 280.0f,0.0f,ADJUST_Z - 50.0f }, XMFLOAT3{ 0.0f,XM_PI*0.5f,0.0f });

	//ÉeÅ[ÉuÉã
	SetStageFurniture(TABLE, XMFLOAT3{ 250.0f,0.0f,150.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });

	//êÙñ èä
	SetStageFurniture(WASHLET, XMFLOAT3{ -400.0f,0.0f,550.0f }, XMFLOAT3{ 0.0f,0.0f,0.0f });
	SetStageFurniture(WASHLET, XMFLOAT3{ 370.0f,0.0f,-500.0f }, XMFLOAT3{ 0.0f,-XM_PI*0.5,0.0f });


	//äKíi
	SetStageFurniture(STAIRs, XMFLOAT3{ 450.0f,1.0f, -350.0f }, XMFLOAT3{ 0.0f,XM_PI,0.0f });
	SetStageFurniture(STAIRs, XMFLOAT3{ 450.0f,45.0f,-430.0f }, XMFLOAT3{ 0.0f,XM_PI,0.0f });


}
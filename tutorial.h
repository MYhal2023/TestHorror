//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 増田 勇斗
//
//=============================================================================
#pragma once



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FLOOR_BLOCK_SIZE			(100.0f)		// 床ブロックサイズ


#define	TUTORIALWALL_SIZE_Y		(100.0f)	// 横ブロックサイズ縦
#define	TUTORIALWALL_SIZE_XZ	(1000.0f)	// 横ブロックサイズ横
#define	PRISONSIDE_SIZE_XZ		(450.0f)	// 横ブロックサイズ横
#define DOOR_SIZE				(75.0f)		// ドアの横幅
#define STAIRS_SIZE_XZ			(250.0f)	// 階段の横幅
#define ROOM					(4)			// 片側の部屋の数(一部屋増やすごとに4枚メッシュウォール使います)

#define BED_SHIFT_X				(400.0f)	// ベッドを部屋の壁からどれだけ離すか
#define BED_SHIFT_Z				(125.0f)	// ベッドを部屋の壁からどれだけ離すか

#define TOILET_SHIFT_X			(400.0f)	// トイレを部屋の壁からどれだけ離すか
#define TOILET_SHIFT_Z			(-50.0f)	// トイレを部屋の壁からどれだけ離すか

#define STAIRS_SHIFT_Y			(45.0f)		// 奥の階段をどれだけスライドさせるか
#define STAIRS_SHIFT_Z			(50.0f)		// 階段をどれだけスライドさせるか
#define UPSTAIRS_SHIFT_Z		(75.0f)		// 奥の階段をどれだけスライドさせるか

//////////////////1F//////////////////
#define CLEARSTAGEWALL_SIZE_Y		(300.0f)
#define CLEARSTAGEWALL_SIZE_X		(1000.0f)
#define CLEARSTAGEWALL_SIZE_Z		(300.0f)
#define CLEARSTAGE_DOOR				(100.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSetTutorial(void);
void InitSetClearStage(void);
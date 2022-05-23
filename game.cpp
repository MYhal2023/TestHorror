//=============================================================================
//
// ゲーム画面処理 [game.cpp]
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
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF
static int	g_PlayMode = MAIN_GAME;
static int	g_PlayStage = FIRST_STAGE;
//=============================================================================
// 初期化処理
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
	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	InitEnemy();

	InitFurniture();

	InitItem();

	InitMatch();

	InitLighter();

	// スコアの初期化
	InitScore();

	// ライフの初期化
	InitLife();

	//UI表示初期化
	InitInterface();

	InitItembox();

	InitParticle();
	InitGame_over();

}
//ステージ別の初期化処理
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

//デバッグステージの初期化
void InitDebugStage(void)
{
	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), FIELD_X, FIELD_Z, BLOCK_SIZE, BLOCK_SIZE, WATER);
	//マップに使う壁の初期化
	InitFieldMeshWall();
}

//牢屋ステージ初期化(第一ステージ？)
void InitFastStage(void)
{
	//マップに使うメッシュ、オブジェクトの初期化
	InitSetTutorial();
}

void InitSecondStage(void)
{
	InitFurnitureFirStage();

	//マップに使う壁の初期化
	InitSetStage();
}

void InitClearStage(void)
{
	//マップに使うメッシュ、オブジェクトの初期化
	InitSetClearStage();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	UninitParticle();

	UninitEnemy();

	//UI表示終了処理
	UninitInterface();

	UninitItembox();

	UninitMatch();

	UninitLighter();

	// スコアの終了処理
	UninitScore();

	// ライフの終了処理
	UninitLife();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	UninitFurniture();

	UninitStageFurniture();

	UninitItem();

	// 影の終了処理
	UninitShadow();
	UninitGame_over();
}

//=============================================================================
// 更新処理
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
	if (CheckGameover() == TRUE)return;	//ゲームオーバーなら更新しない

	if(g_bPause == FALSE)
		return;
	PLAYER *player = GetPlayer();

	// 地面処理の更新
	UpdateMeshField();

	// 壁処理の更新
	UpdateMeshWall();

	UpdateFurniture();

	UpdateItem();

	// プレイヤーの更新処理
	UpdatePlayer();

	UpdateEnemy();

	UpdateLight();

	//ゲーム内部でのやり取り
	CheckGame();

	UpdateItembox();

	// 影の更新処理
	UpdateShadow();

	UpdateFog();

	UpdateMatch();

	UpdateLighter();

	UpdateParticle();

	// スコアの更新処理
	UpdateScore();

	//UI表示更新処理
	UpdateInterface();

	// ライフの更新処理
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
	if (CheckGameover() == TRUE)return;	//ゲームオーバーなら更新しない

	if (g_bPause == FALSE)
		return;
	PLAYER *player = GetPlayer();

	// 地面処理の更新
	UpdateMeshField();

	// 壁処理の更新
	UpdateMeshWall();

	UpdateStageFurniture();

	UpdateItem();

	// プレイヤーの更新処理
	UpdatePlayer();

	UpdateEnemy();

	UpdateLight();

	//ゲーム内部でのやり取り
	CheckGame();

	UpdateItembox();

	// 影の更新処理
	UpdateShadow();

	UpdateFog();

	UpdateMatch();

	UpdateLighter();

	UpdateParticle();

	// スコアの更新処理
	UpdateScore();

	//UI表示更新処理
	UpdateInterface();

	// ライフの更新処理
	UpdateLife();

	UpdateSound();

	CheckModeChange();

}
//=============================================================================
// 描画処理(カメラ目線)
//=============================================================================
void DrawGame0(void)
{
	PLAYER *player = GetPlayer();
	//シェーダー管理
	//ポストエフェクトをかける場合はここから
	int ans = MODE_PLANE;
	SwapShader(ans);

	DrawGame_over();
	if (CheckGameover() == TRUE)return;	//ゲームオーバーなら更新しない

	// 3Dの物を描画する処理
	// 地面の描画処理
	DrawMeshField();

	DrawFurniture();

	DrawItem();

	// 影の描画処理
	DrawShadow();


	// プレイヤーの描画処理
	DrawPlayer();

	DrawMatch();

	DrawLighter();

	DrawParticle();

	DrawEnemy();

	// 壁の描画処理
	DrawMeshWall();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);


	//// スコアの描画処理
	//DrawScore();

	// ライフの描画処理
	DrawLife();

	DrawItembox();

	////UI表示描画処理
	DrawInterface();

	DrawGame_over();
	//シェーダー管理
	//シェーダーを元に戻す。ポストエフェクトはここまで
	ans = MODE_PLANE;
	SwapShader(ans);

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}

void DrawFirstStageGame(void)
{
	PLAYER *player = GetPlayer();
	//シェーダー管理
	//ポストエフェクトをかける場合はここから
	int ans = MODE_PLANE;
	SwapShader(ans);

	DrawGame_over();
	if (CheckGameover() == TRUE)return;	//ゲームオーバーなら更新しない

	// 3Dの物を描画する処理
	// 地面の描画処理
	DrawMeshField();

	DrawStageFurniture();

	DrawItem();

	// 影の描画処理
	DrawShadow();


	// プレイヤーの描画処理
	DrawPlayer();

	DrawMatch();

	DrawLighter();

	DrawParticle();

	DrawEnemy();

	// 壁の描画処理
	DrawMeshWall();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);


	//// スコアの描画処理
	//DrawScore();

	// ライフの描画処理
	DrawLife();

	DrawItembox();

	////UI表示描画処理
	DrawInterface();

	//シェーダー管理
	//シェーダーを元に戻す。ポストエフェクトはここまで
	ans = MODE_PLANE;
	SwapShader(ans);

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);

}

//=============================================================================
// 描画処理(ライト目線。生成したい影に関するオブジェクトだけを指定)
//=============================================================================
void DrawGame1(void)
{
	// 地面の描画処理
	DrawMeshField();

	// プレイヤーの描画処理
	DrawPlayer();

}


void DrawGame(void)
{


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif
	XMFLOAT3 pos;

	// プレイヤー視点
	pos = GetPlayer()->pos;
	//pos = GetEnemy()->pos;	//デバッグ用
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

		// エネミー視点
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		break;

	case TYPE_LIGHT_SCREEN:	//ライト目線の描画をするためのビューポートタイプ
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

//数値のクランプ。引数1:クランプしたい数値、引数2:最小値、引数3:最大値
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

//float型の数値の比較をする。flagがFALSEなら小さい方を、TRUEなら大きい方を返す
float FloatCompare(BOOL flag, float a, float b)
{
	float ans = 0.0f;//一応0.0fをセットしておく
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

//ライトが付いているか否か。TRUEで付いてる。FALSEで付いてない。
BOOL CheckLightOn(void)
{
	BOOL ans = FALSE;
	MATCH *match = GetMatch();
	LIGHTER *lighter = GetLighter();
	//マッチ燃焼時間が0以上、またはライターが炎を出している時、光がある扱い
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
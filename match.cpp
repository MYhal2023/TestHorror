//=============================================================================
//
// マッチ処理 [match.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "input.h"
#include "match.h"
#include "itembox.h"
#include "debugproc.h"
#include "sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	MODEL_MATCH		"data/MODEL/match.obj"			// 読み込むモデル名


#define TEXTURE_WIDTH				(100)	// テクスチャサイズ
#define TEXTURE_HEIGHT				(100)	// 

#define DISTANCE_X					(25.0f)
#define DISTANCE_Y					(10.0f)
#define DISTANCE_Z					(25.0f)

#define NO_USE_Y					(25.0f)

#define MATCH_SCALE					(0.5f)

#define MATCH_POS_X					(600)		// マッチを表示する場所(X軸)
#define DEFAULTMATCH_POS_Y			(600.0f)	// 通常時のマッチの場所(Y軸)
#define STANDBYMATCH_POS_Y			(500.0f)	// スタンバイ時のマッチの場所(Y軸)
#define STANDBYTIME					(10)		// マッチを構えるまでの時間
#define STANDBY_MOVE_FRAME			((STANDBYMATCH_POS_Y-DEFAULTMATCH_POS_Y)/STANDBYTIME)//構える時の1フレームあたりの移動量
#define MATCH_NUM					(10)			// マッチの本数(初期)

#define MATCH_SLOW					(800)
#define MATCH_MIDDLE				(1200)
#define MATCH_FAST					(500)
//#define	MODEL_???			"data/MODEL/???"		// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報





static MATCH						g_Match;

static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMatch(void)
{
	ID3D11Device *pDevice = GetDevice();

	LoadModel(MODEL_MATCH, &g_Match.model);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// マッチの初期化
	g_Match.Use   = TRUE;

	CAMERA		camera = *GetCamera();

	g_Match.Pos = { camera.pos.x + sinf(camera.rot.y)*DISTANCE_X, camera.pos.y - NO_USE_Y  , camera.pos.z + cosf(camera.rot.y)*DISTANCE_Z };
	g_Match.rot = { camera.rot.x , camera.rot.y + XM_PI ,camera.rot.z + XM_PI*0.15f };
	g_Match.scl = { MATCH_SCALE , MATCH_SCALE , MATCH_SCALE };

	g_Match.Out = FALSE;

	g_Match.TexNo = 0;
	g_Match.StandbyTime = 0;
	g_Match.num = MATCH_NUM;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMatch(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	UnloadModel(&g_Match.model);

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMatch(void)
{

	StandbyMatch();



#ifdef _DEBUG	// デバッグ情報を表示する
	//マッチの燃焼時間を設定
	if (GetKeyboardTrigger(DIK_M))
	{
		g_Match.AblazeTime = 5000;
	}
	if (GetKeyboardTrigger(DIK_N))
	{
		g_Match.AblazeTime = 0;
	}

	PrintDebugProc("Match.num %d\n", g_Match.num);
	PrintDebugProc("Match.AblazeTime %d\n", g_Match.AblazeTime);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMatch(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	if (g_Match.Use == FALSE || g_Match.Pos.y <= GAME_Y_CAM - NO_USE_Y) return;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Match.scl.x, g_Match.scl.y, g_Match.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Match.rot.x, g_Match.rot.y, g_Match.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Match.Pos.x, g_Match.Pos.y, g_Match.Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Match.mtxWorld, mtxWorld);

	// モデル描画
	DrawModel(&g_Match.model);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//
void StandbyMatch(void)
{
	//マッチが燃えている時の処理
	int oldBtime = g_Match.AblazeTime;				//過去の残燃焼時間を保存
	if (g_Match.AblazeTime > 0)g_Match.AblazeTime--;//燃焼時間が残ってるなら減算
	int nowBtime = g_Match.AblazeTime;				//減算後の残燃焼時間を保存
	//燃え尽きたらマッチのスタンバイ時間をリセットする
	if (nowBtime <= 0 && oldBtime > 0) 	//過去残が1以上、現在残が0以下→ここが燃え尽きたタイミング！
	{
		g_Match.StandbyTime = 0;
		g_Match.Out = FALSE;
	}
	MoveMatch();

	//マッチが燃えてないならここで処理を終わる
	if (g_Match.AblazeTime > 0)return;
	
	//マッチが燃えていない時の処理
	//マッチを構える条件
	if (!GetCameraPos())return;
	if (IsButtonPressed(0, BUTTON_L)&&(0 < g_Match.num) && ReturnSelect() == MATCH_ITEM)
	{
		g_Match.Out = TRUE;
		//構えきるまでの時間
		if (g_Match.StandbyTime < STANDBYTIME)
		{
			g_Match.StandbyTime++;
			return;
		}

		if(IsButtonPressed(0, BUTTON_R)) SetForceState(TRUE);
		else SetForceState(FALSE);

		switch (IsButtonForce(0))
		{
		case FORCE_NON:
			break;
		case FORCE_SLOW:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_SLOW;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		case FORCE_MIDDLE:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_MIDDLE;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		case FORCE_FAST:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_FAST;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		}
		InitForce(0);
	}
	else
	{
		//おろし終わるまでの時間
		if (0 < g_Match.StandbyTime)
		{
			g_Match.StandbyTime--;
			return;
		}
	}

}

void MoveMatch()
{
	CAMERA		camera = *GetCamera();
	g_Match.Pos.x = camera.pos.x + sinf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_X;
	g_Match.Pos.z = camera.pos.z + cosf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_Z;
	if (g_Match.Out == FALSE && g_Match.Pos.y > camera.pos.y - NO_USE_Y)
	{
		g_Match.Pos.y -= MATCH_MOVE;
	}

	if (g_Match.Out == TRUE && g_Match.Pos.y < camera.pos.y - DISTANCE_Y)
	{
		g_Match.Pos.y += MATCH_MOVE;
	}

	g_Match.rot.y = camera.rot.y + XM_PI * 0.5f;
}


void SetMatchForce(int force)
{
	g_Match.force = force;
}

MATCH *GetMatch(void)
{
	return &g_Match;
}
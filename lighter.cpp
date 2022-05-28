//=============================================================================
//
// ライター処理 [lighter.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "model.h"
#include "interface.h"
#include "lighter.h"
#include "sprite.h"
#include "sound.h"
#include "player.h"
#include "camera.h"
#include "time.h"
#include "game.h"
#include "itembox.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_LIGHTER		"data/MODEL/lighter.obj"			// 読み込むモデル名

#define DISTANCE_X					(25.0f)
#define DISTANCE_Y					(20.0f)
#define DISTANCE_Z					(25.0f)

#define NO_USE_Y						(25.0f)

#define LIGHTER_SCALE						(0.3f)

#define TEXTURE_WIDTH				(160)	// テクスチャサイズ
#define TEXTURE_HEIGHT				(200)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数

#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define LIGHTER_POS_X	(SCREEN_CENTER_X + 250)
#define LIGHTER_POS_Y	(SCREEN_CENTER_Y + 100)
//#define	MODEL_???			"data/MODEL/???"		// 読み込むモデル名



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static int						g_TexNo;					// テクスチャ番号
static LIGHTER					g_Lighter;					

static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLighter(void)
{
	ID3D11Device *pDevice = GetDevice();

	LoadModel(MODEL_LIGHTER, &g_Lighter.model);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 初期化
	CAMERA		camera = *GetCamera();

	g_Lighter.pos = { camera.pos.x + sinf(camera.rot.y)*DISTANCE_X, camera.pos.y - NO_USE_Y  , camera.pos.z + cosf(camera.rot.y)*DISTANCE_Z };
	g_Lighter.rot = { camera.rot.x , camera.rot.y + XM_PI*0.5f ,camera.rot.z };
	g_Lighter.scl = { LIGHTER_SCALE , LIGHTER_SCALE , LIGHTER_SCALE};
	g_TexNo = 0;

	g_Lighter.use = TRUE;
	g_Lighter.oil = LIGHTER_OIL_MAX;
	g_Lighter.out = FALSE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLighter(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	UnloadModel(&g_Lighter.model);

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLighter(void)
{


#ifdef _DEBUG	// デバッグ情報を表示する

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	SetLighterOn(TRUE);
	//}
	//if (GetKeyboardTrigger(DIK_J))
	//{
	//	SetLighterOff();
	//}
	if (GetKeyboardTrigger(DIK_I))
	{
		AddOil(10.0f);
	}
#endif

	CheckOil();
	MoveLighter();
	if (!g_Lighter.use || !g_Lighter.out)
		return;
	ReduceOil();

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	PrintDebugProc("残るオイル量：%f / 100", g_Lighter.oil);
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLighter(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	if (g_Lighter.use == FALSE || g_Lighter.pos.y <= GAME_Y_CAM - NO_USE_Y) return;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Lighter.scl.x, g_Lighter.scl.y, g_Lighter.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Lighter.rot.x, g_Lighter.rot.y, g_Lighter.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Lighter.pos.x, g_Lighter.pos.y, g_Lighter.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Lighter.mtxWorld, mtxWorld);

	// モデル描画
	DrawModel(&g_Lighter.model);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}



//=============================================================================
// ライターON
//=============================================================================

void SetLighterOn(BOOL flag)
{
	//CheckOil();
	if (!g_Lighter.use)		return;
	if(g_Lighter.out == FALSE && flag == TRUE)PlaySound(SOUND_LABEL_SE_LighterON);		//

	g_Lighter.out = flag;
}

//=============================================================================
// ライターOFF
//=============================================================================
void SetLighterOff()
{
	if (!g_Lighter.use)	return;
	g_Lighter.out = FALSE;
}

//=============================================================================
// オイル量を追加
//=============================================================================
void AddOil(float n)
{
	if (g_Lighter.oil + n > LIGHTER_OIL_MAX)
	{
		g_Lighter.oil = LIGHTER_OIL_MAX;
	}
	else
	{
		g_Lighter.oil += n;

	}
	if (g_Lighter.use == FALSE)
	{
		g_Lighter.use = TRUE;
	}
}

//=============================================================================
// オイル量チェック
//=============================================================================
void CheckOil()
{
	if (g_Lighter.oil >= LIGHTER_OIL_MIN || g_Lighter.use == FALSE)
		return;
	SetLighterOff();
	g_Lighter.use = FALSE;
}

//=============================================================================
// オイル量減らす
//=============================================================================
void ReduceOil()
{
	g_Lighter.oil -= LIGHTER_OIL_REDUCE;
}

LIGHTER *GetLighter()
{
	return &g_Lighter;
}

void MoveLighter()
{
	CAMERA		camera = *GetCamera();
	g_Lighter.pos.x = camera.pos.x + sinf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_X;
	g_Lighter.pos.z = camera.pos.z + cosf(camera.rot.y + XM_PI * 0.1f)*DISTANCE_Z;
	if (g_Lighter.out == FALSE && g_Lighter.pos.y > camera.pos.y - NO_USE_Y)
	{
		g_Lighter.pos.y -= LIGHTER_MOVE;
	}

	if (g_Lighter.out == TRUE && g_Lighter.pos.y < camera.pos.y - DISTANCE_Y)
	{
		g_Lighter.pos.y += LIGHTER_MOVE;
	}

	g_Lighter.rot.y = camera.rot.y + XM_PI * 0.5f;


}
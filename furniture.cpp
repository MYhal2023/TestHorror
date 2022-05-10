//=============================================================================
//
// オブジェクトモデル処理 [furniture.cpp]
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "game.h"
#include "furniture.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MODEL_???			"data/MODEL/???"		// 読み込むモデル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static FURNITURE g_Furniture[MAX_FURNITURE];

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitFurniture(void)
{
	for (int i = 0; i < MAX_FURNITURE; i++)
	{
	}
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitFurniture(void)
{

}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateFurniture(void)
{

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawFurniture(void)
{
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// カリング無効
	//SetCullingMode(CULL_MODE_NONE);

	//for (int i = 0; i < MAX_FURNITURE; i++)
	//{
	//	if (g_Furniture[i].use == false) continue;

	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// スケールを反映
	//	mtxScl = XMMatrixScaling(g_Furniture[i].scl.x, g_Furniture[i].scl.y, g_Furniture[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Furniture[i].rot.x, g_Furniture[i].rot.y, g_Furniture[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(g_Furniture[i].pos.x, g_Furniture[i].pos.y, g_Furniture[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックスの設定
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_Furniture[i].mtxWorld, mtxWorld);


	//	// モデル描画
	//	DrawModel(&g_Furniture[i].model);
	//}

	//// カリング設定を戻す
	//SetCullingMode(CULL_MODE_BACK);

}
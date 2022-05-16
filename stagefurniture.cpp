//=============================================================================
//
// ステージオブジェクトモデル処理 [furniture.cpp]
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "game.h"
#include "stagefurniture.h"
#include "furniture.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MODEL_???			"data/MODEL/???"		// 読み込むモデル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static FURNITURE g_StageFurniture[MAX_FURNITURE];

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitFurnitureFirStage(void)
{
	for (int i = 0; i < MAX_STAGEFURNITURE; i++)
	{
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitStageFurniture(void)
{

}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateStageFurniture(void)
{

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawStageFurniture(void)
{
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// カリング無効
	//SetCullingMode(CULL_MODE_NONE);

	//for (int i = 0; i < MAX_FURNITURE; i++)
	//{
	//	if (g_StageFurniture[i].use == false) continue;

	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// スケールを反映
	//	mtxScl = XMMatrixScaling(g_StageFurniture[i].scl.x, g_StageFurniture[i].scl.y, g_StageFurniture[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_StageFurniture[i].rot.x, g_StageFurniture[i].rot.y, g_StageFurniture[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(g_StageFurniture[i].pos.x, g_StageFurniture[i].pos.y, g_StageFurniture[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックスの設定
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_StageFurniture[i].mtxWorld, mtxWorld);


	//	// モデル描画
	//	DrawModel(&g_StageFurniture[i].model);
	//}

	//// カリング設定を戻す
	//SetCullingMode(CULL_MODE_BACK);

}
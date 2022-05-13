//=============================================================================
//
// アイテムモデル処理 [item.cpp]
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "game.h"
#include "item.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MODEL_???			"data/MODEL/???"		// 読み込むモデル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static MODEL_ITEM g_Item[MAX_ITEM];

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
	}
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitItem(void)
{

}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateItem(void)
{

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawItem(void)
{
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// カリング無効
	//SetCullingMode(CULL_MODE_NONE);

	//for (int i = 0; i < MAX_ITEM; i++)
	//{
	//	if (g_Item[i].use == false) continue;

	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// スケールを反映
	//	mtxScl = XMMatrixScaling(g_Item[i].scl.x, g_Item[i].scl.y, g_Item[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Item[i].rot.x, g_Item[i].rot.y, g_Item[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(g_Item[i].pos.x, g_Item[i].pos.y, g_Item[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックスの設定
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_Item[i].mtxWorld, mtxWorld);


	//	// モデル描画
	//	DrawModel(&g_Item[i].model);
	//}

	//// カリング設定を戻す
	//SetCullingMode(CULL_MODE_BACK);

}
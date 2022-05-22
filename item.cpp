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
#include "player.h"
#include "collision.h"
#include "itembox.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_KEY			"data/MODEL/key.obj"		// 読み込むモデル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MODEL_ITEM		g_Item[MAX_ITEM];
static BOOL				g_Load = FALSE;

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		//まずはアイテムをタイプ分け
		switch (i)
		{
		case 0:
			g_Item[i].type = ITEM_KEY;
			g_Item[i].pos = { 0.0f, 20.0f, 150.0f };				// モデルの位置
			g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
			g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// モデルの大きさ(スケール)
			g_Item[i].size = SIZE_KEY;							// 当たり判定の大きさ
			break;
		case 1:
			g_Item[i].type = ITEM_KEY;
			g_Item[i].pos = { 0.0f, 20.0f, 50.0f };				// モデルの位置
			g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
			g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// モデルの大きさ(スケール)
			g_Item[i].size = SIZE_KEY;							// 当たり判定の大きさ
			break;
		}
		//アイテムタイプに基づいたモデルの紐づけ
		switch (g_Item[i].type)
		{
		case ITEM_KEY:
			LoadModel(MODEL_KEY, &g_Item[i].model);
			break;
		case MAXTYPE_ITEM:
			break;
		}
		g_Item[i].load = TRUE;
		g_Item[i].use = TRUE;
		g_Item[i].drop = TRUE;
		g_Item[i].ID = i;

		GetModelDiffuse(&g_Item[i].model, &g_Item[i].diffuse[0]);
	}
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitItem(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_Item[i].load)
		{
			UnloadModel(&g_Item[i].model);
			g_Item[i].load = FALSE;
		}
	}

	g_Load = FALSE;

}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item)continue;

	}

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawItem(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].use || !g_Item[i].drop) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Item[i].scl.x, g_Item[i].scl.y, g_Item[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Item[i].rot.x, g_Item[i].rot.y, g_Item[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Item[i].pos.x, g_Item[i].pos.y, g_Item[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Item[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Item[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);

}

//落ちているアイテムとの当たり判定
void CollisionItem(XMFLOAT3 pos, float size)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].use || !g_Item[i].drop)continue;
		if (!CollisionBC(pos, g_Item[i].pos, size, g_Item[i].size))continue;

		switch (g_Item[i].type)
		{
		case ITEM_KEY:
			AddItembox(KEY);
			g_Item[i].drop = FALSE;
			PlaySound(SOUND_LABEL_SE_GetItem);
			break;
		case MAXTYPE_ITEM:
			break;
		}
	}
}
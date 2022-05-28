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
#include "text_texture.h"
#include "furniture.h"
#include "enemy.h"
#include "match.h"
#include "stagefurniture.h"
#include "tutorial_text.h"
#include "meshwall.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_KEY			"data/MODEL/key.obj"		// 読み込むモデル名
#define	MODEL_MATCH			"data/MODEL/matchibox.obj"		// 読み込むモデル名
#define	MODEL_LIGHTER		"data/MODEL/lighter.obj"		// 読み込むモデル名

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
	//まずは全配列を初期化
	for(int i = 0; i < MAX_ITEM; i++)
	{
		g_Item[i].pos = {0.0f, 0.0f, 0.0f};				// モデルの位置
		g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
		g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// モデルの大きさ(スケール)
		g_Item[i].use = FALSE;
		g_Item[i].load = FALSE;
		g_Item[i].drop = TRUE;				//アイテムが取得状態か(TRUEで落ちている、FALSEで拾われている)
		g_Item[i].size = 0.0f;				// 当たり判定の大きさ
		g_Item[i].type = NULL;				//アイテムの種類
		g_Item[i].ID = 99;					//鍵のみに使用。このIDとドアのIDが一致していれば使用可能になる

	}

	switch (GetPlayStage())
	{
	case PRISON_STAGE:
		for (int i = 0; i < MAX_ITEM; i++)
		{
			//まずはアイテムをタイプ分け
			switch (i)
			{
			case 0:
				g_Item[i].type = ITEM_KEY;								//1
				g_Item[i].pos = { 495.0f, 40.0f, -410.0f };				// モデルの位置
				g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 0.5f, 0.5f, 0.5f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_KEY;							// 当たり判定の大きさ
				g_Item[i].ID = i;
				break;
			case 1:
				g_Item[i].type = ITEM_KEY;							//2
				g_Item[i].pos = { -500.0f, 40.0f, -400.0f };				// モデルの位置
				g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 0.5f, 0.5f,0.5f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_KEY;							// 当たり判定の大きさ
				g_Item[i].ID = i;
				break;
			case 2:
				g_Item[i].type = ITEM_MATCH;
				g_Item[i].pos = { -445.0f, 25.0f, -465.0f };				// モデルの位置
				g_Item[i].rot = { XM_PI * 0.0f, 0.0f, XM_PI * 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 0.2f, 0.2f, 0.2f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_MATCH;							// 当たり判定の大きさ
				break;
			case 3:
				g_Item[i].type = ITEM_LIGHTER;
				g_Item[i].pos = { 415.0f, 22.0f, -460.0f };				// モデルの位置
				g_Item[i].rot = { XM_PI * 0.0f,  XM_PI * 0.25f, XM_PI * 0.5f };				// モデルの向き(回転)
				g_Item[i].scl = { 0.3f, 0.3f, 0.3f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_LIGHTER;							// 当たり判定の大きさ
				break;
			}
			//アイテムタイプに基づいたモデルの紐づけ
			switch (g_Item[i].type)
			{
			case ITEM_KEY:
				LoadModel(MODEL_KEY, &g_Item[i].model);
				break;
			case ITEM_MATCH:
				LoadModel(MODEL_MATCH, &g_Item[i].model);
				break;
			case ITEM_LIGHTER:
				LoadModel(MODEL_LIGHTER, &g_Item[i].model);
				break;

			case MAXTYPE_ITEM:
				break;
			}
			g_Item[i].load = TRUE;
			g_Item[i].use = TRUE;
			g_Item[i].drop = TRUE;

			GetModelDiffuse(&g_Item[i].model, &g_Item[i].diffuse[0]);
		}
		break;
	case FIRST_STAGE:
		for (int i = 0; i < MAX_ITEM; i++)
		{
			g_Item[i].ID = 0;
			//まずはアイテムをタイプ分け
			switch (i)
			{
			case 0:
				g_Item[i].type = ITEM_KEY;
				g_Item[i].pos = { -300.0f, 20.0f, -450.0f };				// モデルの位置
				g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_KEY;							// 当たり判定の大きさ
				g_Item[i].ID = 100;
				break;
			case 1:
				g_Item[i].type = ITEM_KEY;
				g_Item[i].pos = { 220.0f, 20.0f, 550.0f };				// モデルの位置
				g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_KEY;							// 当たり判定の大きさ
				g_Item[i].ID = 1;
				break;
			case 2:
				g_Item[i].type = ITEM_MATCH;
				g_Item[i].pos = { -100.0f, 5.0f, 200.0f };				// モデルの位置
				g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 0.2f, 0.2f, 0.2f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_MATCH;							// 当たり判定の大きさ
				break;
			case 3:
				g_Item[i].type = ITEM_MATCH;
				g_Item[i].pos = { 100.0f, 5.0f, 500.0f };				// モデルの位置
				g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// モデルの向き(回転)
				g_Item[i].scl = { 0.2f, 0.2f, 0.2f };				// モデルの大きさ(スケール)
				g_Item[i].size = SIZE_MATCH;							// 当たり判定の大きさ
				break;
			}
			//アイテムタイプに基づいたモデルの紐づけ
			switch (g_Item[i].type)
			{
			case ITEM_KEY:
				LoadModel(MODEL_KEY, &g_Item[i].model);
				break;
			case ITEM_MATCH:
				LoadModel(MODEL_MATCH, &g_Item[i].model);
				break;
			case ITEM_LIGHTER:
				LoadModel(MODEL_LIGHTER, &g_Item[i].model);
				break;

			case MAXTYPE_ITEM:
				break;
			}
			g_Item[i].load = TRUE;
			g_Item[i].use = TRUE;
			g_Item[i].drop = TRUE;

			GetModelDiffuse(&g_Item[i].model, &g_Item[i].diffuse[0]);
		}
		break;
	case 	CLEAR_STAGE:
	case	MAX_STAGE:
	default:
		for (int i = 0; i < MAX_ITEM; i++)
		{
			if (g_Item[i].use == TRUE)
			{
				g_Item[i].use = FALSE;
			}
		}
		break;
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
	MATCH *match = GetMatch();
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].use || !g_Item[i].drop)continue;
		if (!CollisionBC(pos, g_Item[i].pos, size, g_Item[i].size))continue;

		switch (g_Item[i].type)
		{
		case ITEM_KEY:
			AddItembox(KEY);
			g_Item[i].drop = FALSE;
			SetTexture(TEXT_TAKE_KEY);
			PlaySound(SOUND_LABEL_SE_GetItem);
			break;
		case ITEM_MATCH:
			match->num++;
			g_Item[i].drop = FALSE;
			SetTexture(TEXT_TAKE_MATCH);
			if (GetPlayStage() == PRISON_STAGE)
				SetTutorialTexture(TUTORIAL_MATCHI);
			PlaySound(SOUND_LABEL_SE_GetItem);
			break;
		case ITEM_LIGHTER:
			AddItembox(LIGHTER_ITEM);
			g_Item[i].drop = FALSE;
			SetTexture(TEXT_TAKE_LIGHTER);
			if (GetPlayStage() == PRISON_STAGE)
				SetTutorialTexture(TUTORIAL_LIGHTER);
			PlaySound(SOUND_LABEL_SE_GetItem);
			break;
		case MAXTYPE_ITEM:
			break;
		}
	}
}

//プレイヤーがドアに近づいた時に使う関数。開ける時にTRUE、そうでない時はFALSEを返す
//第一引数にはドアのIDを取る
BOOL CheckDoorKey(int i)
{
	FURNITURE *fur = GetFurniture();
	STAGEFURNITURE *sfur = GetStageFurniture();
	BOOL ans = FALSE;
	switch (GetPlayStage())
	{
	case PRISON_STAGE:
		for (int k = 0; k < MAX_ITEM; k++)
		{
			if (g_Item[k].type != ITEM_KEY || g_Item[k].drop == TRUE)continue;

			if (fur[i].ID == g_Item[k].ID)ans = TRUE;
			if (g_Item[k].ID == 0) {
				SetEnemy(XMFLOAT3(0.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
				DeleteMeshWall(10);
			}
			else if (g_Item[k].ID == 1)
			{
				DeleteMeshWall(11);
			}
		}
		break;
	case FIRST_STAGE:
		for (int k = 0; k < MAX_ITEM; k++)
		{
			if (g_Item[k].type != ITEM_KEY || g_Item[k].drop == TRUE)continue;

			if (i == g_Item[k].ID)ans = TRUE;
		}
		break;
	}
	return ans;
}

MODEL_ITEM *GetItem(void)
{
	return &g_Item[0];
}
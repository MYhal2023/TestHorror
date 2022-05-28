#pragma once
//=============================================================================
//
// アイテムモデル処理 [item.h]
// Author : 
//
//=============================================================================

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ITEM (4)	//使うアイテムの数
#define SIZE_KEY (15)	//鍵の当たり判定の大きさ
#define SIZE_MATCH (25)	//鍵の当たり判定の大きさ
#define SIZE_LIGHTER (25)	//鍵の当たり判定の大きさ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	BOOL				use;
	BOOL				load;
	BOOL				drop;				//アイテムが取得状態か(TRUEで落ちている、FALSEで拾われている)
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	float				size;				// 当たり判定の大きさ
	int					type;				//アイテムの種類
	int					ID;					//鍵のみに使用。このIDとドアのIDが一致していれば使用可能になる
} MODEL_ITEM;

enum ITEM_TYPE
{
	ITEM_KEY,
	ITEM_MATCH,
	ITEM_LIGHTER,
	MAXTYPE_ITEM
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void CollisionItem(XMFLOAT3 pos, float size);
BOOL CheckDoorKey(int i);
MODEL_ITEM *GetItem(void);
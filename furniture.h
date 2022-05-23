#pragma once
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ROOM_FURNITURE (2)		//1部屋の家具の数
#define UNIQUE_FURNITURE (5)	//コピー出来ない家具の数
#define MAX_FURNITURE (ROOM_FURNITURE*(ROOM*2)+UNIQUE_FURNITURE)	//使うオブジェクトの数

#define BED_SCL		(1.5f)
#define TOILET_SCL	(5.0f)
#define CANDLE_SCL	(3.25f)
#define STAIRS_SCL	(2.0f)
#define CAGE_SCL	(1.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	bool				use;
	bool				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	float				size;				// 当たり判定の大きさ
	BOOL				open;
} FURNITURE;

enum
{
	BED,
	TOILET,
	CANDLE,
	STAIRS,
	CAGE
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFurniture(void);
void UninitFurniture(void);
void UpdateFurniture(void);
void DrawFurniture(void);
void SetFurniture(XMFLOAT3 pos, XMFLOAT3 rot, int type);
FURNITURE *GetFurniture(void);

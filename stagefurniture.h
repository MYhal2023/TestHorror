#pragma once
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAGEFURNITURE (0)	//使うオブジェクトの数

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
} STAGEFURNITURE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFurnitureFirStage(void);
void UninitStageFurniture(void);
void UpdateStageFurniture(void);
void DrawStageFurniture(void);
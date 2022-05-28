#pragma once
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAGEFURNITURE (20)	//使うオブジェクトの数



enum STAGEMODELS
{
	DOOR,
	TOILEt,
	WASHLET,
	STAIRs,
	TABLE,
	CAGe,
	BEd,
};

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
	int					ID;
} STAGEFURNITURE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFurnitureFirStage(void);
void UninitStageFurniture(void);
void UpdateStageFurniture(void);
void DrawStageFurniture(void);
void SetStageFurniture(int type, XMFLOAT3 pos, XMFLOAT3 rot);
STAGEFURNITURE *GetStageFurniture(void);
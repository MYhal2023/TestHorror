//=============================================================================
//
// スコア処理 [lighter.h]
// Author : ゴンサロ
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIGHTER_OIL_MAX			(100.0f)		// スコアの最大値
#define LIGHTER_OIL_MIN			(1.0f)			// スコアの最小値
#define LIGHTER_OIL_DIGIT		(2)				// 桁数
#define LIGHTER_TIME			(1)			//減らすため、フレームの数	
#define LIGHTER_OIL_REDUCE		(0.025f)			//減らせる量
#define LIGHTER_MOVE			(0.8f)			//移動速さ
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct LIGHTER
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	
	BOOL		use;	//出せれる
	BOOL		out;		//出している
	float		oil;		//オイル
	float		w, h;		// 幅と高さ

};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLighter(void);
void UninitLighter(void);
void UpdateLighter(void);
void DrawLighter(void);

void SetLighterOn(BOOL flag);
void SetLighterOff(void);
void AddOil(float n);
void CheckOil(void);
void ReduceOil(void);
void MoveLighter(void);

LIGHTER *GetLighter(void);
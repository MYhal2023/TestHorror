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
#define LIGHTER_OIL_REDUCE		(0.1f)			//減らせる量

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct LIGHTER
{
	BOOL		use;		//出せれる
	BOOL		out;		//出している
	float		oil;		//オイル
	float		w, h;		// 幅と高さ

	XMFLOAT3	pos;		// ポリゴンの位置
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

LIGHTER *GetLighter(void);
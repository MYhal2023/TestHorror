//=============================================================================
//
// マッチ処理 [match.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX			(99999)		// スコアの最大値
#define SCORE_MIN			(0)			// スコアの最小値
#define SCORE_DIGIT			(7)			// 桁数
#define SCORE_SAVE			(10)		// スコアの保存数

#define MATCH_MOVE			(0.8f)		//マッチの速さ
#define	MODEL_FIRE	"data/MODEL/fire.obj"			// 読み込むモデル名

struct MATCH
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	BOOL					Use, Out;					// TRUE:使っている  FALSE:未使用
	float					w, h;					// 幅と高さ
	XMFLOAT3				Pos;					// ポリゴンの座標
	int						TexNo;					// テクスチャ番号
	int						StandbyTime;			// 構え始めてからの時間
	int						AblazeTime;				// 燃焼時間
	int						force;					// マッチを擦った速さ
	int						num;					// マッチの本数

};

struct MATCHBOX
{
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	XMFLOAT3			pos;					// ポリゴンの座標
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	BOOL				use;
};
struct FIRE
{
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
	XMFLOAT3			pos;					// ポリゴンの座標
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	BOOL				use;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMatch(void);
void UninitMatch(void);
void UpdateMatch(void);
void DrawMatch(void);
void DrawMatchStick(void);
void DrawMatchbox(void);
void DrawFire(void);
void MoveMatch(void);
void MoveMatchbox(void);
void StandbyMatch(void);
void SetMatchForce(int force);
MATCH *GetMatch(void);

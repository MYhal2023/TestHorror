//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY		(4)	// 敵最大数

#define	ENEMY_WH		(5.0f)	// 当たり判定の大きさ

enum ENEMT_TEXTURE
{
	ENEMY_WOMAN,
	ENEMY_DEBUG,
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ワールドマトリックス
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		spd;			// 移動量
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	int			shadowIdx;		// 影ID
	BOOL		use;			// 使用しているかどうか
	int			texNo;

} ENEMY;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot);

ENEMY *GetEnemy(void);


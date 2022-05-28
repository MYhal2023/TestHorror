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
	ENEMY_BACK,
};

enum ENEMY_STATE
{
	Patrol,
	Chase,
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
	XMVECTOR	moveVec;		//移動ベクトル
	MATERIAL	material;		// マテリアル
	float		spd;			// 移動量
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	int			shadowIdx;		// 影ID
	int			texNo;
	int			state;			//ステートベースAIに使用
	int			memory;
	BOOL		use;			// 使用しているかどうか

	INTERPOLATION_DATA	*tbl_adr;			// アニメデータのテーブル先頭アドレス
	int					tbl_size;			// 登録したテーブルのレコード総数
	float				move_time;			// 実行時間

} ENEMY;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot);
void ResetEnemy(void);
ENEMY *GetEnemy(void);
void EnemyAngleTex(int i);
int StateCheck(int i);
void EnemyInterPoration(int i);
void StateAdjust(int i);
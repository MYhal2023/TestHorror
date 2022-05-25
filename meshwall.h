//=============================================================================
//
// メッシュ壁の処理 [meshwall.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MESH_WALL		(50)					// 壁の総数

#define	WAVE_POS_Y			(-20.0f)	// 波が発生している為、壁を少し下げる
#define	WALL_A_POS_XZ		(500.0f)	// 壁Aの座標
#define	WALL_B_POS_XZ		(100.0f)	// 壁Bの座標

#define	WALL_SHIFT			(200.0f)	// 壁を中心からどれだけずらすか
#define	WALL_Y				(1)	// メッシュ壁の縦枚数
#define	WALL_XZ				(1)	// メッシュ壁の横枚数
#define	WALL_BLOCK_SIZE_Y	(360)	// ブロックサイズ縦
#define	WALL_BLOCK_SIZE_XZ	(FIELD_X_LIMIT * 2.0f)	// ブロックサイズ横
#define	WALL_A_SIZE_XZ		(WALL_A_POS_XZ * 2.0f)	// ブロックサイズ横
#define	WALL_B_SIZE_XZ		(WALL_B_POS_XZ * 2.0f)	// ブロックサイズ横

enum WALL_TEX
{
	WALL_BLACK,
	WALL_WHITE,
	WALL_GRAY,
	WALL_RAY,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY, int texNo);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void MeshWallHit(XMFLOAT3 pos, float size, float old_x, float old_z);
void InitFieldMeshWall(void);
XMFLOAT3 GetMeshWallStPosition(int i);
XMFLOAT3 GetMeshWallEdPosition(int i);
int GetMeshWallNum(void);
int GetCeilingWallNum(void);
void ResetMeshWall(void);
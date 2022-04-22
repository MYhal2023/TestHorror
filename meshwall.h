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
#define	MAX_MESH_WALL		(10)					// 壁の総数

enum WALL_TEX
{
	WALL_BLACK,
	WALL_WHITE,
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
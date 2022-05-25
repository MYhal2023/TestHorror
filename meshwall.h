//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_MESH_WALL		(50)					// �ǂ̑���

#define	WAVE_POS_Y			(-20.0f)	// �g���������Ă���ׁA�ǂ�����������
#define	WALL_A_POS_XZ		(500.0f)	// ��A�̍��W
#define	WALL_B_POS_XZ		(100.0f)	// ��B�̍��W

#define	WALL_SHIFT			(200.0f)	// �ǂ𒆐S����ǂꂾ�����炷��
#define	WALL_Y				(1)	// ���b�V���ǂ̏c����
#define	WALL_XZ				(1)	// ���b�V���ǂ̉�����
#define	WALL_BLOCK_SIZE_Y	(360)	// �u���b�N�T�C�Y�c
#define	WALL_BLOCK_SIZE_XZ	(FIELD_X_LIMIT * 2.0f)	// �u���b�N�T�C�Y��
#define	WALL_A_SIZE_XZ		(WALL_A_POS_XZ * 2.0f)	// �u���b�N�T�C�Y��
#define	WALL_B_SIZE_XZ		(WALL_B_POS_XZ * 2.0f)	// �u���b�N�T�C�Y��

enum WALL_TEX
{
	WALL_BLACK,
	WALL_WHITE,
	WALL_GRAY,
	WALL_RAY,
};

//*****************************************************************************
// �v���g�^�C�v�錾
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
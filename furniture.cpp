//=============================================================================
//
// �I�u�W�F�N�g���f������ [furniture.cpp]
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "furniture.h"
#include "stagefurniture.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BED			"data/MODEL/bed.obj"		//�x�b�h�̃��f��
#define	MODEL_TOILET		"data/MODEL/toilet.obj"		//�g�C���̃��f��
#define	MODEL_CANDLE		"data/MODEL/Candlestick.obj"//�C��̃��f��
#define	MODEL_STAIRS		"data/MODEL/stairs.obj"		//�K�i�̃��f��
#define	MODEL_CAGE			"data/MODEL/cage.obj"		//�P�[�W�̃��f��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static FURNITURE g_Furniture[MAX_FURNITURE];
static int obj;
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitFurniture(void)
{
	obj = 0;
	for (int i = 0; i < MAX_FURNITURE; i++)
	{
		g_Furniture[i].pos = { 0.0f,0.0f,0.0f };
		g_Furniture[i].rot = { 0.0f,0.0f,0.0f };
		g_Furniture[i].scl = { 0.0f,0.0f,0.0f };
		g_Furniture[i].use = FALSE;
		g_Furniture[i].load = FALSE;
		g_Furniture[i].size = 0;
		g_Furniture[i].open = FALSE;
	}
	return S_OK;
}


//*****************************************************************************
// �I������
//*****************************************************************************
void UninitFurniture(void)
{


	for (int i = 0; i < MAX_FURNITURE; i++)
	{
		if (g_Furniture[i].load)
		{
			UnloadModel(&g_Furniture[i].model);
			g_Furniture[i].load = FALSE;
		}
	}
	obj = 0;
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateFurniture(void)
{

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawFurniture(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_FURNITURE; i++)
	{
		if (g_Furniture[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Furniture[i].scl.x, g_Furniture[i].scl.y, g_Furniture[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Furniture[i].rot.x, g_Furniture[i].rot.y, g_Furniture[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Furniture[i].pos.x, g_Furniture[i].pos.y, g_Furniture[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Furniture[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Furniture[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}

void SetFurniture(XMFLOAT3 pos, XMFLOAT3 rot, int type)
{
	g_Furniture[obj].pos = pos;
	g_Furniture[obj].rot = rot;

	switch (type)
	{
	case BED:
		g_Furniture[obj].scl = { BED_SCL, BED_SCL, BED_SCL };
		LoadModel(MODEL_BED, &g_Furniture[obj].model);
		g_Furniture[obj].load = TRUE;
		g_Furniture[obj].use = TRUE;
		break;
	case TOILET:
		g_Furniture[obj].scl = { TOILET_SCL, TOILET_SCL*1.1f, TOILET_SCL };
		LoadModel(MODEL_TOILET, &g_Furniture[obj].model);
		g_Furniture[obj].load = TRUE;
		g_Furniture[obj].use = TRUE;
		break;
	case CANDLE:
		g_Furniture[obj].scl = { CANDLE_SCL*0.75f, CANDLE_SCL, CANDLE_SCL*0.75f };
		LoadModel(MODEL_CANDLE, &g_Furniture[obj].model);
		g_Furniture[obj].load = TRUE;
		g_Furniture[obj].use = TRUE;
		break;
	case STAIRS:
		g_Furniture[obj].scl = { STAIRS_SCL*1.5f, STAIRS_SCL*1.25f, STAIRS_SCL };
		LoadModel(MODEL_STAIRS, &g_Furniture[obj].model);
		g_Furniture[obj].load = TRUE;
		g_Furniture[obj].use = TRUE;
		break;
	case CAGE:
		g_Furniture[obj].scl = { CAGE_SCL*0.8, CAGE_SCL, CAGE_SCL };
		LoadModel(MODEL_CAGE, &g_Furniture[obj].model);
		g_Furniture[obj].load = TRUE;
		g_Furniture[obj].use = TRUE;
		break;

	}
	obj++;
}

FURNITURE *GetFurniture(void)
{
	return &g_Furniture[0];
}
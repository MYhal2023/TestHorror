//=============================================================================
//
// �I�u�W�F�N�g���f������ [furniture.cpp]
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "game.h"
#include "furniture.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	MODEL_???			"data/MODEL/???"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//static FURNITURE g_Furniture[MAX_FURNITURE];

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitFurniture(void)
{
	for (int i = 0; i < MAX_FURNITURE; i++)
	{
	}
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void UninitFurniture(void)
{

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
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	//for (int i = 0; i < MAX_FURNITURE; i++)
	//{
	//	if (g_Furniture[i].use == false) continue;

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_Furniture[i].scl.x, g_Furniture[i].scl.y, g_Furniture[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Furniture[i].rot.x, g_Furniture[i].rot.y, g_Furniture[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_Furniture[i].pos.x, g_Furniture[i].pos.y, g_Furniture[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_Furniture[i].mtxWorld, mtxWorld);


	//	// ���f���`��
	//	DrawModel(&g_Furniture[i].model);
	//}

	//// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);

}
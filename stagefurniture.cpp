//=============================================================================
//
// �X�e�[�W�I�u�W�F�N�g���f������ [furniture.cpp]
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "game.h"
#include "stagefurniture.h"
#include "furniture.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	MODEL_???			"data/MODEL/???"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//static FURNITURE g_StageFurniture[MAX_FURNITURE];

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitFurnitureFirStage(void)
{
	for (int i = 0; i < MAX_STAGEFURNITURE; i++)
	{
	}

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void UninitStageFurniture(void)
{

}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateStageFurniture(void)
{

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawStageFurniture(void)
{
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	//for (int i = 0; i < MAX_FURNITURE; i++)
	//{
	//	if (g_StageFurniture[i].use == false) continue;

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_StageFurniture[i].scl.x, g_StageFurniture[i].scl.y, g_StageFurniture[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_StageFurniture[i].rot.x, g_StageFurniture[i].rot.y, g_StageFurniture[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_StageFurniture[i].pos.x, g_StageFurniture[i].pos.y, g_StageFurniture[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_StageFurniture[i].mtxWorld, mtxWorld);


	//	// ���f���`��
	//	DrawModel(&g_StageFurniture[i].model);
	//}

	//// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);

}
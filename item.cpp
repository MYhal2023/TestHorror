//=============================================================================
//
// �A�C�e�����f������ [item.cpp]
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "game.h"
#include "item.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	MODEL_???			"data/MODEL/???"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//static MODEL_ITEM g_Item[MAX_ITEM];

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
	}
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void UninitItem(void)
{

}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateItem(void)
{

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawItem(void)
{
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	//for (int i = 0; i < MAX_ITEM; i++)
	//{
	//	if (g_Item[i].use == false) continue;

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_Item[i].scl.x, g_Item[i].scl.y, g_Item[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Item[i].rot.x, g_Item[i].rot.y, g_Item[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_Item[i].pos.x, g_Item[i].pos.y, g_Item[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_Item[i].mtxWorld, mtxWorld);


	//	// ���f���`��
	//	DrawModel(&g_Item[i].model);
	//}

	//// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);

}
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
#include "player.h"
#include "collision.h"
#include "itembox.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_KEY			"data/MODEL/key.obj"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MODEL_ITEM		g_Item[MAX_ITEM];
static BOOL				g_Load = FALSE;

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		//�܂��̓A�C�e�����^�C�v����
		switch (i)
		{
		case 0:
			g_Item[i].type = ITEM_KEY;
			g_Item[i].pos = { 0.0f, 20.0f, 150.0f };				// ���f���̈ʒu
			g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// ���f���̌���(��])
			g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// ���f���̑傫��(�X�P�[��)
			g_Item[i].size = SIZE_KEY;							// �����蔻��̑傫��
			break;
		case 1:
			g_Item[i].type = ITEM_KEY;
			g_Item[i].pos = { 0.0f, 20.0f, 50.0f };				// ���f���̈ʒu
			g_Item[i].rot = { 0.0f, 0.0f, 0.0f };				// ���f���̌���(��])
			g_Item[i].scl = { 1.0f, 1.0f, 1.0f };				// ���f���̑傫��(�X�P�[��)
			g_Item[i].size = SIZE_KEY;							// �����蔻��̑傫��
			break;
		}
		//�A�C�e���^�C�v�Ɋ�Â������f���̕R�Â�
		switch (g_Item[i].type)
		{
		case ITEM_KEY:
			LoadModel(MODEL_KEY, &g_Item[i].model);
			break;
		case MAXTYPE_ITEM:
			break;
		}
		g_Item[i].load = TRUE;
		g_Item[i].use = TRUE;
		g_Item[i].drop = TRUE;
		g_Item[i].ID = i;

		GetModelDiffuse(&g_Item[i].model, &g_Item[i].diffuse[0]);
	}
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void UninitItem(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_Item[i].load)
		{
			UnloadModel(&g_Item[i].model);
			g_Item[i].load = FALSE;
		}
	}

	g_Load = FALSE;

}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item)continue;

	}

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawItem(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].use || !g_Item[i].drop) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Item[i].scl.x, g_Item[i].scl.y, g_Item[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Item[i].rot.x, g_Item[i].rot.y, g_Item[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Item[i].pos.x, g_Item[i].pos.y, g_Item[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Item[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Item[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}

//�����Ă���A�C�e���Ƃ̓����蔻��
void CollisionItem(XMFLOAT3 pos, float size)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].use || !g_Item[i].drop)continue;
		if (!CollisionBC(pos, g_Item[i].pos, size, g_Item[i].size))continue;

		switch (g_Item[i].type)
		{
		case ITEM_KEY:
			AddItembox(KEY);
			g_Item[i].drop = FALSE;
			PlaySound(SOUND_LABEL_SE_GetItem);
			break;
		case MAXTYPE_ITEM:
			break;
		}
	}
}
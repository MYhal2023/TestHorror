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
#define	MODEL_DOOR			"data/MODEL/door.obj"		// �ǂݍ��ރ��f����
#define	MODEL_TOILET		"data/MODEL/toilet.obj"		// �ǂݍ��ރ��f����
#define	MODEL_WASHLET		"data/MODEL/washbasin.obj"		// �ǂݍ��ރ��f����
#define	MODEL_STAIRS		"data/MODEL/stairs.obj"		// �ǂݍ��ރ��f����
#define	MODEL_TABLE			"data/MODEL/table.obj"		// �ǂݍ��ރ��f����
#define	MODEL_CAGE			"data/MODEL/cage.obj"		// �ǂݍ��ރ��f����
#define	MODEL_BED			"data/MODEL/bed.obj"		// �ǂݍ��ރ��f����


#define DOOR_SCALE_X		(1.0f)						//�h�A�[��X�X�P�[��
#define DOOR_SCALE_Y		(1.8f)						//�h�A�[��Y�X�P�[��
#define DOOR_SCALE_Z		(2.4f)						//�h�A�[��Z�X�P�[��

#define STAIRS_SCALE_X		(3.7f)						//�K�i��X�X�P�[��
#define STAIRS_SCALE_Y		(2.0f*1.25f)				//�K�i��Y�X�P�[��
#define STAIRS_SCALE_Z		(2.0f)						//�K�i��Z�X�P�[��

#define WASHLET_SCALE_X		(1.5f)						//���ʏ���X�X�P�[��
#define WASHLET_SCALE_Y		(1.5f)						//���ʏ���Y�X�P�[��
#define WASHLET_SCALE_Z		(1.5f)						//���ʏ���Z�X�P�[��

#define TOILET_SCALE_X		(5.0f)						//�g�C����X�X�P�[��
#define TOILET_SCALE_Y		(5.5f)						//�g�C����Y�X�P�[��
#define TOILET_SCALE_Z		(5.0f)						//�g�C����Z�X�P�[��

#define TABLE_SCALE_X		(1.0f)						//�e�[�u����X�X�P�[��
#define TABLE_SCALE_Y		(1.0f)						//�e�[�u����Y�X�P�[��
#define TABLE_SCALE_Z		(1.0f)						//�e�[�u����Z�X�P�[��

#define CAGE_SCALE_X		(1.1f)						//�P�[�W��X�X�P�[��
#define CAGE_SCALE_Y		(1.0f)						//�P�[�W��Y�X�P�[��
#define CAGE_SCALE_Z		(1.0f)						//�P�[�W��Z�X�P�[��

#define BED_SCALE_X		(1.0f)							//�x�b�h��X�X�P�[��
#define BED_SCALE_Y		(1.0f)							//�x�b�h��Y�X�P�[��
#define BED_SCALE_Z		(1.0f)							//�x�b�h��Z�X�P�[��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static STAGEFURNITURE g_StageFurniture[MAX_STAGEFURNITURE];
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static BOOL						g_Load = FALSE;
static int						g_furniture;				//�Ƌ�̐��𐔂���
static int						sid;
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitFurnitureFirStage(void)
{
	ID3D11Device *pDevice = GetDevice();



	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < MAX_STAGEFURNITURE; i++)
	{
		g_StageFurniture[i].pos		= { 0.0f,0.0f,0.0f };
		g_StageFurniture[i].rot		= { 0.0f,0.0f,0.0f };
		g_StageFurniture[i].scl		= { 1.0f,1.0f,1.0f };
		g_StageFurniture[i].use		= FALSE;
		g_StageFurniture[i].load = FALSE;
		g_StageFurniture[i].ID = 99;
		g_StageFurniture[i].size = 0;
	}
	g_furniture = 0;
	sid = 0;
	g_Load = TRUE;
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void UninitStageFurniture(void)
{
	if (g_Load == FALSE) return;
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
	for (int i = 0; i < MAX_STAGEFURNITURE; i++)
	{
		if (g_StageFurniture[i].load = FALSE)
			continue;
		UnloadModel(&g_StageFurniture[i].model);
		g_StageFurniture[i].load = FALSE;
	}
	sid = 0;
	g_Load = FALSE;
	return;
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
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_STAGEFURNITURE; i++)
	{
		if (g_StageFurniture[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_StageFurniture[i].scl.x, g_StageFurniture[i].scl.y, g_StageFurniture[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_StageFurniture[i].rot.x, g_StageFurniture[i].rot.y, g_StageFurniture[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_StageFurniture[i].pos.x, g_StageFurniture[i].pos.y, g_StageFurniture[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_StageFurniture[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_StageFurniture[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}


void SetStageFurniture(int type, XMFLOAT3 pos, XMFLOAT3 rot)
{

	if (g_furniture >= MAX_STAGEFURNITURE)
		return;
	g_StageFurniture[g_furniture].pos = pos;
	g_StageFurniture[g_furniture].rot = rot;

	switch (type)
	{
	case DOOR:
		LoadModel(MODEL_DOOR, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = DOOR_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = DOOR_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = DOOR_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		g_StageFurniture[g_furniture].ID = sid;
		g_StageFurniture[g_furniture].size = 50.0f;
		sid++;
		break;
	case TOILEt:
		LoadModel(MODEL_TOILET, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = TOILET_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = TOILET_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = TOILET_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		break;
	case WASHLET:
		LoadModel(MODEL_WASHLET, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = WASHLET_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = WASHLET_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = WASHLET_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		break;
	case STAIRs:
		LoadModel(MODEL_STAIRS, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = STAIRS_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = STAIRS_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = STAIRS_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		break;
	case TABLE:
		LoadModel(MODEL_TABLE, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = TABLE_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = TABLE_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = TABLE_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		break;
	case CAGe:
		LoadModel(MODEL_CAGE, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = CAGE_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = CAGE_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = CAGE_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		g_StageFurniture[g_furniture].ID = 100;
		g_StageFurniture[g_furniture].size = 50.0f;
		break;
	case BEd:
		LoadModel(MODEL_BED, &g_StageFurniture[g_furniture].model);
		g_StageFurniture[g_furniture].scl.x = BED_SCALE_X;
		g_StageFurniture[g_furniture].scl.y = BED_SCALE_Y;
		g_StageFurniture[g_furniture].scl.z = BED_SCALE_Z;
		g_StageFurniture[g_furniture].load = TRUE;
		g_StageFurniture[g_furniture].use = TRUE;
		break;
	}
	g_furniture++;
}

STAGEFURNITURE *GetStageFurniture(void)
{
	return &g_StageFurniture[0];
}
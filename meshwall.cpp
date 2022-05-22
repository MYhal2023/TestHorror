//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "meshwall.h"
#include "renderer.h"
#include "player.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(3)						// �e�N�X�`���̐�

#define	VALUE_MOVE_WALL		(5.0f)					// �ړ����x
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.001f)		// ��]���x

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4		mtxWorld;
	XMFLOAT3		pos;						// �|���S���\���ʒu�̒��S���W
	XMFLOAT3		rot;						// �|���S���̉�]�p
	int				nNumBlockX, nNumBlockY;		// �u���b�N��
	int				nNumVertex;					// �����_��	
	int				nNumVertexIndex;			// ���C���f�b�N�X��
	int				nNumPolygon;				// ���|���S����
	float			fBlockSizeX, fBlockSizeY;	// �u���b�N�T�C�Y
	int				texNo;						//�g�p����e�N�X�`���̎w��
	BOOL			use;	
	MATERIAL		material;					// �}�e���A��
	ID3D11Buffer	*vertexBuffer;				// ���_�o�b�t�@
	ID3D11Buffer	*indexBuffer;				// �C���f�b�N�X�o�b�t�@

} MESH_WALL;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;		// �e�N�X�`���ԍ�

static MESH_WALL g_aMeshWall[MAX_MESH_WALL];		// ���b�V���ǃ��[�N
static int g_nNumMeshWall = 0;						// ���b�V���ǂ̐�
static int g_CeilingWall = 0;						// �V��Ɏg�p�������b�V����

static char* g_TextureName[TEXTURE_MAX] = {
	"data/TEXTURE/wall_001.png",
	"data/TEXTURE/wall_002.png",
	"data/TEXTURE/wall101.jpg",
};

static BOOL							g_Load = FALSE;


//======================================================Po=======================
// ����������
//=============================================================================
HRESULT InitMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
						int nNumBlockX, int nNumBlockY, float fBlockSizeX, float fBlockSizeZ, int texNo)
{
	MESH_WALL *pMesh;

	if(g_nNumMeshWall >= MAX_MESH_WALL)
	{
		return E_FAIL;
	}



	pMesh = &g_aMeshWall[g_nNumMeshWall];

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh->material, sizeof(pMesh->material));
	pMesh->material.Diffuse = col;

	pMesh->use = TRUE;
	pMesh->texNo = texNo;
	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh->pos = pos;

	pMesh->rot = rot;

	// �u���b�N���̐ݒ�
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;

	// ���_���̐ݒ�
	pMesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh->fBlockSizeX = fBlockSizeX;
	pMesh->fBlockSizeY = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh->vertexBuffer);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(pMesh->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh->indexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for(int nCntVtxY = 0; nCntVtxY < (pMesh->nNumBlockY + 1); nCntVtxY++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Position.x = -(pMesh->nNumBlockX / 2.0f) * pMesh->fBlockSizeX + nCntVtxX * pMesh->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Position.y = pMesh->nNumBlockY * pMesh->fBlockSizeY - nCntVtxY * pMesh->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Position.z = 0.0f;

				// �@���̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxY;
			}
		}

		GetDeviceContext()->Unmap(pMesh->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for(int nCntVtxY = 0; nCntVtxY < pMesh->nNumBlockY; nCntVtxY++)
		{
			if(nCntVtxY > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxY < (pMesh->nNumBlockY - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + pMesh->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh->indexBuffer, 0);
	}

	if (pMesh->rot.x == XM_PI * 0.5f && pMesh->rot.z == XM_PI * 0.5f)
		g_CeilingWall = g_nNumMeshWall;

	g_nNumMeshWall++;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	MESH_WALL *pMesh;
	int nCntMeshField;

	if (g_Load == FALSE) return;

	for(nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];

		if(pMesh->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pMesh->vertexBuffer->Release();
			pMesh->vertexBuffer = NULL;
		}

		if(pMesh->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh->indexBuffer->Release();
			pMesh->indexBuffer = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	//�\���̂̃����o�ϐ���S�ă��Z�b�g
	ResetMeshWall();
	//�ǂݍ��ݐ������Z�b�g����
	g_nNumMeshWall = 0;
	g_CeilingWall = 0;
	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(void)
{
	MESH_WALL *pMesh;
	int nCntMeshField;
	
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);


	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	for(nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];
		
		if (pMesh->use != TRUE)continue;

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh->vertexBuffer, &stride, &offset);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		GetDeviceContext()->IASetIndexBuffer(pMesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0);


		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(pMesh->rot.x, pMesh->rot.y, pMesh->rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&pMesh->mtxWorld, mtxWorld);

		// �}�e���A���ݒ�
		SetMaterial(pMesh->material);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[pMesh->texNo]);

		// �|���S���̕`��
		GetDeviceContext()->DrawIndexed(pMesh->nNumVertexIndex, 0, 0);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

void MeshWallHit(XMFLOAT3 pos, float size, float old_x, float old_z)
{
	MESH_WALL *pMesh;
	for (int i = 0; i < g_nNumMeshWall; i++)
	{
		pMesh = &g_aMeshWall[i];
		if (pMesh->use != TRUE)continue;

		//��]�ʂ�����x����z�������̕����v�Z
		float rotatew = cosf(pMesh->rot.y);
		float rotatez = sinf(pMesh->rot.y);
		float width = pMesh->fBlockSizeX * fabsf(rotatew);
		float thickness = pMesh->fBlockSizeX * fabsf(rotatez);

		//�ǂƃv���C���[�̓����蔻��BBB�ōs�����߁Ay���W�͌���l�����Ă��Ȃ��B
		if (CollisionBB(pos, size, size, pMesh->pos, width, thickness) == TRUE
			&& pMesh->rot.x != XM_PI * 0.5f && pMesh->rot.z != XM_PI * 0.5f)
		{
			PLAYER *player = GetPlayer();
			//���슴���グ�邽�߂ɁA�Е��̍��W�݂̂����ɖ߂�
			if (width < pMesh->fBlockSizeX)
				player->pos.x = old_x;
			else if (thickness < pMesh->fBlockSizeX)
				player->pos.z = old_z;

			//�����̍��W���߂��Ă���Ȃ炱��ȏ�̑����͕K�v�Ȃ�
			if (player->pos.x == old_x && player->pos.z == old_z)
				return;
		}
	}

}

void InitFieldMeshWall(void)
{
	//�}�b�v�����̕�
	InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, WALL_A_POS_XZ), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_A_POS_XZ, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_A_POS_XZ, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, -WALL_A_POS_XZ), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X��(+),Z��(+)
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X��(-),Z��(-)
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ - WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ - WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X��(+),Z��(-)
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ - WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ + -WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X��(-),Z��(+)
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

}

XMFLOAT3 GetMeshWallStPosition(int i)
{
	MESH_WALL *pMesh;
	XMFLOAT3 pos;
	pMesh = &g_aMeshWall[i];
	float rotatew = cosf(pMesh->rot.y);
	float rotatez = sinf(pMesh->rot.y);
	float width = pMesh->fBlockSizeX * fabsf(rotatew);
	float thickness = pMesh->fBlockSizeX * fabsf(rotatez);
	if (width > thickness)
		pos = { pMesh->pos.x + width * 0.5f, 0.0f, pMesh->pos.z };
	else if (thickness > width)
		pos = { pMesh->pos.x, 0.0f, pMesh->pos.z + thickness * 0.5f };

	return pos;
}

XMFLOAT3 GetMeshWallEdPosition(int i)
{
	MESH_WALL *pMesh;
	XMFLOAT3 pos;
	pMesh = &g_aMeshWall[i];
	float rotatew = cosf(pMesh->rot.y);
	float rotatez = sinf(pMesh->rot.y);
	float width = pMesh->fBlockSizeX * fabsf(rotatew);
	float thickness = pMesh->fBlockSizeX * fabsf(rotatez);
	if (width > thickness)
		pos = { pMesh->pos.x - width * 0.5f, 0.0f, pMesh->pos.z };
	else if (thickness > width)
		pos = { pMesh->pos.x, 0.0f, pMesh->pos.z - thickness * 0.5f };

	return pos;
}

int GetMeshWallNum(void)
{
	return g_nNumMeshWall;
}

int GetCeilingWallNum(void)
{
	return g_CeilingWall;
}

void ResetMeshWall(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}
	for (int i = 0; i < MAX_MESH_WALL; i++)
	{
		ZeroMemory(&g_aMeshWall[i].material, sizeof(g_aMeshWall[i].material));
		g_aMeshWall[i].material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_aMeshWall[i].vertexBuffer = NULL;				// ���_�o�b�t�@
		g_aMeshWall[i].indexBuffer = NULL;				// �C���f�b�N�X�o�b�t�@
		g_aMeshWall[i].pos = {0.0f, 0.0f, 0.0f};		// �|���S���\���ʒu�̒��S���W
		g_aMeshWall[i].rot = { 0.0f, 0.0f, 0.0f };		// �|���S���̉�]�p
		g_aMeshWall[i].nNumBlockX = 0;
		g_aMeshWall[i].nNumBlockY = 0;					// �u���b�N��
		g_aMeshWall[i].nNumVertex = 0;					// �����_��	
		g_aMeshWall[i].nNumVertexIndex = 0;				// ���C���f�b�N�X��
		g_aMeshWall[i].nNumPolygon = 0;					// ���|���S����
		g_aMeshWall[i].fBlockSizeX = 0;
		g_aMeshWall[i].fBlockSizeY = 0;					// �u���b�N�T�C�Y
		g_aMeshWall[i].texNo = WALL_GRAY;						//�g�p����e�N�X�`���̎w��
		g_aMeshWall[i].use = FALSE;
	}
	g_nNumMeshWall = 0;
	g_CeilingWall = 0;
}
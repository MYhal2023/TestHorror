//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author :�đq�r�N
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "shadow.h"
#include "enemy.h"
#include "sound.h"
#include "camera.h"
#include "collision.h"
#include "input.h"
#include "player.h"
#include "amadeus.h"
#include "game.h"
#include "light.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(3)			// �e�N�X�`���̐�

#define	ENEMY_TEXMAG	(0.08f)							// ���摜�ɑ΂���{��
#define	ENEMY_WIDTH		(650.0f * ENEMY_TEXMAG)			// ���_�T�C�Y
#define	ENEMY_HEIGHT	(812.0f * ENEMY_TEXMAG)			// ���_�T�C�Y
#define ENEMY_SIGHT		(400.0f)		//�G�l�~�[�̎���
#define	ENEMY_SPEED		(1.0f)			// �G�l�~�[�̈ړ��X�s�[�h


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemy(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static ENEMY						g_Enemy[MAX_ENEMY];	// �o���b�g���[�N

static char *g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/enemy.png",
	"data/TEXTURE/enemy_debug.png",
	"data/TEXTURE/enemy_back.png",
};

static BOOL							g_Load = FALSE;

static INTERPOLATION_DATA move_tbl[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	MakeVertexEnemy();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	// �G�l�~�[�̏�����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		ZeroMemory(&g_Enemy[i].material, sizeof(g_Enemy[i].material));
		g_Enemy[i].material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

		g_Enemy[i].pos = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 1.0f, 1.0f, 1.0f };
		g_Enemy[i].spd = ENEMY_SPEED;
		g_Enemy[i].fWidth = ENEMY_WIDTH;
		g_Enemy[i].fHeight = ENEMY_HEIGHT;
		g_Enemy[i].use = FALSE;
		g_Enemy[i].texNo = 0;

		g_Enemy[i].move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy[i].tbl_size = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	}

	//���`��ԃf�[�^�Z�b�g
	g_Enemy[0].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[0].tbl_adr = move_tbl;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{// �e�N�X�`���̉��
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if (g_VertexBuffer != NULL)
	{// ���_�o�b�t�@�̉��
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use != TRUE)	//�g���ĂȂ��G�l�~�[�͏������X�L�b�v
			continue;

		PLAYER *player = GetPlayer();
		LIGHT *light = GetLightData(1);
		XMVECTOR vec = XMLoadFloat3(&g_Enemy[i].pos) - XMLoadFloat3(&player->pos);
		vec = XMVector3Length(vec);
		float lenSq = 0.0f;
		XMStoreFloat(&lenSq, vec);
		lenSq = fabsf(lenSq);
		float line = 0.0f;
		if (CheckLightOn() == TRUE)
		{
			line = LIGHT_LINE_ON;
		}
		else
		{
			line = LIGHT_LINE_OFF;
		}

		float diffuse = FloatClamp(1.0f - (lenSq + 0.01f) / line, 0.0f, 1.0f);
		float alpha = 1.0f;
		if (diffuse <= 0.1f)alpha = diffuse;
		g_Enemy[i].material.Diffuse = { diffuse, diffuse, diffuse, alpha };

		//�G�l�~�[�̃X�e�[�g����
		int oldState = g_Enemy[i].state;
		g_Enemy[i].state = StateCheck(i);
		int nowState = g_Enemy[i].state;
		//�X�e�[�g���J�ڂ����ꍇ�͐��`��ԃf�[�^�̕ۑ��Ȃǂ��s��
		if (oldState != nowState)
		{
			StateAdjust(i);
		}
		switch (g_Enemy[i].state)
		{
		case Patrol:
			EnemyInterPoration(i);
			break;

		case Chase:
			//AI�����̌��A�ړ��x�N�g��������
			g_Enemy[i].moveVec = TacticalPointSystem(i);
			break;
		}

		//�ړ��l���x�N�g���ϊ����Ĉړ�������
		XMVECTOR moveVec = g_Enemy[i].moveVec;
		XMVECTOR now = XMLoadFloat3(&g_Enemy[i].pos);								// ���݂̏ꏊ
		XMStoreFloat3(&g_Enemy[i].pos, now + XMVector3Normalize(moveVec) * g_Enemy[i].spd);	//�P�ʃx�N�g�������Ɉړ�

		//�f�o�b�O���Ȃ�摜�̓���ւ����\(�|����ƕ|���Ȃ���ɓ���ւ��\�I)
#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F))
			g_Enemy[i].texNo = (g_Enemy[i].texNo == ENEMY_WOMAN) ? ENEMY_DEBUG : ENEMY_WOMAN;
#endif
	}
	//�G�l�~�[�̃Z�b�g���s����
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_G))
		SetEnemy(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// ���e�X�g��L����
	SetAlphaTestEnable(TRUE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxTranslate, mtxRot, mtxWorld;
	CAMERA *cam = GetCamera();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);


	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_Enemy[i].use)continue;

		EnemyAngleTex(i);

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// �}�e���A���ݒ�
		SetMaterial(g_Enemy[i].material);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

		// �|���S���̕`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// ���e�X�g�𖳌���
	SetAlphaTestEnable(FALSE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEnemy(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = ENEMY_WIDTH;
	float fHeight = ENEMY_HEIGHT;

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, 0.0f, 0.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// �G�l�~�[�̃p�����[�^���Z�b�g
//=============================================================================
int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot)
{
	int nIdxEnemy = -1;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use)continue;
		g_Enemy[i].pos = pos;
		g_Enemy[i].rot = rot;
		g_Enemy[i].scl = { 1.0f, 1.0f, 1.0f };
		g_Enemy[i].use = TRUE;

		g_Enemy[i].texNo = ENEMY_WOMAN;
		break;
	}

	return nIdxEnemy;
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &(g_Enemy[0]);
}

//�v���C���[�̎��_���猩���G�l�~�[�̑O�㔻��
void EnemyAngleTex(int i)
{
	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();

	//�p�x�Ƃ𗘗p���ăG�l�~�[�̕����x�N�g�����쐬
	float buffer = 1.0f;
	XMFLOAT3 pos1 = { g_Enemy[i].pos.x + sinf(g_Enemy[i].rot.y * buffer), 0.0f, g_Enemy[i].pos.z + cosf(g_Enemy[i].rot.y * buffer) };
	XMFLOAT3 pos2 = { g_Enemy[i].pos.x, 0.0f, g_Enemy[i].pos.z };
	XMVECTOR v1 = XMVector3Normalize(XMLoadFloat3(&pos1) - XMLoadFloat3(&g_Enemy[i].pos)); //���K�����������x�N�g��

	//�J��������G�l�~�[�Ɍ������x�N�g�����쐬
	XMFLOAT3 playerEdPos = { g_Enemy[i].pos.x, 0.0f, g_Enemy[i].pos.z };
	XMFLOAT3 playerStPos = { cam->pos.x, 0.0f, cam->pos.z };
	XMVECTOR v2 = XMVector3Normalize(XMLoadFloat3(&playerEdPos) - XMLoadFloat3(&playerStPos)); //���K�������x�N�g��

	//2�x�N�g���̓��ς����߁A���ς̐����ɂ���ĉ摜�������ւ�(���\�𔽉f�����邽��)
	float ECdot = dotProduct(&v1, &v2);
	if (ECdot <= 0.0f)
		g_Enemy[i].texNo = ENEMY_WOMAN;
	else
		g_Enemy[i].texNo = ENEMY_BACK;
}

int StateCheck(int i)
{
	PLAYER *player = GetPlayer();
	int ans = Patrol;			//�f�t�H���g�͏��񃂁[�h
	//�v���C���[�����E�ɑ�������
	if (Visibility(g_Enemy[i].pos, player->pos, g_Enemy[i].rot.y, ENEMY_SIGHT) == TRUE ||
		 CollisionBC(player->pos, g_Enemy[i].pos, player->size, g_Enemy[i].fWidth) == TRUE)
	{
		ans = Chase;	//�ǐՊJ�n
	}

	return ans;
}

void EnemyInterPoration(int i)
{
	if (g_Enemy[i].tbl_adr == NULL)return;	// ���`��Ԃ����s����H
							// ���`��Ԃ̏���
	// �ړ�����
	int		index = (int)g_Enemy[i].move_time;
	float	time = g_Enemy[i].move_time - index;
	int		size = g_Enemy[i].tbl_size;

	float dt = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
	g_Enemy[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

	if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
	{
		g_Enemy[i].move_time = 0.0f;
		index = 0;
	}

	// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
	XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
	XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Enemy[i].pos, p0 + vec * time);

	// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
	XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// ���̊p�x
	XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

	// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
	XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// ����Scale
	XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// ���݂�Scale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

}

//�X�e�[�g�J�ڂɔ�����񐮍����s��
void StateAdjust(int i)
{
	int index = (int)g_Enemy[i].move_time;

	switch (g_Enemy[i].state)
	{
	case Patrol:	//�ǐՂ��珄���
		//BGM�J�ڏ���
		SetSoundFade(SOUND_LABEL_BGM_Fear, SOUNDFADE_OUT, 0.0f, 3.0f);
		if (g_Enemy[i].tbl_adr == NULL)return;	// ���`��Ԃ����s����H

		// ���`��ԃf�[�^������W���̒��o
		XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index].pos);	// ���݂̏ꏊ
		XMStoreFloat3(&g_Enemy[i].pos, p0);

		break;

	case Chase:		//���񂩂�ǐՂ�
		StopSound();
		float Volume = 0.5f;
		SetSourceVolume(SOUND_LABEL_BGM_Fear, Volume);
		PlaySound(SOUND_LABEL_BGM_Fear);
		break;
	}
}
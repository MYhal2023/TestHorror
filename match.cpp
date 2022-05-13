//=============================================================================
//
// �}�b�`���� [match.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "sprite.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "input.h"
#include "match.h"
#include "debugproc.h"
#include "sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// �e�N�X�`���T�C�Y
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define MATCH_POS_X					(600)		// �}�b�`��\������ꏊ(X��)
#define DEFAULTMATCH_POS_Y			(600.0f)	// �ʏ펞�̃}�b�`�̏ꏊ(Y��)
#define STANDBYMATCH_POS_Y			(500.0f)	// �X�^���o�C���̃}�b�`�̏ꏊ(Y��)
#define STANDBYTIME					(10)		// �}�b�`���\����܂ł̎���
#define STANDBY_MOVE_FRAME			((STANDBYMATCH_POS_Y-DEFAULTMATCH_POS_Y)/STANDBYTIME)//�\���鎞��1�t���[��������̈ړ���
#define MATCH_NUM					(10)			// �}�b�`�̖{��(����)

#define MATCH_SLOW					(300)
#define MATCH_MIDDLE				(600)
#define MATCH_FAST					(100)
//#define	MODEL_???			"data/MODEL/???"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/match.png",
	"data/TEXTURE/fire.png",

};




static MATCH						g_Match;

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMatch(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �}�b�`�̏�����
	g_Match.Use   = TRUE;
	g_Match.w     = TEXTURE_WIDTH;
	g_Match.h     = TEXTURE_HEIGHT;
	g_Match.Pos   = { MATCH_POS_X, DEFAULTMATCH_POS_Y, 0.0f };
	g_Match.TexNo = 0;
	g_Match.StandbyTime = 0;
	g_Match.num = MATCH_NUM;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMatch(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMatch(void)
{

	StandbyMatch();

#ifdef _DEBUG	// �f�o�b�O����\������
	//�}�b�`�̔R�Ď��Ԃ�ݒ�
	if (GetKeyboardTrigger(DIK_M))
	{
		g_Match.AblazeTime = 5000;
	}
	if (GetKeyboardTrigger(DIK_N))
	{
		g_Match.AblazeTime = 0;
	}

	PrintDebugProc("Match.num %d\n", g_Match.num);
	PrintDebugProc("Match.AblazeTime %d\n", g_Match.AblazeTime);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMatch(void)
{
	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	//if (g_Match.Use == false) return;

	//// ���[���h�}�g���b�N�X�̏�����
	//mtxWorld = XMMatrixIdentity();

	//// �X�P�[���𔽉f
	//mtxScl = XMMatrixScaling(g_Match.scl.x, g_Match.scl.y, g_Match.scl.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// ��]�𔽉f
	//mtxRot = XMMatrixRotationRollPitchYaw(g_Match.rot.x, g_Match.rot.y, g_Match.rot.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//// �ړ��𔽉f
	//mtxTranslate = XMMatrixTranslation(g_Match.Pos.x, g_Match.Pos.y, g_Match.Pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//SetWorldMatrix(&mtxWorld);

	//XMStoreFloat4x4(&g_Match.mtxWorld, mtxWorld);

	//// ���f���`��
	//DrawModel(&g_Match.model);

	//// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);
}

//
void StandbyMatch(void)
{
	//�}�b�`���R���Ă��鎞�̏���
	int oldBtime = g_Match.AblazeTime;				//�ߋ��̎c�R�Ď��Ԃ�ۑ�
	if (g_Match.AblazeTime > 0)g_Match.AblazeTime--;//�R�Ď��Ԃ��c���Ă�Ȃ猸�Z
	int nowBtime = g_Match.AblazeTime;				//���Z��̎c�R�Ď��Ԃ�ۑ�
	//�R���s������}�b�`�̃X�^���o�C���Ԃ����Z�b�g����
	if (nowBtime <= 0 && oldBtime > 0) g_Match.StandbyTime = 0;	//�ߋ��c��1�ȏ�A���ݎc��0�ȉ����������R���s�����^�C�~���O�I

	g_Match.Pos.y = STANDBY_MOVE_FRAME * g_Match.StandbyTime + DEFAULTMATCH_POS_Y;	//�}�b�`�̃|�W�V�����͏�ɍX�V

	//�}�b�`���R���ĂȂ��Ȃ炱���ŏ������I���
	if (g_Match.AblazeTime > 0)return;
	
	//�}�b�`���R���Ă��Ȃ����̏���
	//�}�b�`���\�������
	if (IsButtonPressed(0, BUTTON_L)&&(0 < g_Match.num))
	{
		//�\������܂ł̎���
		if (g_Match.StandbyTime < STANDBYTIME)
		{
			g_Match.StandbyTime++;
			return;
		}

		if(IsButtonPressed(0, BUTTON_R)) SetForceState(TRUE);
		else SetForceState(FALSE);

		switch (IsButtonForce(0))
		{
		case FORCE_NON:
			break;
		case FORCE_SLOW:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_SLOW;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		case FORCE_MIDDLE:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_MIDDLE;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		case FORCE_FAST:
			g_Match.num--;
			g_Match.AblazeTime = MATCH_FAST;
			PlaySound(SOUND_LABEL_SE_MatchON);
			break;
		}
		InitForce(0);
	}
	else
	{
		//���낵�I���܂ł̎���
		if (0 < g_Match.StandbyTime)
		{
			g_Match.StandbyTime--;
			return;
		}
	}

}


void SetMatchForce(int force)
{
	g_Match.force = force;
}

MATCH *GetMatch(void)
{
	return &g_Match;
}
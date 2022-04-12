//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "game.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "collision.h"
#include "time.h"
#include "sound.h"
#include "lighter.h"
#include "match.h"
#include "lighter.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE			(1.0f)							// �ړ���
#define	VALUE_DASH_MOVE		(2.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(0.0f)							// �v���C���[�̑��������킹��
#define PLAYER_OFFSET_Z		(-300.0f)							// �v���C���[�̑��������킹��
#define PLAYER_LIFE			(100)								// �v���C���[�̃��C�t
#define PLAYER_STAMINA		(100)								// �v���C���[�̃X�^�~�i

#define PLAYER_PARTS_MAX	(1)								// �v���C���[�̃p�[�c�̐�
#define PLAYER_AT_FLAME		(30.0f)							// �v���C���[�̍U���t���[��
#define PLAYER_INVINC_FLAME	(120.0f)						// �v���C���[���G�t���[��
#define DEFER				(2.0f)							// �v���C���[�ړ������̗]�蕝
#define PLAYER_MIN_X	(-(FIELD_X / 2.0f) * BLOCK_SIZE + DEFER * BLOCK_SIZE)				// �v���C���[�ړ����
#define PLAYER_MIN_Z	((FIELD_Z / 2.0f) * BLOCK_SIZE - (FIELD_Z - DEFER) * BLOCK_SIZE)	// 
#define PLAYER_MAX_X	(-(FIELD_X / 2.0f) * BLOCK_SIZE + (FIELD_X - DEFER) * BLOCK_SIZE )	// 
#define PLAYER_MAX_Z	((FIELD_Z / 2.0f) * BLOCK_SIZE - DEFER * BLOCK_SIZE)				// 

#define PLAYER_PEACE_LIFE (PLAYER_LIFE * 0.8)
#define PLAYER_ANXIE_LIFE (PLAYER_LIFE * 0.5)
#define PLAYER_FEAR_LIFE  (PLAYER_LIFE * 0.25)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static BOOL			g_Load = FALSE;
static int			atCount;
static float		angle = 0.0f;
static int			g_BeatType;
static int			g_BreathType;
// �v���C���[�̊K�w�A�j���[�V�����f�[�^
static INTERPOLATION_DATA move_tbl_right[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(20.0f, 15.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(20.0f, 15.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		 XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
	{ XMFLOAT3(20.0f, 15.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_left[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};



//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;

	g_Player.pos = { 0.0f, PLAYER_OFFSET_Y, PLAYER_OFFSET_Z };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 0.8f, 1.0f, 1.0f };
	g_Player.moveVec = { 0.0f, 0.0f, 0.0f };
	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��
	g_Player.life = PLAYER_LIFE;
	g_Player.lifeMax = g_Player.life;
	g_Player.stamina = PLAYER_LIFE;
	g_Player.staminaMax = g_Player.stamina;
	g_Player.use = TRUE;
	g_Player.dash = FALSE;
	g_Player.attack = FALSE;
	g_Player.attackUse = FALSE;

	g_Player.atInvinc = FALSE;				//���G���Ԓ����ۂ�
	g_Player.atInvincTime = 0.0f;			//���Ԍv���p
	g_Player.blink = 0.0f;
	g_Player.blinking = 10.0f;
	g_Player.blinkingMax = 20.0f;
	g_Player.blinkingCount = 2.0f;
	g_Player.BeatSpeed = 1;
	g_Player.BreathInterval = 0;
	// �K�w�A�j���[�V�����p�̏���������
	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	g_BeatType = 0;
	g_BreathType = 0;
	g_Load = TRUE;
	atCount = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//�v���C���[�̋����W��ۑ�
	float old_x = g_Player.pos.x;
	float old_z = g_Player.pos.z;

	CAMERA *cam = GetCamera();
	PlayerMoveControl();	//�v���C���[�̈ړ�����	
	PlayerDashControl();	//�_�b�V������
	PlayerDashProcess();	//�_�b�V���̏���

	// Key���͂���������ړ���������
	if (g_Player.spd > 0.0f)
	{
		float angle = atan2f(g_Player.moveVec.x, g_Player.moveVec.z);
		g_Player.rot.y = angle;
	}
	//�ړ��l���x�N�g���ϊ����Ĉړ�������
	XMVECTOR moveVec = XMLoadFloat3(&g_Player.moveVec);
	XMVECTOR now = XMLoadFloat3(&g_Player.pos);								// ���݂̏ꏊ
	XMStoreFloat3(&g_Player.pos, now + XMVector3Normalize(moveVec) * g_Player.spd);	//�P�ʃx�N�g�������Ɉړ�

	HeartBeat();	//�S���̃Z�b�g
	PlayerBreath(); //�������̃Z�b�g
	MeshWallHit(g_Player.pos, g_Player.size, old_x, old_z);
	////���b�V���t�B�[���h�͈͊O�ɏo�Ȃ��悤�ɂ���
	//if (g_Player.pos.x <= PLAYER_MIN_X ||
	//	g_Player.pos.x >= PLAYER_MAX_X ||
	//	g_Player.pos.z <= PLAYER_MIN_Z ||
	//	g_Player.pos.z >= PLAYER_MAX_Z)
	//{
	//	g_Player.pos.x = old_x;
	//	g_Player.pos.z = old_z;
	//}

	if (g_Player.atInvinc == TRUE)	//��_���[�W�ɂ�閳�G���ɂ��邱�Ƃ́H
	{
		IncibleEffect();
		g_Player.atInvincTime += 1.0f;
		if (g_Player.atInvincTime >= PLAYER_INVINC_FLAME)//���G���Ԃ��I��������
		{
			g_Player.blinkingCount = 1.0f;
			g_Player.blink = 0.0f;
			g_Player.atInvincTime = 0.0f;
			g_Player.atInvinc = FALSE;
		}
	}


	g_Player.spd *= 0.7f;
	g_Player.moveVec.x *= 0.8f;
	g_Player.moveVec.z *= 0.8f;

#ifdef _DEBUG
	//�̗͂̒���
	if (GetKeyboardPress(DIK_1))
	{
		g_Player.life = 100;
	}
	if (GetKeyboardPress(DIK_2))
	{
		g_Player.life = 70;
	}
	if (GetKeyboardPress(DIK_3))
	{
		g_Player.life = 49;
	}
	if (GetKeyboardPress(DIK_4))
	{
		g_Player.life = 1;
	}

	if (GetKeyboardPress(DIK_7))
	{
		padFFAxisStart(0);
	}
	if (GetKeyboardPress(DIK_8))
	{
		padFFAxisStop(0);
	}

	PrintDebugProc("g_Player.life %d\n", g_Player.life);

#endif


#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	PrintDebugProc("Player:Stamina %d Player:spd %f\n", g_Player.stamina, g_Player.spd);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	//// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// ���[���h�}�g���b�N�X�̏�����
	//mtxWorld = XMMatrixIdentity();

	//// �X�P�[���𔽉f
	//mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// ��]�𔽉f
	//mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y, g_Player.rot.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	////// �N�H�[�^�j�I���𔽉f
	//XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.quaternion));
	//mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);
	//
	//// �ړ��𔽉f
	//mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//SetWorldMatrix(&mtxWorld);

	//XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	//SetFuchi(1);

	//// ���f���`��
	//DrawModel(&g_Player.model);


	//SetFuchi(0);

	//// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//void SettingPlayer(void)
//{
//	// ���C�L���X�g���đ����̍��������߂�
//	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// �Ԃ������|���S���̖@���x�N�g���i�����j
//	XMFLOAT3 hitPosition;								// ��_
//	hitPosition.y = g_Player.pos.y - PLAYER_OFFSET_Y;	// �O�ꂽ���p�ɏ��������Ă���
//	bool ans = RayHitField(g_Player.pos, &hitPosition, &normal);
//	g_Player.pos.y = hitPosition.y + PLAYER_OFFSET_Y;
//	//g_Player.pos.y = PLAYER_OFFSET_Y;
//
//
//
//	g_Player.spd *= 0.5f;
//
//
////////////////////////////////////////////////////////////////////////
//// �p������
////////////////////////////////////////////////////////////////////////
//
//	XMVECTOR vx, nvx, up;
//	XMVECTOR quat;
//	float len, angle;
//
//	// �Q�̃x�N�g���̊O�ς�����ĔC�ӂ̉�]�������߂�
//	g_Player.upVector = normal;
//	up = { 0.0f, 1.0f, 0.0f, 0.0f };
//	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));	//���ꂪ�C�ӂ̎�
//
//	// ���߂���]������N�H�[�^�j�I�������o��
//	nvx = XMVector3Length(vx);
//	XMStoreFloat(&len, nvx);
//	nvx = XMVector3Normalize(vx);
//	angle = asinf(len);
//	quat = XMQuaternionRotationNormal(nvx, angle);
//
//	// �O��̃N�H�[�^�j�I�����獡��̃N�H�[�^�j�I���܂ł̉�]�����炩�ɂ���
//	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.2f);		//��O�����͗v�̓p�[�Z���e�[�W�B���̏ꍇ1f��5%���X�������Ă�
//
//	// ����̃N�H�[�^�j�I���̌��ʂ�ۑ�����
//	XMStoreFloat4(&g_Player.quaternion, quat);
//
//}

void IncibleEffect(void)
{
	if (g_Player.blinking < 0)
	{
		g_Player.blinkingCount *= -1;
		g_Player.blink -= 1.0f;
	}

	if (g_Player.blinking > g_Player.blinkingMax)
	{
		g_Player.blinkingCount *= -1;
		g_Player.blink += 1.0f;
	}
	g_Player.blinking += g_Player.blinkingCount;
}

void PlayerMoveControl(void)
{
	CAMERA *cam = GetCamera();
	if (g_Player.attack == TRUE || GetTime() <= 0 || g_Player.life <= 0)	//�U�������Q�[���I�[�o�[�łȂ���Έړ��\
		return;

	float changeRotCamera = 0.025f; //�X�����x
	// �ړ��{�^��
	if (IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_A))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x -= cosf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z += sinf(cam->rot.y) * changeRotCamera;
	}
	if (IsButtonPressed(0, BUTTON_RIGHT) || GetKeyboardPress(DIK_D))
	{	// �E�ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x += cosf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z -= sinf(cam->rot.y) * changeRotCamera;
	}
	if (IsButtonPressed(0, BUTTON_UP) || GetKeyboardPress(DIK_W))
	{	// ��ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x += sinf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z += cosf(cam->rot.y) * changeRotCamera;
	}
	if (IsButtonPressed(0, BUTTON_DOWN) || GetKeyboardPress(DIK_S))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x -= sinf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z -= cosf(cam->rot.y) * changeRotCamera;
	}
}

void PlayerDashControl(void)
{
	//�X�^�~�i0�ȉ��̓_�b�V���ł��Ȃ�
	if (g_Player.stamina <= 0)return;	

	if (IsButtonPressed(0, BUTTON_B) || GetKeyboardPress(DIK_Z))
		g_Player.dash = TRUE;
	else
		g_Player.dash = FALSE;
}

void PlayerLighterControl(void)
{
	if (GetKeyboardTrigger(DIK_L))
	{
		SetLighterOn(TRUE);
	}
	if (GetKeyboardTrigger(DIK_J))
	{
		SetLighterOn(FALSE);
	}
}

void PlayerDashProcess(void)
{
	if (g_Player.dash == FALSE)return;

	g_Player.spd = VALUE_DASH_MOVE;
	g_Player.stamina--;

	if (g_Player.stamina <= 0)
		g_Player.dash = FALSE;
}

void HeartBeat(void)
{
	float Volume = 0.0f;
	g_Player.BeatSpeed--;
	BeatTypeDecision();
	if (g_BeatType == BEAT_NON || g_Player.BeatSpeed > 0)return;	
	MATCH *match = GetMatch();
	switch (g_BeatType)
	{
	case BEAT_SHALLOW:
		Volume = 0.2f;
		g_Player.BeatSpeed = 120;
		break;
	case BEAT_VSLOW:		
		Volume = 0.3f;
		g_Player.BeatSpeed = 100;
		break;
	case BEAT_SLOW:
		Volume = 0.4f;
		g_Player.BeatSpeed = 90;
		break;
	case BEAT_MIDDLE:
		Volume = 0.4f;
		g_Player.BeatSpeed = 70;
		break;
	case BEAT_FAST:
		Volume = 0.5f;
		g_Player.BeatSpeed = 60;
		break;
	case BEAT_VFAST:
		Volume = 0.7f;
		g_Player.BeatSpeed = 30;
		break;
	}
	SetSourceVolume(SOUND_LABEL_SE_HeartBeat, Volume);
	PlaySound(SOUND_LABEL_SE_HeartBeat);
}

void BeatTypeDecision(void)
{
	MATCH *match = GetMatch();
	LIGHTER *lighter = GetLighter();
	float Volume = 0.0f;
	if ((match->AblazeTime <= 0 && lighter->out != TRUE) && g_Player.life <= PLAYER_FEAR_LIFE)		//�Â��A�m���̂Ƃ�
	{
		g_BeatType = BEAT_VFAST;	
	}
	else if ((match->AblazeTime > 0 || lighter->out == TRUE) && g_Player.life <= PLAYER_FEAR_LIFE)	//���邭�A�m���̂Ƃ�
	{
		g_BeatType = BEAT_FAST;
	}
	else if ((match->AblazeTime <= 0 && lighter->out != TRUE) && g_Player.life <= PLAYER_ANXIE_LIFE) //�Â��A�̗͂������ȉ��̎�
	{
		g_BeatType = BEAT_MIDDLE;
	}
	else if ((match->AblazeTime > 0 || lighter->out == TRUE) && g_Player.life <= PLAYER_ANXIE_LIFE)	//���邭�A�̗͂������ȉ��̎�
	{
		g_BeatType = BEAT_SLOW;
	}
	else if ((match->AblazeTime <= 0 && lighter->out != TRUE) && g_Player.life < PLAYER_PEACE_LIFE)	//�Â��A�̗͂�80%�ȉ��̎�
	{
		g_BeatType = BEAT_VSLOW;
	}
	else if((match->AblazeTime > 0 || lighter->out == TRUE) && g_Player.life < PLAYER_PEACE_LIFE)	//���邭�A�̗͂�80%�ȏ�̎�
	{
		g_BeatType = BEAT_SHALLOW;
	}
	else
	{
		g_BeatType = BEAT_NON;																		//��L�S�Ăɓ��Ă͂܂�Ȃ�=�S���댯�ł͂Ȃ���
	}
}

//�v���C���[�̑�������炷
void PlayerBreath(void)
{
	//�V���̃u���X�^�C�v�𔻒肵�A��v���Ȃ��Ȃ瑦����SE��ύX
	int oldType = g_BreathType;
	BreathDicision();
	int nowType = g_BreathType;
	int SetSound = 0;
	if (oldType != nowType)
	{
		switch (oldType)
		{
		case BREATH_HYPERPNEA:
			SetSound = SOUND_LABEL_SE_Breath003;
			break;
		case BREATH_HARD:
			SetSound = SOUND_LABEL_SE_Breath001;
			break;
		case BREATH_SOFT:
			SetSound = SOUND_LABEL_SE_Breath002;
			break;
		case BREATH_NON:
			return;
		}
		SetSourceVolume(SetSound, 0.0f);	//���^�C�v�̑�����SE������
	}
	else
	{
		g_Player.BreathInterval--;
		if (g_Player.BreathInterval > 0)return;//�C���^�[�o�����J���؂��Ă��Ȃ��Ȃ珈�����Ȃ�
	}

	float Volume = 0.8f;
	switch (g_BreathType)
	{
	case BREATH_HYPERPNEA:
		SetSound = SOUND_LABEL_SE_Breath003;
		g_Player.BreathInterval = 70;
		break;
	case BREATH_HARD:
		SetSound = SOUND_LABEL_SE_Breath001;
		g_Player.BreathInterval = 180;
		break;
	case BREATH_SOFT:
		SetSound = SOUND_LABEL_SE_Breath002;
		g_Player.BreathInterval = 180;
		break;
	case BREATH_NON:
		g_Player.BreathInterval = 0;
		return;
	}
	SetSourceVolume(SetSound, Volume);
	PlaySound(SetSound);
}

//�������̃^�C�v���A�_�b�V����ԋy�ѐS����ԂŌ��߂�
void BreathDicision(void)
{
	if (g_Player.dash == TRUE)
	{
		if (g_BeatType >= BEAT_SHALLOW && g_BeatType < BEAT_SLOW)
			g_BreathType = BREATH_SOFT;			//�_�b�V�������S�������肵�Ă���Ȃ�y�����̂�
		else if (g_BeatType >= BEAT_SLOW && g_BeatType < BEAT_FAST)
			g_BreathType = BREATH_HARD;			//�_�b�V�������S�����s����Ȃ猃�������̂�
		else if (g_BeatType >= BEAT_FAST)
			g_BreathType = BREATH_HYPERPNEA;	//�_�b�V�����y�ѐS�����댯�Ȃ�ߌċz��
		else
			g_BreathType = BREATH_NON;			//����ȊO�̎��͖炳�Ȃ�
	}
	else
	{
		if (g_BeatType >= BEAT_VSLOW && g_BeatType < BEAT_MIDDLE)
			g_BreathType = BREATH_SOFT;			//�S�������肵�Ă���Ȃ�y�����̂�
		else if (g_BeatType >= BEAT_MIDDLE && g_BeatType < BEAT_VFAST)
			g_BreathType = BREATH_HARD;			//�S�����s����Ȃ猃�������̂�
		else if (g_BeatType >= BEAT_VFAST)
			g_BreathType = BREATH_HYPERPNEA;	//�S�����댯�Ȃ�ߌċz��
		else
			g_BreathType = BREATH_NON;			//����ȊO�̎��͖炳�Ȃ�
	}
}
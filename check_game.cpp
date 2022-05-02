//=============================================================================
//
// �Q�[���V�X�e������ [check_game.cpp]
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "match.h"
#include "lighter.h"
#include "collision.h"
#include "life.h"
#include "check_game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INV_TIME		(180)					//���G�t���[����
#define SAN_INV_TIME	(60)				//���C�x���G�t���[����
#define NATURALHEAL		(600)				//���R�񕜂��n�߂�܂ł̎���
#define SAN_PROTECT		(180)				//�ÈŃ_���[�W���󂯂�܂ł̎���
#define LIGHT_LIFE_CD	(60)				//���邳�ɂ�郉�C�t�̉e����CD
#define HEAL_CD			(60)				//���R�񕜂�CD
#define SANITY_DAMAGE_AREA	(200.0f)		//���C�x���_���[�W���󂯂�͈�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int playerInv = 0;	//�v���C���[���G����
static int SanInv = 0;	//���C�x�̖��G����
static int playerDamageLast = 0;	//�Ō�Ƀ_���[�W���󂯂Ă���̌o�ߎ���
static int sanProtect = SAN_PROTECT;
static int lightLife = LIGHT_LIFE_CD;
static int healTime = HEAL_CD;
//*****************************************************************************
// �X�V����
//*****************************************************************************
void CheckGame(void)
{
	//CheckLightOn();
	if (playerInv > 0)playerInv--;
	CheckPECollision();

	if (playerDamageLast > NATURALHEAL)
	playerDamageLast++;

	if (SanInv > 0)SanInv--;
	CheckPECollisionSan();

	if (lightLife > 0)lightLife--;
	LightCheck();

	if (healTime > 0)healTime--;
	LifeHeal();

}

//*****************************************************************************
// �`�揈��
//*****************************************************************************

//*****************************************************************************
// �X�V��������
//*****************************************************************************

//�v���C���[�ƃG�l�~�[�̓����蔻��
void CheckPECollision(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use != TRUE || playerInv > 0)continue;		//�G�l�~�[���g���Ă��Ȃ������G���Ԓ��Ȃ�X���[
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].fWidth))
		{
			AddLife(-5, PLAYER_LIFE, 0);	//���C�t�_���[�W
			AddSanity(-5, PLAYER_LIFE, 0);	//���C�x�_���[�W
			playerInv = INV_TIME;			//���������疳�G���Ԃ�ǉ�
			playerDamageLast = 0;			//�����������ߌo�ߎ��Ԃ�0��
		}
	}

}

//���Ԍo�߂Ő��C�x�������Ă�������
void CheckPECollisionSan(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use != TRUE || playerInv > 0)continue;		//�G�l�~�[���g���Ă��Ȃ������G���Ԓ��Ȃ�X���[
		if (CollisionBC(player->pos, enemy[i].pos, player->size + SANITY_DAMAGE_AREA, enemy[i].fWidth))
		{
			AddSanity(-1, PLAYER_LIFE, 0);	//���C�x�_���[�W
			SanInv = SAN_INV_TIME;
		}
	}

}

//���R�񕜂̏���
void LifeHeal(void)
{
	ENEMY *enemy = GetEnemy();
	if (healTime > 0)return;	//�񕜂�CD��������X���[
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].state == Chase)continue;
	}
	AddLife(5, PLAYER_LIFE, 0);	//���C�t��
	healTime = HEAL_CD;
}

//�Â��Ƃ��ɐ��C�x�������Ă�������
void LightCheck(void)
{
	MATCH *match = GetMatch();
	LIGHTER *lighter = GetLighter();
	if (lightLife > 0)return;	//CD���Ȃ�X���[
	if (lighter->out == FALSE && match->AblazeTime == 0)//���邭�Ȃ����̃`�F�b�N
	{
		if(sanProtect>0)sanProtect--;
	}
	else
	{
		if(sanProtect<SAN_PROTECT)sanProtect++;
	}
	if (sanProtect == 0)AddSanity(-5, PLAYER_LIFE, 0);	//���C�x�_���[�W
	if (sanProtect == SAN_PROTECT)AddSanity(5, PLAYER_LIFE, 0);	//���C�x��
	lightLife = LIGHT_LIFE_CD;
}
//=============================================================================
//
// ゲームシステム処理 [check_game.cpp]
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"

#include "collision.h"
#include "life.h"
#include "check_game.h"
#include "sound.h"

#include "interface.h"
#include "item.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INV_TIME		(120)					//無敵フレーム数
#define SAN_INV_TIME	(60)				//正気度無敵フレーム数
#define NATURALHEAL		(180)				//自然回復し始めるまでの時間
#define SAN_PROTECT		(60)				//暗闇ダメージを受けるまでの時間
#define LIGHT_LIFE_CD	(10)				//明るさによるライフの影響のCD
#define HEAL_CD			(120)				//自然回復のCD
#define SANITY_DAMAGE_AREA	(200.0f)		//正気度がダメージを受ける範囲
#define SAN_TO_LIFE_DMG		(25)			//正気度喪失によるライフダメージの浸食割合間隔
#define SAN_TO_LIFE_INT		(60)			//正気度喪失によるライフダメージの間隔

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int playerInv = 0;	//プレイヤー無敵時間
static int SanInv = 0;	//正気度の無敵時間
static int playerDamageLast = 0;	//最後にダメージを受けてからの経過時間
static int sanProtect = SAN_PROTECT;
static int lightLife = LIGHT_LIFE_CD;
static int healTime = HEAL_CD;
static int intStoL = SAN_TO_LIFE_INT;
//*****************************************************************************
// 更新処理
//*****************************************************************************
void CheckGame(void)
{
	if (GetPlayStage() == CLEAR_STAGE)return;
	PLAYER *player = GetPlayer();
	//CheckLightOn();
	if (playerInv > 0)playerInv--;
	CheckPECollision();

	if (playerDamageLast < NATURALHEAL)
	playerDamageLast++;

	if (SanInv > 0)SanInv--;
	CheckPECollisionSan();

	if (lightLife > 0)lightLife--;
	LightCheck();

	if (healTime > 0)healTime--;
	LifeHeal();

	CheckSan();

	CollisionItem(player->pos, player->size);
}

//*****************************************************************************
// 描画処理
//*****************************************************************************

//*****************************************************************************
// 更新内部処理
//*****************************************************************************

//プレイヤーとエネミーの当たり判定
void CheckPECollision(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use != TRUE || playerInv > 0)continue;		//エネミーが使われていないか無敵時間中ならスルー
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].fWidth))
		{
			PlaySound(SOUND_LABEL_SE_EnemyVoice);	//サウンド再生
			AddLife(-35, PLAYER_LIFE, 0);	//ライフダメージ
			AddSanity(-35, PLAYER_LIFE, 0);	//正気度ダメージ
			playerInv = INV_TIME;			//当たったら無敵時間を追加
			playerDamageLast = 0;			//当たったため経過時間を0に
			HurtAnimationOn();				//当たったらUIアニメーションをONにする
		}
	}

}

//時間経過で正気度が減っていく処理
void CheckPECollisionSan(void)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use != TRUE || SanInv > 0)continue;		//エネミーが使われていないか無敵時間中ならスルー
		if (CollisionBC(player->pos, enemy[i].pos, player->size + SANITY_DAMAGE_AREA, enemy[i].fWidth))
		{
			AddSanity(-1, PLAYER_LIFE, 0);	//正気度ダメージ
			SanInv = SAN_INV_TIME;
		}
	}

}

//自然回復の処理
void LifeHeal(void)
{
	ENEMY *enemy = GetEnemy();
	if (healTime > 0)return;	//回復のCDだったらスルー
	if (playerDamageLast < NATURALHEAL)return;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].state == Chase)return;
	}
	AddLife(5, PLAYER_LIFE, 0);	//ライフ回復
	healTime = HEAL_CD;
}

//暗いときに正気度が減っていく処理
void LightCheck(void)
{
	if (lightLife > 0)return;	//CD中ならスルー
	if (CheckLightOn() == FALSE)//明るくないかのチェック
	{
		if(sanProtect>0)sanProtect--;
	}
	else
	{
		if(sanProtect<SAN_PROTECT)sanProtect++;
	}

	if (sanProtect == 0)
	{
		AddSanity(-1, PLAYER_LIFE, 0);	//正気度ダメージ
		lightLife = LIGHT_LIFE_CD;
	}
	else if (sanProtect == SAN_PROTECT) 
	{
		AddSanity(1, PLAYER_LIFE, 0);	//正気度回復
		lightLife = LIGHT_LIFE_CD;
	}
}
//走ってないときスタミナ回復
void StaminaHeal(BOOL run)
{
	if (run == FALSE)
	{
		PLAYER *player = GetPlayer();

		if (player->staminaMax <= player->stamina)return;
			player->stamina++;
	}
}

//SAN値が低いほど、症状進行が深刻に
void CheckSan(void)
{
	PLAYER *player = GetPlayer();
	int f = player->sanity / SAN_TO_LIFE_DMG;
	//SAN値が75%以下で発症
	if (f <= 2 && CheckLightOn() == FALSE)
	{
		intStoL--;
		if (intStoL > 0)return;

		int x = -1 * (3 - f);
		AddLife(x, PLAYER_LIFE, 0);
		intStoL = SAN_TO_LIFE_INT;
		healTime = HEAL_CD;			//自然回復キャンセル
	}
}
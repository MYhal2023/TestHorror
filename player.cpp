//=============================================================================
//
// モデル処理 [player.cpp]
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
#include "check_game.h"
#include "particle.h"
#include "furniture.h"
#include "tutorial.h"
#include "item.h"
#include "itembox.h"
#include "enemy.h"
#include "stagefurniture.h"
#include "text_texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(1.5f)							// 移動量
#define	VALUE_DASH_MOVE		(3.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define PLAYER_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define PLAYER_SPAWN1_X		(-450.0f)						//FIRST_STAGE　スポーン値X
#define PLAYER_SPAWN1_Z		(-550.0f)						//FIRST_STAGE　スポーン値Z
#define PLAYER_OFFSET_Y		(20.0f)							// プレイヤーの足元をあわせる
#define PLAYER_OFFSET_Z		(-300.0f)							// プレイヤーの足元をあわせる
#define PLAYER_LIFE			(100)								// プレイヤーのライフ
#define PLAYER_STAMINA		(120)								// プレイヤーのスタミナ
#define PLAYER_STAMINA_INT	(3)								// スタミナ回復間隔
#define PLAYER_SANITY		(100)								// プレイヤーの正気度

#define PLAYER_PARTS_MAX	(1)								// プレイヤーのパーツの数
#define PLAYER_AT_FLAME		(30.0f)							// プレイヤーの攻撃フレーム
#define PLAYER_INVINC_FLAME	(120.0f)						// プレイヤー無敵フレーム
#define DEFER				(2.0f)							// プレイヤー移動制限の余剰幅

#define PLAYER_PEACE_LIFE (PLAYER_LIFE * 0.8)
#define PLAYER_ANXIE_LIFE (PLAYER_LIFE * 0.5)
#define PLAYER_FEAR_LIFE  (PLAYER_LIFE * 0.25)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;						// プレイヤー

static BOOL			g_Load = FALSE;
static int			atCount;
static float		angle = 0.0f;
static int			g_BeatType;
static int			g_BreathType;
// プレイヤーの階層アニメーションデータ
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
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;

	g_Player.pos = { 0.0f, PLAYER_OFFSET_Y, PLAYER_OFFSET_Z };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 0.8f, 1.0f, 1.0f };
	g_Player.moveVec = { 0.0f, 0.0f, 0.0f };
	g_Player.spd = 0.0f;			// 移動スピードクリア
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ
	g_Player.life = PLAYER_LIFE;
	g_Player.lifeMax = g_Player.life;
	g_Player.stamina = PLAYER_STAMINA;
#ifdef _DEBUG
	g_Player.stamina = 5000;
#endif
	g_Player.staminaMax = g_Player.stamina;
	g_Player.staminaInt = PLAYER_STAMINA_INT;
	g_Player.sanity = PLAYER_SANITY;
	g_Player.sanityMax = g_Player.sanity;

	g_Player.use = TRUE;
	g_Player.dash = FALSE;
	g_Player.batdash = TRUE;
	g_Player.attack = FALSE;
	g_Player.attackUse = FALSE;

	g_Player.atInvinc = FALSE;				//無敵時間中か否か
	g_Player.atInvincTime = 0.0f;			//時間計測用
	g_Player.blink = 0.0f;
	g_Player.blinking = 10.0f;
	g_Player.blinkingMax = 20.0f;
	g_Player.blinkingCount = 2.0f;
	g_Player.BeatSpeed = 1;
	g_Player.BreathInterval = 0;
	g_Player.WalkInterval = 0;
	g_Player.DashInterval = 0;
	// 階層アニメーション用の初期化処理
	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる

	g_BeatType = 0;
	g_BreathType = 0;
	g_Load = TRUE;
	atCount = 0;

	switch (GetPlayStage())
	{
	case PRISON_STAGE:
		g_Player.pos = { -350.0f, PLAYER_OFFSET_Y, -280.0f };
		break;
	case FIRST_STAGE:
		g_Player.pos = { PLAYER_SPAWN1_X, PLAYER_OFFSET_Y, PLAYER_SPAWN1_Z };
		break;
	case CLEAR_STAGE:
		g_Player.pos = { 0.0f, PLAYER_OFFSET_Y, -450.0f };
		break;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//プレイヤーの旧座標を保存
	float old_x = g_Player.pos.x;
	float old_z = g_Player.pos.z;

	CAMERA *cam = GetCamera();
	PlayerMoveControl();	//プレイヤーの移動操作	
	PlayerDashControl();	//ダッシュ操作
	PlayerDashProcess();	//ダッシュの処理
	SetChangeCamera();
	PlayerMoveSE();
	if (!GetCameraPos())g_Player.spd *= 0.5f;
	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		float angle = atan2f(g_Player.moveVec.x, g_Player.moveVec.z);
		g_Player.rot.y = angle;
	}
#ifdef _DEBUG

	if (GetKeyboardPress(DIK_5))
	{
		g_Player.spd *= 4.0f;
	}
#endif
	//移動値をベクトル変換して移動させる
	XMVECTOR moveVec = XMLoadFloat3(&g_Player.moveVec);
	XMVECTOR now = XMLoadFloat3(&g_Player.pos);								// 現在の場所
	XMStoreFloat3(&g_Player.pos, now + XMVector3Normalize(moveVec) * g_Player.spd);	//単位ベクトルを元に移動

	HeartBeat();	//心音のセット
	PlayerBreath(); //息遣いのセット
	MeshWallHit(g_Player.pos, g_Player.size, old_x, old_z);
	FunitureHit();
	PlayerLighterControl();
	if (g_Player.atInvinc == TRUE)	//被ダメージによる無敵中にすることは？
	{
		IncibleEffect();
		g_Player.atInvincTime += 1.0f;
		if (g_Player.atInvincTime >= PLAYER_INVINC_FLAME)//無敵時間を終了させる
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
	//体力の調整
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

	if (GetKeyboardPress(DIK_5))
	{
		g_Player.spd *= 10.0f;
	}
	PrintDebugProc("g_Player.life %d\n", g_Player.life);

	//if (GetKeyboardTrigger(DIK_B))
	//{
	//	SetParticle(g_Player.pos, XMFLOAT3(0.0f, 100.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 30.0f, 30.0f, 120);
	//}

#endif


#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	PrintDebugProc("Player:Stamina %d Player:san %d\n", g_Player.stamina, g_Player.sanity);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	//// カリング無効
	//SetCullingMode(CULL_MODE_NONE);

	//XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//// ワールドマトリックスの初期化
	//mtxWorld = XMMatrixIdentity();

	//// スケールを反映
	//mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// 回転を反映
	//mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y, g_Player.rot.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	////// クォータニオンを反映
	//XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.quaternion));
	//mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);
	//
	//// 移動を反映
	//mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//// ワールドマトリックスの設定
	//SetWorldMatrix(&mtxWorld);

	//XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	//SetFuchi(1);

	//// モデル描画
	//DrawModel(&g_Player.model);


	//SetFuchi(0);

	//// カリング設定を戻す
	//SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}


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
	if (g_Player.attack == TRUE || g_Player.life <= 0)	//攻撃中かゲームオーバーでなければ移動可能
		return;

	float changeRotCamera = 0.025f; //傾く速度
	// 移動ボタン
	if (IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_A))
	{	// 左へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x -= cosf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z += sinf(cam->rot.y) * changeRotCamera;
	}
	if (IsButtonPressed(0, BUTTON_RIGHT) || GetKeyboardPress(DIK_D))
	{	// 右へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x += cosf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z -= sinf(cam->rot.y) * changeRotCamera;
	}
	if (IsButtonPressed(0, BUTTON_UP) || GetKeyboardPress(DIK_W))
	{	// 上へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x += sinf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z += cosf(cam->rot.y) * changeRotCamera;
	}
	if (IsButtonPressed(0, BUTTON_DOWN) || GetKeyboardPress(DIK_S))
	{	// 下へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.moveVec.x -= sinf(cam->rot.y) * changeRotCamera;
		g_Player.moveVec.z -= cosf(cam->rot.y) * changeRotCamera;
	}
}

void PlayerMoveSE(void)
{
	if (!GetCameraPos())
	{
		SetSourceVolume(SOUND_LABEL_SE_Dash, 0.0f);
		SetSourceVolume(SOUND_LABEL_SE_Walk_sound, 0.0f);
		return;
	}
	//SEをどう鳴らすかは速度とWalkIntervalで判定。鳴らさない場合は変数ごとリセット
	if (g_Player.spd >= VALUE_MOVE && g_Player.WalkInterval == 0 && g_Player.spd < VALUE_DASH_MOVE)
	{
		SetSourceVolume(SOUND_LABEL_SE_Dash, 0.0f);
		SetSourceVolume(SOUND_LABEL_SE_Walk_sound, 1.0f);
		PlaySound(SOUND_LABEL_SE_Walk_sound);
		g_Player.WalkInterval++;
		g_Player.DashInterval = 0;
	}
	else if (g_Player.spd <= 0.1f)
	{
		SetSourceVolume(SOUND_LABEL_SE_Walk_sound, 0.0f);
		SetSourceVolume(SOUND_LABEL_SE_Dash, 0.0f);
		g_Player.WalkInterval = 0;
		g_Player.DashInterval = 0;
	}
	else if (g_Player.spd >= VALUE_MOVE && g_Player.WalkInterval != 0 && g_Player.spd < VALUE_DASH_MOVE)
	{
		g_Player.WalkInterval++;
		if (g_Player.WalkInterval >= 760)
			g_Player.WalkInterval = 0;
	}
	else if (g_Player.spd >= VALUE_DASH_MOVE && g_Player.DashInterval == 0)
	{
		SetSourceVolume(SOUND_LABEL_SE_Dash, 1.0f);
		SetSourceVolume(SOUND_LABEL_SE_Walk_sound, 0.0f);
		PlaySound(SOUND_LABEL_SE_Dash);
		g_Player.DashInterval++;
		g_Player.WalkInterval = 0;
	}
	else if (g_Player.spd >= VALUE_DASH_MOVE && g_Player.DashInterval != 0)
	{
		g_Player.DashInterval++;
		if (g_Player.DashInterval >= 660)
			g_Player.DashInterval = 0;
	}
}

void PlayerDashControl(void)
{
	//スタミナ0以下になったらダッシュできなくなる
	if (g_Player.stamina <= 0)g_Player.batdash = FALSE;	

	g_Player.staminaInt++;
	if (g_Player.staminaInt > PLAYER_STAMINA_INT)
	{
		StaminaHeal(g_Player.dash);	//スタミナ回復判定
		g_Player.staminaInt = 0;
	}
	//ダッシュ不可状態の場合、スタミナが最大値になったらダッシュ可能になる
	if (g_Player.batdash != TRUE && g_Player.stamina < g_Player.staminaMax || !GetCameraPos())
		return;
	else if (g_Player.batdash != TRUE && g_Player.stamina >= g_Player.staminaMax)
		g_Player.batdash = TRUE;

	if (IsButtonPressed(0, BUTTON_B) || GetKeyboardPress(DIK_Z))
		g_Player.dash = TRUE;
	else
		g_Player.dash = FALSE;
}

void PlayerLighterControl(void)
{
	if (ReturnSelect() != LIGHTER_ITEM || (!GetCameraPos()))
	{
		SetLighterOn(FALSE);
		return;
	}
	if (GetKeyboardPress(DIK_L) || (IsButtonPressed(0, BUTTON_L) && IsButtonPressed(0, BUTTON_A)))
	{
		SetLighterOn(TRUE);
	}
	else if (!GetKeyboardPress(DIK_L) || (!IsButtonPressed(0, BUTTON_L) || !IsButtonPressed(0, BUTTON_A)))
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
	if ((match->AblazeTime <= 0 && lighter->out != TRUE) && g_Player.life <= PLAYER_FEAR_LIFE)		//暗く、瀕死のとき
	{
		g_BeatType = BEAT_VFAST;	
	}
	else if ((match->AblazeTime > 0 || lighter->out == TRUE) && g_Player.life <= PLAYER_FEAR_LIFE)	//明るく、瀕死のとき
	{
		g_BeatType = BEAT_FAST;
	}
	else if ((match->AblazeTime <= 0 && lighter->out != TRUE) && g_Player.life <= PLAYER_ANXIE_LIFE) //暗く、体力が半分以下の時
	{
		g_BeatType = BEAT_MIDDLE;
	}
	else if ((match->AblazeTime > 0 || lighter->out == TRUE) && g_Player.life <= PLAYER_ANXIE_LIFE)	//明るく、体力が半分以下の時
	{
		g_BeatType = BEAT_SLOW;
	}
	else if ((match->AblazeTime <= 0 && lighter->out != TRUE) && g_Player.life < PLAYER_PEACE_LIFE)	//暗く、体力が80%以下の時
	{
		g_BeatType = BEAT_VSLOW;
	}
	else if((match->AblazeTime > 0 || lighter->out == TRUE) && g_Player.life < PLAYER_PEACE_LIFE)	//明るく、体力が80%以上の時
	{
		g_BeatType = BEAT_SHALLOW;
	}
	else
	{
		g_BeatType = BEAT_NON;																		//上記全てに当てはまらない=全く危険ではない時
	}
}

//プレイヤーの息遣いを鳴らす
void PlayerBreath(void)
{
	//新旧のブレスタイプを判定し、一致しないなら即座にSEを変更
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
		SetSourceVolume(SetSound, 0.0f);	//旧タイプの息遣いSEを消す
	}
	else
	{
		g_Player.BreathInterval--;
		if (g_Player.BreathInterval > 0)return;//インターバルが開き切っていないなら処理しない
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

//息遣いのタイプを、ダッシュ状態及び心音状態で決める
void BreathDicision(void)
{
	if (g_Player.dash == TRUE)
	{
		if (g_BeatType >= BEAT_SHALLOW && g_BeatType < BEAT_SLOW)
			g_BreathType = BREATH_SOFT;			//ダッシュ中かつ心音が安定しているなら軽いものを
		else if (g_BeatType >= BEAT_SLOW && g_BeatType < BEAT_FAST)
			g_BreathType = BREATH_HARD;			//ダッシュ中かつ心音が不安定なら激しいものを
		else if (g_BeatType >= BEAT_FAST)
			g_BreathType = BREATH_HYPERPNEA;	//ダッシュ中及び心音が危険なら過呼吸に
		else
			g_BreathType = BREATH_NON;			//それ以外の時は鳴らさない
	}
	else
	{
		if (g_BeatType >= BEAT_VSLOW && g_BeatType < BEAT_MIDDLE)
			g_BreathType = BREATH_SOFT;			//心音が安定しているなら軽いものを
		else if (g_BeatType >= BEAT_MIDDLE && g_BeatType < BEAT_VFAST)
			g_BreathType = BREATH_HARD;			//心音が不安定なら激しいものを
		else if (g_BeatType >= BEAT_VFAST)
			g_BreathType = BREATH_HYPERPNEA;	//心音が危険なら過呼吸に
		else
			g_BreathType = BREATH_NON;			//それ以外の時は鳴らさない
	}
}

void FunitureHit(void)
{
	if ((GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_X)))
	{
		MODEL_ITEM *item = GetItem();
		FURNITURE *fur = GetFurniture();
		STAGEFURNITURE *sfur = GetStageFurniture();
		switch (GetPlayStage())
		{
		case PRISON_STAGE:
			if (ReturnSelect() != KEY)return;
			for (int i = 0; i < MAX_FURNITURE; i++)
			{
				if (fur[i].use == FALSE)continue;
				if (CollisionBC(g_Player.pos, fur[i].pos, g_Player.size, fur[i].size) &&
					CheckDoorKey(i))
				{
					fur[i].use = FALSE;
					UseItemSelect();
					PlaySound(SOUND_LABEL_SE_Door_Open);
					SetTexture(TEXT_USE_KEY);
					break;
				}
			}
			break;
		case FIRST_STAGE:
			for (int i = 0; i < MAX_STAGEFURNITURE; i++)
			{
				if (sfur[i].ID == 99 || sfur[i].use == FALSE)continue;	//IDが99の場合はドアではないため次へ

				if (sfur[i].ID == 100 && CollisionBC(g_Player.pos, sfur[i].pos, g_Player.size, sfur[i].size) &&
					CheckDoorKey(sfur[i].ID) && (ReturnSelect() == KEY))
				{
					sfur[i].use = FALSE;
					DeleteMeshWall(50);
					UseItemSelect();
					PlaySound(SOUND_LABEL_SE_Door_Open);
					SetTexture(TEXT_USE_KEY);
					break;
				}
				else if (sfur[i].ID != 100 && CollisionBC(g_Player.pos, sfur[i].pos, g_Player.size, sfur[i].size))
				{
					if (sfur[i].ID == 1)//鍵付きドア
					{
						if (CheckDoorKey(sfur[i].ID) && (ReturnSelect() == KEY))
						{
							sfur[i].use = FALSE;
							DeleteMeshWall(40 + sfur[i].ID);
							UseItemSelect();
							PlaySound(SOUND_LABEL_SE_Door_Open);
							SetTexture(TEXT_USE_KEY);
							break;
						}
						PlaySound(SOUND_LABEL_SE_Door_Close);
						SetTexture(TEXT_NEED_KEY);
					}
					else {
						sfur[i].use = FALSE;
						DeleteMeshWall(40 + sfur[i].ID);
						PlaySound(SOUND_LABEL_SE_Door_Open);
						break;
					}
				}
			}
			break;
		}
	}
}
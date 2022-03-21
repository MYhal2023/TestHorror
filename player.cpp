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
#include "collision.h"
#include "time.h"
#include "sound.h"
#include "lighter.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(1.0f)							// 移動量
#define	VALUE_DASH_MOVE		(2.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define PLAYER_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(0.0f)							// プレイヤーの足元をあわせる
#define PLAYER_OFFSET_Z		(-300.0f)							// プレイヤーの足元をあわせる
#define PLAYER_LIFE			(100)								// プレイヤーのライフ
#define PLAYER_STAMINA		(100)								// プレイヤーのスタミナ

#define PLAYER_PARTS_MAX	(1)								// プレイヤーのパーツの数
#define PLAYER_AT_FLAME		(30.0f)							// プレイヤーの攻撃フレーム
#define PLAYER_INVINC_FLAME	(120.0f)						// プレイヤー無敵フレーム
#define DEFER				(2.0f)							// プレイヤー移動制限の余剰幅
#define PLAYER_MIN_X	(-(FIELD_X / 2.0f) * BLOCK_SIZE + DEFER * BLOCK_SIZE)				// プレイヤー移動上限
#define PLAYER_MIN_Z	((FIELD_Z / 2.0f) * BLOCK_SIZE - (FIELD_Z - DEFER) * BLOCK_SIZE)	// 
#define PLAYER_MAX_X	(-(FIELD_X / 2.0f) * BLOCK_SIZE + (FIELD_X - DEFER) * BLOCK_SIZE )	// 
#define PLAYER_MAX_Z	((FIELD_Z / 2.0f) * BLOCK_SIZE - DEFER * BLOCK_SIZE)				// 

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
	g_Player.stamina = PLAYER_LIFE;
	g_Player.staminaMax = g_Player.stamina;
	g_Player.use = TRUE;
	g_Player.dash = FALSE;
	g_Player.attack = FALSE;
	g_Player.attackUse = FALSE;

	g_Player.atInvinc = FALSE;				//無敵時間中か否か
	g_Player.atInvincTime = 0.0f;			//時間計測用
	g_Player.blink = 0.0f;
	g_Player.blinking = 10.0f;
	g_Player.blinkingMax = 20.0f;
	g_Player.blinkingCount = 2.0f;


	// 階層アニメーション用の初期化処理
	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる

	g_Load = TRUE;
	atCount = 0;
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
	//SettingPlayer();	//レイキャストとクォータニオン処理

	//// モデルの色を変更
	//for (int i = 0; i < g_Player.model.SubsetNum; i++)
	//{
	//	SetModelDiffuse(&g_Player.model, i, XMFLOAT4(1.0f, 1.0f - g_Player.blink, 1.0f - g_Player.blink, 1.0f));
	//}

	//プレイヤーの旧座標を保存
	float old_x = g_Player.pos.x;
	float old_z = g_Player.pos.z;

	CAMERA *cam = GetCamera();
	PlayerMoveControl();	//プレイヤーの移動操作	
	PlayerDashControl();	//ダッシュ操作
	PlayerDashProcess();	//ダッシュの処理

	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		float angle = atan2f(g_Player.moveVec.x, g_Player.moveVec.z);
		g_Player.rot.y = angle;
	}
	//移動値をベクトル変換して移動させる
	XMVECTOR moveVec = XMLoadFloat3(&g_Player.moveVec);
	XMVECTOR now = XMLoadFloat3(&g_Player.pos);								// 現在の場所
	XMStoreFloat3(&g_Player.pos, now + XMVector3Normalize(moveVec) * g_Player.spd);	//単位ベクトルを元に移動



	//メッシュフィールド範囲外に出ないようにする
	if (g_Player.pos.x <= PLAYER_MIN_X ||
		g_Player.pos.x >= PLAYER_MAX_X ||
		g_Player.pos.z <= PLAYER_MIN_Z ||
		g_Player.pos.z >= PLAYER_MAX_Z)
	{
		g_Player.pos.x = old_x;
		g_Player.pos.z = old_z;
	}

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
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	PrintDebugProc("Player:Stamina %d Player:spd %f\n", g_Player.stamina, g_Player.spd);
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

//void SettingPlayer(void)
//{
//	// レイキャストして足元の高さを求める
//	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// ぶつかったポリゴンの法線ベクトル（向き）
//	XMFLOAT3 hitPosition;								// 交点
//	hitPosition.y = g_Player.pos.y - PLAYER_OFFSET_Y;	// 外れた時用に初期化しておく
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
//// 姿勢制御
////////////////////////////////////////////////////////////////////////
//
//	XMVECTOR vx, nvx, up;
//	XMVECTOR quat;
//	float len, angle;
//
//	// ２つのベクトルの外積を取って任意の回転軸を求める
//	g_Player.upVector = normal;
//	up = { 0.0f, 1.0f, 0.0f, 0.0f };
//	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));	//これが任意の軸
//
//	// 求めた回転軸からクォータニオンを作り出す
//	nvx = XMVector3Length(vx);
//	XMStoreFloat(&len, nvx);
//	nvx = XMVector3Normalize(vx);
//	angle = asinf(len);
//	quat = XMQuaternionRotationNormal(nvx, angle);
//
//	// 前回のクォータニオンから今回のクォータニオンまでの回転を滑らかにする
//	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.2f);		//第三引数は要はパーセンテージ。この場合1fに5%ずつ傾けさせてる
//
//	// 今回のクォータニオンの結果を保存する
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
	if (g_Player.attack == TRUE || GetTime() <= 0 || g_Player.life <= 0)	//攻撃中かゲームオーバーでなければ移動可能
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

void PlayerDashControl(void)
{
	//スタミナ0以下はダッシュできない
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
	if (g_Player.life >= PLAYER_PEACE_LIFE)return;

	if (g_Player.life >= PLAYER_ANXIE_LIFE)
	{
		SetSourceVolume(SOUND_LABEL_SE_HeartBeat, 1.0f);
	}
}
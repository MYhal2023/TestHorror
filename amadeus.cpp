//=============================================================================
//
// エネミーAI処理 [amadeus.cpp]
// Author : 米倉睦起
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "amadeus.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "time.h"
#include "score.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EYE_VALUE		(3.0f)				//視点の乗算評価値
#define BACK_VALUE		(0.5f)				//背面の乗算評価値
//ここからAIに使用する定義
#define EYE_ANGLE		(XM_PI * 0.25f)		//1方向の角度
#define ENEMY_VISION	(0.25f)				//エネミーの視界
#define ANGLE_TYPE		(4)					//方角の数
#define AI_ROCK_SIZE	(4)					//岩の侵入不可のサイズ
//TCP用の定数命名
#define AI_WAYAREA_SIZE	(3)					//ウェイポイント生成用定数
#define WAYAREA_SIZE	((AI_WAYAREA_SIZE * 2) - 1)	//ウェイポイント生成エリアの一辺のポイント数(グリッド状の正方形)
#define NEAR_ENEMY		(40.0f)				//ウェイポイントに近いとはどれくらいの距離を指すのか定義
#define VALUE_VAR		(3)					//評価対象の数
#define EYE_VALUE_TPS	(1.2f)				//視点の評価係数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static WAYPOINT			g_WayPoint[WAYAREA_SIZE][WAYAREA_SIZE];// ウェイポイント

static BOOL				g_Load = FALSE;

static float			valueCoe[VALUE_VAR];					//評価係数の保存先
//=============================================================================
// 初期化処理(全ての初期化処理の後に実行させること)
//=============================================================================
HRESULT InitAmadeus(void)
{
	for (int i = 0; i < WAYAREA_SIZE; i++)
	{
		for (int k = 0; k < WAYAREA_SIZE; k++)
		{
			g_WayPoint[i][k].pos = { 0.0f, 0.0f, 0.0f };			//座標
			g_WayPoint[i][k].rot = { 0.0f, 0.0f, 0.0f };			// 角度
			g_WayPoint[i][k].scl = { 1.0f, 1.0f, 1.0f };			// スケール
			g_WayPoint[i][k].value = 1.0f;			//評価値
			g_WayPoint[i][k].use = FALSE;			//使用の可否(TRUEで使用可)
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAmadeus(void)
{
	if (g_Load == FALSE) return;


	g_Load = FALSE;
}

//========================================================================================================================
// 対象を目視出来ているのか(引数1:自分の位置,引数2:対象の位置,引数3:自分の角度,引数4:どれくらいの距離なら目視できるのか)
//========================================================================================================================
BOOL Visibility(XMFLOAT3 pos1, XMFLOAT3 pos2, float rot, float dist)
{
	BOOL ans = FALSE;

	float len = (dist) * (dist);		// 半径を2乗した物
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2点間の距離（2乗した物）
	float lenSq = 0.0f;
	XMStoreFloat(&lenSq, temp);
	// 半径を2乗した物より距離が長い
	if (len <= lenSq)
	{
		return ans;	// 目視外にあるためこの時点で計算終了
	}

	//対象との角度を算出
	float angleX = pos2.x - pos1.x;
	float angleZ = pos2.z - pos1.z;
	float angle = atan2f(angleX, angleZ);
	//自身の角度を8方向で算出
	float stVision = 0.0f;
	float edVision = 0.0f;
	//angleがXM_PI～-XMPIなのでそれに合わせる
	//視界が値の切れ目に挟まると少し面倒な処理が必要
	if ((rot > XM_PI * (1.0f - ENEMY_VISION)) || (rot < XM_PI * (ENEMY_VISION - 1.0f)))
	{
		if (rot > XM_PI * (1.0f - ENEMY_VISION))
		{
			stVision = rot - XM_PI * ENEMY_VISION;
			edVision = (rot + XM_PI * ENEMY_VISION) - (XM_PI * 2.0f);
		}
		else if (rot < XM_PI * (ENEMY_VISION - 1.0f))
		{
			stVision = rot - XM_PI * ENEMY_VISION + (XM_PI * 2.0f);
			edVision = rot + XM_PI * ENEMY_VISION;
		}

		if (angle >= stVision || angle <= edVision)
		{
			ans = TRUE;
		}
	}
	else
	{
		stVision = rot - XM_PI * ENEMY_VISION;
		edVision = rot + XM_PI * ENEMY_VISION;
		if (angle >= stVision && angle <= edVision)
		{
			ans = TRUE;
		}
	}

	return ans;
}

XMVECTOR TacticalPointSystem(int i)	//戦術位置解析システム。配列番号を持ってくること
{
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();
	int stWayArea = 1 - AI_WAYAREA_SIZE;
	int edWayArea = AI_WAYAREA_SIZE;
	//左下から右上にかけてウェイポイントを決定。初期化
	for (int h = stWayArea; h < edWayArea; h++)
	{
		for (int k = stWayArea; k < edWayArea; k++)
		{
			int a = h + (-stWayArea);
			int b = k + (-stWayArea);
			float x = enemy[i].pos.x + POINT_INTERVAL * k;
			float y = 0.0f;
			float z = enemy[i].pos.z + POINT_INTERVAL * h;
			g_WayPoint[a][b].pos = { x, y, z };
			//座標決定と共にフィルタリングフェーズへ
			g_WayPoint[a][b].use = FilterWayPoint(g_WayPoint[a][b].pos, enemy[i].pos, i);
			g_WayPoint[a][b].value = 1.0f;


			//ウェイポイント評価フェーズ
			if (g_WayPoint[a][b].use == TRUE)
				g_WayPoint[a][b].value = ValueWayPoint(g_WayPoint[a][b].pos, 0.0f, i);

			////デバッグ時、分かりやすいように色覚を変える
			//if (g_WayPoint[a][b].use == TRUE)
			//	g_WayPoint[a][b].material.Diffuse = { 0.0f + g_WayPoint[a][b].value, 0.0f, 0.0f, 1.0f };
			//else
			//	g_WayPoint[a][b].material.Diffuse = { 0.0f, 0.0f, 1.0f, 1.0f };
		}
	}

	//残ったウェイポイントの評価値を比較
	float valueMax = 0.0f;		//最大評価値を保存
	int value_x = 0;			//最大評価値の配列番号を保存
	int value_z = 0;			//
	for (int a = 0; a < WAYAREA_SIZE; a++)
	{
		for (int b = 0; b < WAYAREA_SIZE; b++)
		{
			if (g_WayPoint[a][b].use == FALSE)continue;	//FALSEの場合、削除したウェイポイントなので処理をスキップ

			if (valueMax < g_WayPoint[a][b].value)
			{
				valueMax = g_WayPoint[a][b].value;
				value_z = a;
				value_x = b;
			}
		}
	}
	//g_WayPoint[value_z][value_x].material.Diffuse = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMVECTOR vec = { 0.0f, 0.0f, 0.0f };
	if (valueMax == 0.0f) //どこにも移動候補が無い場合はそのまま返す
		return vec;

	//移動量
	float vecX = g_WayPoint[value_z][value_x].pos.x - enemy[i].pos.x;
	float vecZ = g_WayPoint[value_z][value_x].pos.z - enemy[i].pos.z;
	//X軸とZ軸次元での移動ベクトルを計算
	g_WayPoint[value_z][value_x].pos.y = 0.0f;
	enemy[i].pos.y = 0.0f;
	vec = XMLoadFloat3(&g_WayPoint[value_z][value_x].pos) - XMLoadFloat3(&enemy[i].pos);
	//単位ベクトル化。これにエネミーのスピードをかける
	vec = XMVector3Normalize(vec);
	enemy[i].moveVec = vec;

	//角度を算出し変更
	float angle = atan2f(vecX, vecZ);
	enemy[i].rot.y = angle;
	return vec;
}

//ウェイポイントをフィルタリングする関数。ポイントの座標と自身の配列番号も持ってくる
BOOL FilterWayPoint(XMFLOAT3 pos, XMFLOAT3 pos2, int k)		//ウェイポイント、自身の現座標、自身の配列番号
{
	BOOL ans = TRUE;							//まずはTRUEをセット。ここから何にも引っかからなければTRUEが返り、ウェイポイントとして使用する
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();
	XMFLOAT3 checkPos = {player->pos.x, 0.0f, player->pos.z};


	//侵入不可の場所を削除
	for (int i = 0; i < GetMeshWallNum(); i++)
	{

		if (CheckCrossLine(pos2, pos, GetMeshWallStPosition(i), GetMeshWallEdPosition(i)) == TRUE)
		{
			return ans = FALSE;
		}
	}
	return ans;
}

//ウェイポイントを評価する関数。自身の配列番号も持ってくる
//引数1:ウェイポイント座標、引数2:最初にある評価値、引数3:配列番号
float ValueWayPoint(XMFLOAT3 pos, float value, int k)
{
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();

	XMVECTOR disP = XMLoadFloat3(&player->pos) - XMLoadFloat3(&pos);
	disP = XMVector3Length(disP);				//距離を2乗
	float point;
	XMStoreFloat(&point, disP);
	if (point < 1.0f)				//座標差分が1.0以下なら評価最大値を出力
	{
		value += 1.0f;
	}
	else
	{
		point = 1.0f / point;						//離れているほど係数を小さくなるようにする。
		value += 1.0f * point;		//評価係数と距離の評価を乗算
	}

	//if (Visibility(enemy[k].pos, pos, enemy[k].rot.y, POINT_INTERVAL * POINT_INTERVAL) == TRUE)
	//{
	//	value *= EYE_VALUE_TPS;
	//}

	return value;
}
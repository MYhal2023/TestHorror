//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author :米倉睦起
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(3)			// テクスチャの数

#define	ENEMY_TEXMAG	(0.08f)							// 元画像に対する倍率
#define	ENEMY_WIDTH		(650.0f * ENEMY_TEXMAG)			// 頂点サイズ
#define	ENEMY_HEIGHT	(812.0f * ENEMY_TEXMAG)			// 頂点サイズ
#define ENEMY_SIGHT		(400.0f)		//エネミーの視力
#define	ENEMY_SPEED		(1.0f)			// エネミーの移動スピード


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static ENEMY						g_Enemy[MAX_ENEMY];	// バレットワーク

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
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	MakeVertexEnemy();

	// テクスチャ生成
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

	// エネミーの初期化
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

		g_Enemy[i].move_time = 0.0f;	// 線形補間用のタイマーをクリア
		g_Enemy[i].tbl_adr = NULL;		// 再生するアニメデータの先頭アドレスをセット
		g_Enemy[i].tbl_size = 0;		// 再生するアニメデータのレコード数をセット
	}

	//線形補間データセット
	g_Enemy[0].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[0].tbl_adr = move_tbl;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{// テクスチャの解放
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if (g_VertexBuffer != NULL)
	{// 頂点バッファの解放
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use != TRUE)	//使われてないエネミーは処理をスキップ
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

		//エネミーのステート処理
		int oldState = g_Enemy[i].state;
		g_Enemy[i].state = StateCheck(i);
		int nowState = g_Enemy[i].state;
		//ステートが遷移した場合は線形補間データの保存などを行う
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
			//AI処理の元、移動ベクトルを決定
			g_Enemy[i].moveVec = TacticalPointSystem(i);
			break;
		}

		//移動値をベクトル変換して移動させる
		XMVECTOR moveVec = g_Enemy[i].moveVec;
		XMVECTOR now = XMLoadFloat3(&g_Enemy[i].pos);								// 現在の場所
		XMStoreFloat3(&g_Enemy[i].pos, now + XMVector3Normalize(moveVec) * g_Enemy[i].spd);	//単位ベクトルを元に移動

		//デバッグ中なら画像の入れ替えが可能(怖いやつと怖くないやつに入れ替え可能！)
#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F))
			g_Enemy[i].texNo = (g_Enemy[i].texNo == ENEMY_WOMAN) ? ENEMY_DEBUG : ENEMY_WOMAN;
#endif
	}
	//エネミーのセットを行える
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_G))
		SetEnemy(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	// αテストを有効に
	SetAlphaTestEnable(TRUE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxTranslate, mtxRot, mtxWorld;
	CAMERA *cam = GetCamera();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);


	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_Enemy[i].use)continue;

		EnemyAngleTex(i);

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		// マテリアル設定
		SetMaterial(g_Enemy[i].material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

		// ポリゴンの描画
		GetDeviceContext()->Draw(4, 0);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);

	// ライティングを有効に
	SetLightEnable(TRUE);

	// αテストを無効に
	SetAlphaTestEnable(FALSE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEnemy(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = ENEMY_WIDTH;
	float fHeight = ENEMY_HEIGHT;

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, 0.0f, 0.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// エネミーのパラメータをセット
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
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &(g_Enemy[0]);
}

//プレイヤーの視点から見たエネミーの前後判別
void EnemyAngleTex(int i)
{
	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();

	//角度θを利用してエネミーの方向ベクトルを作成
	float buffer = 1.0f;
	XMFLOAT3 pos1 = { g_Enemy[i].pos.x + sinf(g_Enemy[i].rot.y * buffer), 0.0f, g_Enemy[i].pos.z + cosf(g_Enemy[i].rot.y * buffer) };
	XMFLOAT3 pos2 = { g_Enemy[i].pos.x, 0.0f, g_Enemy[i].pos.z };
	XMVECTOR v1 = XMVector3Normalize(XMLoadFloat3(&pos1) - XMLoadFloat3(&g_Enemy[i].pos)); //正規化した向きベクトル

	//カメラからエネミーに向かうベクトルを作成
	XMFLOAT3 playerEdPos = { g_Enemy[i].pos.x, 0.0f, g_Enemy[i].pos.z };
	XMFLOAT3 playerStPos = { cam->pos.x, 0.0f, cam->pos.z };
	XMVECTOR v2 = XMVector3Normalize(XMLoadFloat3(&playerEdPos) - XMLoadFloat3(&playerStPos)); //正規化したベクトル

	//2ベクトルの内積を求め、内積の正負によって画像を差し替え(裏表を反映させるため)
	float ECdot = dotProduct(&v1, &v2);
	if (ECdot <= 0.0f)
		g_Enemy[i].texNo = ENEMY_WOMAN;
	else
		g_Enemy[i].texNo = ENEMY_BACK;
}

int StateCheck(int i)
{
	PLAYER *player = GetPlayer();
	int ans = Patrol;			//デフォルトは巡回モード
	//プレイヤーを視界に捉えたか
	if (Visibility(g_Enemy[i].pos, player->pos, g_Enemy[i].rot.y, ENEMY_SIGHT) == TRUE ||
		 CollisionBC(player->pos, g_Enemy[i].pos, player->size, g_Enemy[i].fWidth) == TRUE)
	{
		ans = Chase;	//追跡開始
	}

	return ans;
}

void EnemyInterPoration(int i)
{
	if (g_Enemy[i].tbl_adr == NULL)return;	// 線形補間を実行する？
							// 線形補間の処理
	// 移動処理
	int		index = (int)g_Enemy[i].move_time;
	float	time = g_Enemy[i].move_time - index;
	int		size = g_Enemy[i].tbl_size;

	float dt = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1フレームで進める時間
	g_Enemy[i].move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、最初へ戻す
	{
		g_Enemy[i].move_time = 0.0f;
		index = 0;
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&g_Enemy[i].pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

}

//ステート遷移に伴う情報整合を行う
void StateAdjust(int i)
{
	int index = (int)g_Enemy[i].move_time;

	switch (g_Enemy[i].state)
	{
	case Patrol:	//追跡から巡回へ
		//BGM遷移処理
		SetSoundFade(SOUND_LABEL_BGM_Fear, SOUNDFADE_OUT, 0.0f, 3.0f);
		if (g_Enemy[i].tbl_adr == NULL)return;	// 線形補間を実行する？

		// 線形補間データから座標情報の抽出
		XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index].pos);	// 現在の場所
		XMStoreFloat3(&g_Enemy[i].pos, p0);

		break;

	case Chase:		//巡回から追跡へ
		StopSound();
		float Volume = 0.5f;
		SetSourceVolume(SOUND_LABEL_BGM_Fear, Volume);
		PlaySound(SOUND_LABEL_BGM_Fear);
		break;
	}
}
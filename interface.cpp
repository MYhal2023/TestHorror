//=============================================================================
//
// その他UI表示処理 [interface.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "interface.h"
#include "itembox.h"
#include "sprite.h"
#include "game.h"
#include "time.h"
#include "player.h"
#include "lighter.h"
#include "match.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(12)				// テクスチャの数

#define PLAYER_HURT_FRAME			(120)			// プレイヤー無敵フレーム


#define HP_TEXTURE_WIDTH			(3200/5)	// テクスチャサイズ
#define HP_TEXTURE_HEIGHT			(80/5)	// 
#define	HP_X						(80)			// テクスチャ座標
#define HP_Y						(50)

#define OIL_TEXTURE_WIDTH			(100/5)	// テクスチャサイズ
#define OIL_TEXTURE_HEIGHT			(600/5)	// 
#define OIL_X						(SCREEN_WIDTH-50)	// テクスチャ座標
#define OIL_Y						(SCREEN_HEIGHT-100)					// 

#define STAM_TEXTURE_WIDTH			(300)	// テクスチャサイズ
#define STAM_TEXTURE_HEIGHT			(10)	// 
#define	STAM_X						(HP_X)			// テクスチャ座標
#define STAM_Y						(HP_Y+20)

#define MIND_TEXTURE_WIDTH			(350)	// テクスチャサイズ
#define MIND_TEXTURE_HEIGHT			(20)	// 
#define	MIND_X						(HP_X)			// テクスチャ座標
#define MIND_Y						(HP_Y+40)

#define ITEM_BOX_TEXTURE_WIDTH		(40)	// テクスチャサイズ
#define ITEM_BOX_TEXTURE_HEIGHT		(35)	// 
#define	ITEM_BOX_X					(SCREEN_WIDTH/2 - 50)			// テクスチャ座標
#define ITEM_BOX_Y					(SCREEN_HEIGHT-40)

#define MATCH_X						(50)
#define MATCH_Y						(SCREEN_HEIGHT-80)

#define MATCH_TEXTURE_WIDTH			(40*2)
#define MATCH_TEXTURE_HEIGHT		(20*2)
#define MATCH_NUM_TEXTURE_WIDTH		(25)	// テクスチャサイズ
#define MATCH_NUM_TEXTURE_HEIGHT	(50)	// 

#define MATCH_DIGIT					(2)			// 桁数

#define HUNDRED						(100.0f)				//フロート型100
#define	DEVIATION_HP				(0.8f)					//ゲージの移動を訂正するため
#define DEVIATION_OIL				(0.6f)
#define DEVIATION_STAM				(0.8f)
#define DEVIATION_MIND				(0.6f)

#define WAIT_TIME					(50)					//ゲージアニメーションを待たせる時間


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/hpbarred.png",
	"data/TEXTURE/hpbarred.png",
	"data/TEXTURE/hpbar.png",
	"data/TEXTURE/stamina1.png",				//後で追加してくださいね
	"data/TEXTURE/oilbarred.png",
	"data/TEXTURE/oilbar.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/match_ui.png",
	"data/TEXTURE/mind.png",
	"data/TEXTURE/hurt.png",
	"data/TEXTURE/lowmind.png",
	"data/TEXTURE/itembox.png",
};


static UI_ELEMENT	g_UI[TEXTURE_MAX];
static UI_ELEMENT	g_ItemBox[ITEM_MAX];

static BOOL						g_Load = FALSE;
static int						g_match = 0;
static int						g_time = 0;
static int						g_hurt_time = 0;
static int						g_ItemMax = ITEM_MIN;			//現在アイテムを持てる限界


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitInterface(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < TEXTURE_MAX - 1; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].TexNo = i;
		g_UI[i].tw = 1.0f;		// テクスチャの幅
		g_UI[i].th = 1.0f;		// テクスチャの高さ
		g_UI[i].tx = 0.0f;			// テクスチャの左上X座標
		g_UI[i].ty = 0.0f;			// テクスチャの左上Y座標
		g_UI[i].color = { 1.0f,1.0f,1.0f,1.0f };
	}
	//HPゲージ
	g_UI[HP_BAR].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_BAR].w = HP_TEXTURE_WIDTH;
	g_UI[HP_BAR].h = HP_TEXTURE_HEIGHT;

	//HP　背景
	g_UI[HP_RED_BG].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_RED_BG].w = HP_TEXTURE_WIDTH;
	g_UI[HP_RED_BG].h = HP_TEXTURE_HEIGHT;
	g_UI[HP_RED_BG].color = { 15.0f,0.7f,1.0f,1.0f };

	//HP　色
	g_UI[HP_RED].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_RED].w = HP_TEXTURE_WIDTH;
	g_UI[HP_RED].h = HP_TEXTURE_HEIGHT;
	g_UI[HP_RED].color = { 0.0f,5.0f,2.0f,1.0f };

	//オイルゲージ
	g_UI[OIL_BAR].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_BAR].w = OIL_TEXTURE_WIDTH;
	g_UI[OIL_BAR].h = OIL_TEXTURE_HEIGHT;

	//オイル色
	g_UI[OIL_RED].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_RED].w = OIL_TEXTURE_WIDTH;
	g_UI[OIL_RED].h = OIL_TEXTURE_HEIGHT;

	//マッチ数字
	g_UI[MATCH_NUM].pos = { MATCH_X + MATCH_TEXTURE_WIDTH, MATCH_Y, 0.0f };
	g_UI[MATCH_NUM].w = MATCH_NUM_TEXTURE_WIDTH;
	g_UI[MATCH_NUM].h = MATCH_NUM_TEXTURE_HEIGHT;

	//マッチUI
	g_UI[MATCH_PIC].pos = { MATCH_X, MATCH_Y, 0.0f };
	g_UI[MATCH_PIC].w = MATCH_TEXTURE_WIDTH;
	g_UI[MATCH_PIC].h = MATCH_TEXTURE_HEIGHT;

	//Sanityゲージ
	g_UI[MIND].pos = { MIND_X, MIND_Y, 0.0f };
	g_UI[MIND].w = MIND_TEXTURE_WIDTH;
	g_UI[MIND].h = MIND_TEXTURE_HEIGHT;

	//痛みエフェクト
	g_UI[HURT].use = FALSE;
	g_UI[HURT].pos = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f };
	g_UI[HURT].w = SCREEN_WIDTH;
	g_UI[HURT].h = SCREEN_HEIGHT;
	g_UI[HURT].color = { 0.3f,0.0f,0.0f,0.0f };

	//Sanityエフェクト
	g_UI[INSANE].use = FALSE;
	g_UI[INSANE].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	g_UI[INSANE].w = SCREEN_WIDTH;
	g_UI[INSANE].h = SCREEN_HEIGHT;
	g_UI[INSANE].color = { 1.0f,0.0f,0.0f,0.0f };

	//スタミナバー
	g_UI[STAM_BAR].pos = { STAM_X , STAM_Y , 0.0f };
	g_UI[STAM_BAR].w = STAM_TEXTURE_WIDTH;
	g_UI[STAM_BAR].h = STAM_TEXTURE_HEIGHT;
	g_UI[STAM_BAR].color = { 0.0f,1.0f,0.0f,1.0f };


	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (i >= g_ItemMax)
		{
			g_ItemBox[i].use = FALSE;
		}
		else
		{
			g_ItemBox[i].use = TRUE;
		}
		g_ItemBox[i].TexNo = ITEM_BOX;
		g_ItemBox[i].tw = 1.0f;		// テクスチャの幅
		g_ItemBox[i].th = 1.0f;		// テクスチャの高さ
		g_ItemBox[i].tx = 0.0f;			// テクスチャの左上X座標
		g_ItemBox[i].ty = 0.0f;			// テクスチャの左上Y座標
		g_ItemBox[i].pos = { ITEM_BOX_X + (ITEM_BOX_TEXTURE_WIDTH * i)*1.0f,ITEM_BOX_Y, 0.0f };
		g_ItemBox[i].w = ITEM_BOX_TEXTURE_WIDTH;
		g_ItemBox[i].h = ITEM_BOX_TEXTURE_HEIGHT;
		g_ItemBox[i].color = { 1.0f,1.0f,1.0f,1.0f };
	}


	// 初期化



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitInterface(void)
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
// 更新処理
//=============================================================================
void UpdateInterface(void)
{
	PLAYER player = *GetPlayer();
	LIGHTER lighter = *GetLighter();
	MATCH match = *GetMatch();

	//HP 
	g_UI[HP_RED].w = HP_TEXTURE_WIDTH * (player.life / HUNDRED);
	g_UI[HP_RED].pos.x = HP_X - (HUNDRED - player.life)*DEVIATION_HP;

	//スタミナ
	g_UI[STAM_BAR].w = STAM_TEXTURE_WIDTH * (player.stamina / HUNDRED);
	g_UI[STAM_BAR].pos.x = STAM_X - (HUNDRED - player.stamina) * DEVIATION_STAM;
	g_UI[STAM_BAR].color = { 1.0f - player.stamina / HUNDRED , player.stamina / HUNDRED , 0.0f,1.0f };

	//ライター
	g_UI[OIL_RED].h = OIL_TEXTURE_HEIGHT * (lighter.oil / HUNDRED);				//オイルが少なくなって、ゲージも小さくなる
	g_UI[OIL_RED].pos.y = OIL_Y + (HUNDRED - lighter.oil)*DEVIATION_OIL;			//小さくすれば、下に移動する

	//Sanity
	g_UI[MIND].w = MIND_TEXTURE_WIDTH * (player.sanity / HUNDRED);
	g_UI[MIND].pos.x = MIND_X - (HUNDRED - player.sanity) * DEVIATION_MIND;

	//Sanity エフェクト
	SanityCheck(player.sanity);


	g_match = match.num;													//マッチの数字を貰う

	GaugeAnimation();
	HurtAnimation();

#ifdef _DEBUG	// デバッグ情報を表示する
	if (GetKeyboardTrigger(DIK_K))
	{
		for (int i = 0; i < ITEM_MAX; i++)
		{
			if (g_ItemBox[i].use == FALSE)
			{
				g_ItemBox[i].use = TRUE;
				break;
			}
		}
	}
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawInterface(void)
{
	for (int i = 0; i < TEXTURE_MAX - 1; i++)
	{
		if (i == MATCH_NUM)																//マッチの数字を別で印刷
			continue;
		if (g_UI[i].use == FALSE)
			continue;
		if (i == HURT || i == INSANE)
		{
			SetBlendState(BLEND_MODE_ADD);
		}
		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		// マトリクス設定
		SetWorldViewProjection2D();

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].TexNo]);

		// スコアの位置やテクスチャー座標を反映
		float px = g_UI[i].pos.x;	// 表示位置X
		float py = g_UI[i].pos.y;			// 表示位置Y
		float pw = g_UI[i].w;				// 表示幅
		float ph = g_UI[i].h;				// 表示高さ

		float tw = g_UI[i].tw;		// テクスチャの幅
		float th = g_UI[i].th;		// テクスチャの高さ
		float tx = g_UI[i].tx;		// テクスチャの左上X座標
		float ty = g_UI[i].ty;		// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_UI[i].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		if (i == HURT || i== INSANE)
		{
			SetBlendState(BLEND_MODE_NONE);
		}
	}

	//マッチの数字

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[MATCH_NUM].TexNo]);

	// 桁数分処理する
	int number = g_match;
	for (int i = 0; i < MATCH_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(number % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = g_UI[MATCH_NUM].pos.x - g_UI[MATCH_NUM].w * i;	// スコアの表示位置X
		float py = g_UI[MATCH_NUM].pos.y;			// スコアの表示位置Y
		float pw = g_UI[MATCH_NUM].w;				// スコアの表示幅
		float ph = g_UI[MATCH_NUM].h;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		number /= 10;
	}


	//アイテムボックス
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_ItemBox[i].use == FALSE)
			continue;
		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		// マトリクス設定
		SetWorldViewProjection2D();

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_ItemBox[i].TexNo]);

		// スコアの位置やテクスチャー座標を反映
		float px = g_ItemBox[i].pos.x;	// 表示位置X
		float py = g_ItemBox[i].pos.y;			// 表示位置Y
		float pw = g_ItemBox[i].w;				// 表示幅
		float ph = g_ItemBox[i].h;				// 表示高さ

		float tw = g_ItemBox[i].tw;		// テクスチャの幅
		float th = g_ItemBox[i].th;		// テクスチャの高さ
		float tx = g_ItemBox[i].tx;		// テクスチャの左上X座標
		float ty = g_ItemBox[i].ty;		// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_ItemBox[i].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

//=============================================================================
// ゲージ減らすアニメーション処理
//=============================================================================
void GaugeAnimation()
{
	if (g_UI[HP_RED].w == g_UI[HP_RED_BG].w)
		return;

	if (g_UI[HP_RED].w > g_UI[HP_RED_BG].w)
	{
		g_UI[HP_RED_BG].w = g_UI[HP_RED].w;
		g_UI[HP_RED_BG].pos.x = g_UI[HP_RED].pos.x;
		g_time = 0;
		return;
	}
	if (g_time > WAIT_TIME)
	{
		g_UI[HP_RED_BG].w -= 1.5f;
		g_UI[HP_RED_BG].pos.x -= 1.5f;
	}
	else
	{
		g_time++;
	}

	return;
}


//=============================================================================
// アイテムボックスを戻す
//=============================================================================
UI_ELEMENT *GetItemBox()
{
	return &g_ItemBox[0];
}
//=============================================================================
// 現時点でアイテムボックスの最大値
//=============================================================================
int GetItemMax()
{
	return g_ItemMax;
}
//=============================================================================
// アイテムボックスの最大値を＋１増加
//=============================================================================
void IncreaseItemmax()
{
	if (g_ItemMax >= ITEM_MAX)
		return;
	g_ItemMax++;
}

//=============================================================================
// ダメージを受けるとアニメーションをONにする
//=============================================================================
void HurtAnimationOn()
{
	g_UI[HURT].use = TRUE;
}

//=============================================================================
// ダメージを受けるとのアニメーション
//=============================================================================
void HurtAnimation()
{
	if (g_UI[HURT].use == FALSE)
		return;
	if (g_hurt_time >= PLAYER_HURT_FRAME)
	{
		g_hurt_time = 0;
		g_UI[HURT].use = FALSE;
		g_UI[HURT].color.w = 0.0f;
		return;
	}
	g_hurt_time++;
	if (g_UI[HURT].color.w < 1.0f)
	{
		g_UI[HURT].color.w += 0.08f;
	}
	else
	{
		g_UI[HURT].color.w -= 0.08f;
	}
	return;
}

//Sanity エフェクト
void SanityCheck(int sanity)
{
	if (sanity > 75) { g_UI[INSANE].color.w = 0.25f; };
	if (sanity > 50 && sanity < 75) { g_UI[INSANE].color.w = 0.5f; };
	if (sanity > 25 && sanity < 50) { g_UI[INSANE].color.w = 0.75f; };
	if (sanity < 25) { g_UI[INSANE].color.w = 0.89f; };
	return;
}
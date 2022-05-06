//=============================================================================
//
// ライター処理 [lighter.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "interface.h"
#include "lighter.h"
#include "sprite.h"
#include "sound.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(160)	// テクスチャサイズ
#define TEXTURE_HEIGHT				(200)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数

#define SCREEN_WIDTH	(960)			// ウインドウの幅
#define SCREEN_HEIGHT	(540)			// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define LIGHTER_POS_X	(SCREEN_CENTER_X + 250)
#define LIGHTER_POS_Y	(SCREEN_CENTER_Y + 100)



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/lighter.png",
};

static int						g_TexNo;					// テクスチャ番号
static LIGHTER					g_Lighter;					

static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLighter(void)
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


	// 初期化
	g_Lighter.w     = TEXTURE_WIDTH;
	g_Lighter.h     = TEXTURE_HEIGHT;
	g_Lighter.pos   = { LIGHTER_POS_X, LIGHTER_POS_Y, 0.0f };
	g_TexNo = 0;

	g_Lighter.use = TRUE;
	g_Lighter.oil = LIGHTER_OIL_MAX;
	g_Lighter.out = FALSE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLighter(void)
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
void UpdateLighter(void)
{
#ifdef _DEBUG	// デバッグ情報を表示する

	if (GetKeyboardTrigger(DIK_L))
	{
		SetLighterOn(TRUE);
	}
	if (GetKeyboardTrigger(DIK_J))
	{
		SetLighterOff();
	}
	if (GetKeyboardTrigger(DIK_I))
	{
		AddOil(10.0f);
	}
#endif

	CheckOil();
	if (!g_Lighter.use || !g_Lighter.out)
		return;
	ReduceOil();

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	PrintDebugProc("残るオイル量：%f / 100", g_Lighter.oil);
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLighter(void)
{
	if (!g_Lighter.out)
		return;

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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// スコアの位置やテクスチャー座標を反映
	float px = g_Lighter.pos.x;	// スコアの表示位置X
	float py = g_Lighter.pos.y;			// スコアの表示位置Y
	float pw = g_Lighter.w;				// スコアの表示幅
	float ph = g_Lighter.h;				// スコアの表示高さ

	float tw = 1.0f;		// テクスチャの幅
	float th = 1.0f;		// テクスチャの高さ
	float tx = 0.0f;			// テクスチャの左上X座標
	float ty = 0.0f;			// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// ライターON
//=============================================================================

void SetLighterOn(BOOL flag)
{
	//CheckOil();
	if (!g_Lighter.use)		return;
	PlaySound(SOUND_LABEL_SE_LighterON);
	g_Lighter.out = flag;
}

//=============================================================================
// ライターOFF
//=============================================================================
void SetLighterOff()
{
	if (!g_Lighter.use)	return;
	g_Lighter.out = FALSE;
}

//=============================================================================
// オイル量を追加
//=============================================================================
void AddOil(float n)
{
	if (g_Lighter.oil + n > LIGHTER_OIL_MAX)
	{
		g_Lighter.oil = LIGHTER_OIL_MAX;
	}
	else
	{
		g_Lighter.oil += n;

	}
	if (g_Lighter.use == FALSE)
	{
		g_Lighter.use = TRUE;
	}
}

//=============================================================================
// オイル量チェック
//=============================================================================
void CheckOil()
{
	if (g_Lighter.oil >= LIGHTER_OIL_MIN || g_Lighter.use == FALSE)
		return;
	SetLighterOff();
	g_Lighter.use = FALSE;
}

//=============================================================================
// オイル量減らす
//=============================================================================
void ReduceOil()
{
	g_Lighter.oil -= LIGHTER_OIL_REDUCE;
}

LIGHTER *GetLighter()
{
	return &g_Lighter;
}
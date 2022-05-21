//=============================================================================
//
// ゲームオーバー画面処理 [game_over.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "game_over.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 
#define	ENEMY_TEXMAG	(0.05f)							// 元画像に対する倍率
#define	ENEMY_WIDTH		(650.0f * ENEMY_TEXMAG)			// 頂点サイズ
#define	ENEMY_HEIGHT	(812.0f * ENEMY_TEXMAG)			// 頂点サイズ
#define ENEMY_SPOWN	(330)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/gameover.png",
	"data/TEXTURE/enemy.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static int						g_Event;
static float					g_w, g_h;					// 幅と高さ
static float					g_Ew, g_Eh;					// 幅と高さ
static float					g_Tw, g_Th;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static XMFLOAT3					g_EPos;						// エネミーポリゴンの座標
static int						g_Count;
static int						g_TexNo;					// テクスチャ番号
static XMFLOAT4					g_Color[TEXTURE_MAX];
static BOOL						g_Load = FALSE;
static float					alpha = 0.0f;
static float					Ealpha = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame_over(void)
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


	// 変数の初期化
	g_Use = FALSE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_Ew = ENEMY_WIDTH;
	g_Eh = ENEMY_HEIGHT;
	g_Tw = 947.0f * 0.5f;
	g_Th = 400.0f * 0.5f;
	g_EPos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	// BGM再生
	//PlaySound(SOUND_LABEL_BGM_sample000);
	g_Event = 0;
	g_Count = 0;
	alpha = 0.0f;
	Ealpha = 0.0f;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame_over(void)
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
void UpdateGame_over(void)
{
	if (!g_Use)return;

	switch (g_Event)
	{
	case Start_BGM:	//BGMをまず鳴らす
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GameOver);
		g_Event++;
		break;
	case Enemy_Dash:	//適切なタイミングでエネミーが近づいてくる
		g_Count++;
		if (g_Count >= ENEMY_SPOWN)
		{
			g_Ew *= 1.1f;
			g_Eh *= 1.1f;
			Ealpha = 1.0f;
		}

		if (g_Count >= ENEMY_SPOWN + 30)
		{
			g_Event++;
			g_Count = 0;
		}
		break;
	case Dead:	//エネミーを消し、文字を浮かび上がらせる。
		if (g_Count == 0)
		{
			Ealpha = 0.0f;
		}
		else if (g_Count >= 60 && alpha < 1.0f)
		{
			alpha += 0.02f;
		}
		else if (g_Count == 220)
		{
			SetFade(FADE_OUT, MODE_GAME);	//文字が出たらモードリセット
		}
		g_Count++;
		break;
	}



#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame_over(void)
{
	if (!g_Use)return;

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		switch (i)	//ここでテクスチャ毎に座標と大きさを指定
		{
		case GAME_OVER_BG:
			g_w = TEXTURE_WIDTH;
			g_h = TEXTURE_HEIGHT;
			g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
			g_Color[i] = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case GAME_OVER_TEXT:
			g_w = g_Tw;
			g_h = g_Th;
			g_Pos = XMFLOAT3(TEXTURE_WIDTH / 2, TEXTURE_HEIGHT * 0.5f, 0.0f);
			g_Color[i] = XMFLOAT4{ 1.0f, 1.0f, 1.0f, alpha };
			break;
		case GAME_OVER_ENEMY:
			g_w = g_Ew;
			g_h = g_Eh;
			g_Pos = XMFLOAT3(TEXTURE_WIDTH / 2, TEXTURE_HEIGHT * 0.5f, 0.0f);
			g_Color[i] = XMFLOAT4{ 1.0f, 1.0f, 1.0f,Ealpha};
			break;
		}

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f,
			g_Color[i]);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}

void SetGameOver(void)
{
	g_Use = TRUE;
}
//=============================================================================
//
// 文字テクスチャ等の表示処理 [text_texture.cpp]
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
#include "text_texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(12)				// テクスチャの数
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
};
static BOOL						g_Load = FALSE;
static TEXT_TEXTURE				g_Texttex[TEXTURE_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTexttex(void)
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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texttex[i].use = FALSE;
		g_Texttex[i].TexNo = i;						//テクスチャ番号
		g_Texttex[i].w = 0;
		g_Texttex[i].h = 0;
		g_Texttex[i].tw = 1.0f;
		g_Texttex[i].th = 1.0f;
		g_Texttex[i].tx = 0.0f;
		g_Texttex[i].ty = 0.0f;		// 幅と高さ、テクスチャ座標
		g_Texttex[i].pos = {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f};						// ポリゴンの位置
		g_Texttex[i].color = { 1.0f, 1.0f, 1.0f, 0.0f };						//色
		g_Texttex[i].time = 0;					
		g_Texttex[i].timeMax = 180;				
	}

	// 初期化
	g_Load = TRUE;
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTexttex(void)
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
void UpdateTexttex(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texttex[i].use != TRUE)continue;

		const float fade = 0.04f;
		if (g_Texttex[i].color.w < 1.0f && g_Texttex[i].time < g_Texttex[i].timeMax)
		{//表示開始
			g_Texttex[i].color.w += fade;
		}
		else if (g_Texttex[i].color.w >= 1.0f && g_Texttex[i].time < g_Texttex[i].timeMax)
		{//表示しきったのでそれを既定時間まで維持
			g_Texttex[i].time++;
		}
		else if (g_Texttex[i].color.w > 0.0f && g_Texttex[i].time >= g_Texttex[i].timeMax)
		{//消去開始
			g_Texttex[i].color.w -= fade;
		}
		else if (g_Texttex[i].color.w <= 0.0f && g_Texttex[i].time >= g_Texttex[i].timeMax)
		{//消去したため、全て初期化
			g_Texttex[i].use = FALSE;
			g_Texttex[i].color.w = 0.0f;
			g_Texttex[i].time = 0;
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawTexttex(void)
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

	float px = 0.0f;	// 表示位置X
	float py = 0.0f;			// 表示位置Y
	float pw = 0.0f;				// 表示幅
	float ph = 0.0f;				// 表示高さ

	float tw = 0.0f;		// テクスチャの幅
	float th = 0.0f;		// テクスチャの高さ
	float tx = 0.0f;		// テクスチャの左上X座標
	float ty = 0.0f;		// テクスチャの左上Y座標

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texttex[i].use != TRUE)continue;

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Texttex[i].TexNo]);

		// スコアの位置やテクスチャー座標を反映
		px = g_Texttex[i].pos.x;			// 表示位置X
		py = g_Texttex[i].pos.y;			// 表示位置Y
		pw = g_Texttex[i].w;				// 表示幅
		ph = g_Texttex[i].h;				// 表示高さ

		tw = g_Texttex[i].tw;		// テクスチャの幅
		th = g_Texttex[i].th;		// テクスチャの高さ
		tx = g_Texttex[i].tx;		// テクスチャの左上X座標
		ty = g_Texttex[i].ty;		// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_Texttex[i].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

void SetTexture(int i)
{
	if (g_Texttex[i].use != TRUE)
		g_Texttex[i].use = TRUE;
}
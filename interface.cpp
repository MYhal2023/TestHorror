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
#define HP_TEXTURE_WIDTH				(800/5)	// テクスチャサイズ
#define HP_TEXTURE_HEIGHT				(200/5)	// 
#define TEXTURE_MAX					(6)				// テクスチャの数
#define	HP_X						(80)			// テクスチャ座標
#define HP_Y						(200)
#define OIL_X						(SCREEN_WIDTH-100)	// テクスチャ座標
#define OIL_Y						(SCREEN_HEIGHT-150)					// 
#define MATCH_X						(50)
#define MATCH_Y						(SCREEN_HEIGHT-80)
#define MATCH_TEXTURE_WIDTH			(40*2)
#define MATCH_TEXTURE_HEIGHT		(20*2)
#define MATCH_NUM_TEXTURE_WIDTH		(25)	// テクスチャサイズ
#define MATCH_NUM_TEXTURE_HEIGHT	(50)	// 

#define MATCH_DIGIT			(2)			// 桁数

#define HUNDRED						(100.0f)				//フロート型100
#define	DEVIATION					(0.8f)					//ライターのゲージの移動を訂正するため


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
	"data/TEXTURE/hpbar.png",
	"data/TEXTURE/oilbarred.png",
	"data/TEXTURE/oilbar.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/match_ui.png",
};


UI_ELEMENT	g_UI[TEXTURE_MAX];

static BOOL						g_Load = FALSE;
static int						g_match = 0;


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

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].TexNo = i;
		g_UI[i].tw = 1.0f;		// テクスチャの幅
		g_UI[i].th = 1.0f;		// テクスチャの高さ
		g_UI[i].tx = 0.0f;			// テクスチャの左上X座標
		g_UI[i].ty = 0.0f;			// テクスチャの左上Y座標
	}
	g_UI[HP_BAR].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_BAR].w = HP_TEXTURE_WIDTH;
	g_UI[HP_BAR].h = HP_TEXTURE_HEIGHT;

	g_UI[HP_RED].pos = { HP_X , HP_Y , 0.0f };
	g_UI[HP_RED].w = HP_TEXTURE_WIDTH;
	g_UI[HP_RED].h = HP_TEXTURE_HEIGHT;

	g_UI[OIL_BAR].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_BAR].w = HP_TEXTURE_HEIGHT;
	g_UI[OIL_BAR].h = HP_TEXTURE_WIDTH;

	g_UI[OIL_RED].pos = { OIL_X , OIL_Y , 0.0f };
	g_UI[OIL_RED].w = HP_TEXTURE_HEIGHT;
	g_UI[OIL_RED].h = HP_TEXTURE_WIDTH;

	g_UI[MATCH_NUM].pos = { MATCH_X+ MATCH_TEXTURE_WIDTH, MATCH_Y, 0.0f };
	g_UI[MATCH_NUM].w = MATCH_NUM_TEXTURE_WIDTH;
	g_UI[MATCH_NUM].h = MATCH_NUM_TEXTURE_HEIGHT;

	g_UI[MATCH_PIC].pos = { MATCH_X, MATCH_Y, 0.0f };
	g_UI[MATCH_PIC].w = MATCH_TEXTURE_WIDTH;
	g_UI[MATCH_PIC].h = MATCH_TEXTURE_HEIGHT;
	// プレイヤーの初期化



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

	g_UI[OIL_RED].h = HP_TEXTURE_WIDTH * (lighter.oil/HUNDRED);				//オイルが少なくなって、ゲージも小さくなる
	g_UI[OIL_RED].pos.y = OIL_Y + (HUNDRED-lighter.oil)*DEVIATION;			//小さくすれば、下に移動する
	g_match = match.num;													//マッチの数字を貰う


#ifdef _DEBUG	// デバッグ情報を表示する

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawInterface(void)
{
	for(int i=0;i<TEXTURE_MAX;i++)
	{
		if (i == MATCH_NUM)																//マッチの数字を別で印刷
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
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
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
}

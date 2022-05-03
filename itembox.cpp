//=============================================================================
//
// アイテムボックス処理 [itembox.cpp]
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
#define TEXTURE_MAX					(6)				// テクスチャの数

#define ITEM_TEXTURE_WIDTH		(40)	// テクスチャサイズ
#define ITEM_TEXTURE_HEIGHT		(35)	// 

#define HUNDRED						(100.0f)				//フロート型100

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
	"data/TEXTURE/match.png",
	"data/TEXTURE/lighter.png",
	"data/TEXTURE/enemy_debug.png",
	"data/TEXTURE/enemy.png",
	"data/TEXTURE/itembox.png",
};


static ITEM			g_Item[TEXTURE_MAX];							//アイテム表示のデータ
static BOX			g_Box[ITEM_MAX];								//アイテムボックス表示
static int			g_items = ITEM_MIN;								//アイテムの数字
static int			g_Select;										//今どこに選択している
static UI_ELEMENT	g_Selectbox;									//選択しているための表示

static BOOL			g_Load = FALSE;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItembox(void)
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
		g_Item[i].use = TRUE;
		g_Item[i].TexNo = i;
		g_Item[i].id = i;
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (i < ITEM_MIN)
		{
			g_Box[i].use = TRUE;
		}
		else
		{
			g_Box[i].use = FALSE;
		}

		g_Box[i].open = TRUE;
		g_Box[i].TexNo = 3;
		g_Box[i].slot = i;
		UI_ELEMENT	*itembox = GetItemBox();
		g_Box[i].pos.x = itembox[i].pos.x;
		g_Box[i].pos.y = itembox[i].pos.y;
		g_Box[i].pos.z = 0.0f;
		g_Box[i].w = ITEM_TEXTURE_WIDTH;
		g_Box[i].h = ITEM_TEXTURE_HEIGHT;
		g_Box[i].tw = 1.0f;		// テクスチャの幅
		g_Box[i].th = 1.0f;		// テクスチャの高さ
		g_Box[i].tx = 0.0f;			// テクスチャの左上X座標
		g_Box[i].ty = 0.0f;			// テクスチャの左上Y座標
		g_Box[i].color = { 1.0f,1.0f,1.0f,0.0f };
	}


	g_Selectbox.use = TRUE;
	g_Selectbox.TexNo = EMPTY;
	g_Selectbox.w = ITEM_TEXTURE_WIDTH;
	g_Selectbox.h = ITEM_TEXTURE_HEIGHT;
	g_Selectbox.tw = 1.0f;		// テクスチャの幅
	g_Selectbox.th = 1.0f;		// テクスチャの高さ
	g_Selectbox.tx = 0.0f;			// テクスチャの左上X座標
	g_Selectbox.ty = 0.0f;			// テクスチャの左上Y座標
	UI_ELEMENT	*itembox = GetItemBox();
	g_Selectbox.pos.x = itembox[0].pos.x;
	g_Selectbox.pos.y = itembox[0].pos.y;
	g_Selectbox.pos.z = 0.0f;
	g_Selectbox.color = { 1.0f,0.0f,0.0f,1.0f };


	g_Select = 0;
	// 初期化



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItembox(void)
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
void UpdateItembox(void)
{
	if (GetItemMax() != g_items)
	{
		g_items = GetItemMax();
	}

	MoveSelect();

#ifdef _DEBUG	// デバッグ情報を表示する
	if (GetKeyboardTrigger(DIK_9))
	{
		AddItembox(g_Item[MATCH_ITEM].TexNo);
	}
	if (GetKeyboardTrigger(DIK_0))
	{
		AddItembox(g_Item[LIGHTER_ITEM].TexNo);
	}
	if (GetKeyboardTrigger(DIK_8))
	{
		AddItembox(g_Item[RANDOM_ITEM_1].TexNo);
	}
	if (GetKeyboardTrigger(DIK_7))
	{
		AddItembox(g_Item[RANDOM_ITEM_2].TexNo);
	}
	if (GetKeyboardTrigger(DIK_N))
	{
		IncreaseItembox(1);
	}

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItembox(void)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_Box[i].use == FALSE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Box[i].TexNo]);

		// スコアの位置やテクスチャー座標を反映
		float px = g_Box[i].pos.x;	// 表示位置X
		float py = g_Box[i].pos.y;			// 表示位置Y
		float pw = g_Box[i].w;				// 表示幅
		float ph = g_Box[i].h;				// 表示高さ

		float tw = g_Box[i].tw;		// テクスチャの幅
		float th = g_Box[i].th;		// テクスチャの高さ
		float tx = g_Box[i].tx;		// テクスチャの左上X座標
		float ty = g_Box[i].ty;		// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_Box[i].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	//選択ボックス

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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Selectbox.TexNo]);

	// スコアの位置やテクスチャー座標を反映
	float px = g_Selectbox.pos.x;	// 表示位置X
	float py = g_Selectbox.pos.y;			// 表示位置Y
	float pw = g_Selectbox.w;				// 表示幅
	float ph = g_Selectbox.h;				// 表示高さ

	float tw = g_Selectbox.tw;		// テクスチャの幅
	float th = g_Selectbox.th;		// テクスチャの高さ
	float tx = g_Selectbox.tx;		// テクスチャの左上X座標
	float ty = g_Selectbox.ty;		// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		g_Selectbox.color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// アイテムボックスにアイテムを追加
//=============================================================================
void AddItembox(int TNum)
{
	for (int i = 0; i < g_items; i++)
	{
		if (g_Box[i].use == FALSE || g_Box[i].open == FALSE)
			continue;
		g_Box[i].TexNo = TNum;
		g_Box[i].open = FALSE;
		g_Box[i].color = { 1.0f,1.0f,1.0f,1.0f };
		break;
	}
	return;
}
//=============================================================================
// アイテムボックスに空いているところを追加
//=============================================================================
void IncreaseItembox(int n)
{
	int m = 0;
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (g_Box[i].use == TRUE)
			continue;
		if (m < n)
		{
			UI_ELEMENT	*itembox = GetItemBox();
			itembox[i].use = TRUE;
			g_Box[i].use = TRUE;
			IncreaseItemmax();
			m++;
		}
	}
	return;
}


void MoveSelect()		//C→		A←
{
	if (IsButtonTriggered(0, BUTTON_C) && g_Select < g_items - 1)
	{
		g_Select++;
		g_Selectbox.pos.x = g_Box[g_Select].pos.x;
	}
	if (IsButtonTriggered(0, BUTTON_A) && g_Select > 0)
	{
		g_Select--;
		g_Selectbox.pos.x = g_Box[g_Select].pos.x;
	}
}

int	ReturnSelect()		//今選択しているアイテムのIDを戻す
{
	return g_Box[g_Select].TexNo;
}
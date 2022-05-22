//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 
#define TEXTURE_HEIGHT_PER			(0.15f)			// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TITLE_TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TITLE_TEXTURE_MAX] = {
	"data/TEXTURE/result001.jpg",
	"data/TEXTURE/cursol.png",
	"data/TEXTURE/endgame.png",
	"data/TEXTURE/newgame.png",
	"data/TEXTURE/loadgame.png",
};

static TITLE					g_Title[TITLE_TEXTURE_MAX];
static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
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
	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
	{
		g_Title[i].use = TRUE;
		//g_Title[i].w = TEXTURE_WIDTH;
		//g_Title[i].h = TEXTURE_HEIGHT;
		//g_Title[i].pos = XMFLOAT3(g_Title[i].w / 2, g_Title[i].h / 2, 0.0f);
		g_Title[i].texNo = i;
		const float mag = 0.1f;
		const float Cmag = 0.04f;
		switch (i)	//ここでテクスチャ毎に座標と大きさを指定
		{
		case TITLE_BG:
			g_Title[i].w = TEXTURE_WIDTH;
			g_Title[i].h = TEXTURE_HEIGHT;
			g_Title[i].pos.x = g_Title[i].w / 2;
			g_Title[i].pos.y = g_Title[i].h / 2;
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_CURSOL:
			g_Title[i].w = 1600.0f * Cmag;
			g_Title[i].h = 1200.0f * Cmag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.35f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * 0.6f + TEXTURE_HEIGHT_PER;
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_NEW:
			g_Title[i].w = 1936.0f * mag;
			g_Title[i].h = 412.0f * mag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.5f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * 0.6f;
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_LOAD:
			g_Title[i].w = 1996.0f * mag;
			g_Title[i].h = 412.0f * mag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.5f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * (0.6f+ TEXTURE_HEIGHT_PER);
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case TITLE_END:
			g_Title[i].w = 1789.0f * mag;
			g_Title[i].h = 412.0f * mag;
			g_Title[i].pos.x = TEXTURE_WIDTH * 0.5f;
			g_Title[i].pos.y = TEXTURE_HEIGHT * (0.6f + TEXTURE_HEIGHT_PER * 2.0f);
			g_Title[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		}

	}

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_Normal);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
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
void UpdateTitle(void)
{
	if ((GetKeyboardTrigger(DIK_RETURN) ||
		IsButtonTriggered(0, BUTTON_START) ||
		IsButtonTriggered(0, BUTTON_B))
		&& g_Title[TITLE_CURSOL].pos.y - 1.0f <= g_Title[TITLE_NEW].pos.y)
	{// Enter押したら、ステージを切り替える
		PlaySound(SOUND_LABEL_SE_Cursol_On);
		SetFade(FADE_OUT, MODE_GAME);
	}
	else if((GetKeyboardTrigger(DIK_RETURN) ||
		IsButtonTriggered(0, BUTTON_START) ||
		IsButtonTriggered(0, BUTTON_B))
		&& g_Title[TITLE_CURSOL].pos.y >= g_Title[TITLE_END].pos.y)
	{
		PlaySound(SOUND_LABEL_SE_Cursol_On);
		SetFade(FADE_OUT, MODE_MAX);
	}
	else if((GetKeyboardTrigger(DIK_RETURN) ||
		IsButtonTriggered(0, BUTTON_START) ||
		IsButtonTriggered(0, BUTTON_B)))
	{
		PlaySound(SOUND_LABEL_SE_Door_Close);
	}

	if ((GetKeyboardTrigger(DIK_UP) ||
		IsButtonTriggered(0, BUTTON_UP))
		&& g_Title[TITLE_CURSOL].pos.y > g_Title[TITLE_NEW].pos.y + 1.0f)//float値操作により誤差が生じるため、余剰を設ける
	{
		PlaySound(SOUND_LABEL_SE_Cursol_Move);
		g_Title[TITLE_CURSOL].pos.y -= 540.0f * TEXTURE_HEIGHT_PER;
	}
	else if ((GetKeyboardTrigger(DIK_DOWN) ||
		IsButtonTriggered(0, BUTTON_DOWN))
		&& g_Title[TITLE_CURSOL].pos.y < g_Title[TITLE_END].pos.y)
	{
		PlaySound(SOUND_LABEL_SE_Cursol_Move);
		g_Title[TITLE_CURSOL].pos.y += 540.0f * TEXTURE_HEIGHT_PER;
	}



#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
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

	for (int i = 0; i < TITLE_TEXTURE_MAX; i++)
	{
		if (!g_Title[i].use)continue;
		float px = 0.0f;
		float py = 0.0f;
		float tw = 0.0f;
		float th = 0.0f;
		tw = g_Title[i].w;
		th = g_Title[i].h;
		px = g_Title[i].pos.x;
		py = g_Title[i].pos.y;

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[i].texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, tw, th, 0.0f, 0.0f, 1.0f, 1.0f, g_Title[i].color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

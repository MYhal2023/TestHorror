//=============================================================================
//
// マッチ処理 [match.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "player.h"
#include "time.h"
#include "game.h"
#include "input.h"
#include "match.h"
#include "debugproc.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// テクスチャサイズ
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define MATCH_POS_X					(600)		// マッチを表示する場所(X軸)
#define DEFAULTMATCH_POS_Y			(600.0f)	// 通常時のマッチの場所(Y軸)
#define STANDBYMATCH_POS_Y			(500.0f)	// スタンバイ時のマッチの場所(Y軸)
#define STANDBYTIME					(10)		// マッチを構えるまでの時間
#define STANDBY_MOVE_FRAME			((STANDBYMATCH_POS_Y-DEFAULTMATCH_POS_Y)/STANDBYTIME)//構える時の1フレームあたりの移動量
#define MATCH_NUM					(10)			// マッチの本数(初期)
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
	"data/TEXTURE/fire.png",

};




static MATCH						g_Match;

static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMatch(void)
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


	// マッチの初期化
	g_Match.Use   = TRUE;
	g_Match.w     = TEXTURE_WIDTH;
	g_Match.h     = TEXTURE_HEIGHT;
	g_Match.Pos   = { MATCH_POS_X, DEFAULTMATCH_POS_Y, 0.0f };
	g_Match.TexNo = 0;
	g_Match.StandbyTime = 0;
	g_Match.num = MATCH_NUM;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMatch(void)
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
void UpdateMatch(void)
{

	StandbyMatch();


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	PrintDebugProc("Match.num %d\n", g_Match.num);
	PrintDebugProc("Match.AblazeTime %d\n", g_Match.AblazeTime);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMatch(void)
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

	// マッチを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, g_Match.Pos.x, g_Match.Pos.y, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// 炎を描画
	if(g_Match.AblazeTime > 0)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, g_Match.Pos.x, g_Match.Pos.y-50, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}

//
void StandbyMatch(void)
{
	if(g_Match.AblazeTime>0)g_Match.AblazeTime--;
	if (g_Match.AblazeTime != 0)return;
	
	//マッチを構える条件
	if (IsButtonPressed(0, BUTTON_L)&&(0 < g_Match.num))
	{

		//構えきるまでの時間
		if (g_Match.StandbyTime < STANDBYTIME)
		{
			g_Match.StandbyTime++;
			g_Match.Pos.y = STANDBY_MOVE_FRAME* g_Match.StandbyTime + DEFAULTMATCH_POS_Y;
			return;
		}

		//switch (g_Match.force)
		switch (IsButtonForce(0))
		{
		case FORCE_NON:
			break;
		case FORCE_SLOW:
			g_Match.num--;
			g_Match.AblazeTime = 300;
			break;
		case FORCE_MIDDLE:
			g_Match.num--;
			g_Match.AblazeTime = 600;
			break;
		case FORCE_FAST:
			g_Match.num--;
			g_Match.AblazeTime = 100;
			break;
		}

	}
	else
	{
		//おろし終わるまでの時間
		if (0 < g_Match.StandbyTime)
		{
			g_Match.StandbyTime--;
			g_Match.Pos.y = STANDBY_MOVE_FRAME * g_Match.StandbyTime + DEFAULTMATCH_POS_Y;
			return;
		}
	}

}


void SetMatchForce(int force)
{
	g_Match.force = force;
}
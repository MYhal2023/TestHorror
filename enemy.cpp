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


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(2)			// テクスチャの数

#define	ENEMY_TEXMAG	(0.04f)							// 元画像に対する倍率
#define	ENEMY_WIDTH		(650.0f * ENEMY_TEXMAG)			// 頂点サイズ
#define	ENEMY_HEIGHT	(812.0f * ENEMY_TEXMAG)			// 頂点サイズ

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
};

static BOOL							g_Load = FALSE;


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
	}

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

		//デバッグ中なら画像の入れ替えが可能(怖いやつと怖くないやつに入れ替え可能！)
#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F))
			g_Enemy[i].texNo = (g_Enemy[i].texNo == ENEMY_WOMAN) ? ENEMY_DEBUG : ENEMY_WOMAN;
#endif
	}
	//エネミーのセットを行える
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_G))
		SetEnemy(XMFLOAT3(0.0f, 0.0f, -250.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

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

	XMMATRIX mtxScl, mtxTranslate, mtxWorld;
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
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

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
		//デバッグ中なら最初は怖くないほうに再セット
#ifdef _DEBUG
		g_Enemy[i].texNo = ENEMY_DEBUG;

#endif
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


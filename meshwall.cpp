//=============================================================================
//
// メッシュ壁の処理 [meshwall.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "meshwall.h"
#include "renderer.h"
#include "player.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(3)						// テクスチャの数

#define	VALUE_MOVE_WALL		(5.0f)					// 移動速度
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.001f)		// 回転速度

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4		mtxWorld;
	XMFLOAT3		pos;						// ポリゴン表示位置の中心座標
	XMFLOAT3		rot;						// ポリゴンの回転角
	int				nNumBlockX, nNumBlockY;		// ブロック数
	int				nNumVertex;					// 総頂点数	
	int				nNumVertexIndex;			// 総インデックス数
	int				nNumPolygon;				// 総ポリゴン数
	float			fBlockSizeX, fBlockSizeY;	// ブロックサイズ
	int				texNo;						//使用するテクスチャの指定
	BOOL			use;	
	MATERIAL		material;					// マテリアル
	ID3D11Buffer	*vertexBuffer;				// 頂点バッファ
	ID3D11Buffer	*indexBuffer;				// インデックスバッファ

} MESH_WALL;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;		// テクスチャ番号

static MESH_WALL g_aMeshWall[MAX_MESH_WALL];		// メッシュ壁ワーク
static int g_nNumMeshWall = 0;						// メッシュ壁の数
static int g_CeilingWall = 0;						// 天井に使用したメッシュ壁

static char* g_TextureName[TEXTURE_MAX] = {
	"data/TEXTURE/wall_001.png",
	"data/TEXTURE/wall_002.png",
	"data/TEXTURE/wall101.jpg",
};

static BOOL							g_Load = FALSE;


//======================================================Po=======================
// 初期化処理
//=============================================================================
HRESULT InitMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
						int nNumBlockX, int nNumBlockY, float fBlockSizeX, float fBlockSizeZ, int texNo)
{
	MESH_WALL *pMesh;

	if(g_nNumMeshWall >= MAX_MESH_WALL)
	{
		return E_FAIL;
	}



	pMesh = &g_aMeshWall[g_nNumMeshWall];

	// マテリアル情報の初期化
	ZeroMemory(&pMesh->material, sizeof(pMesh->material));
	pMesh->material.Diffuse = col;

	pMesh->use = TRUE;
	pMesh->texNo = texNo;
	// ポリゴン表示位置の中心座標を設定
	pMesh->pos = pos;

	pMesh->rot = rot;

	// ブロック数の設定
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;

	// 頂点数の設定
	pMesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// インデックス数の設定
	pMesh->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// ポリゴン数の設定
	pMesh->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// ブロックサイズの設定
	pMesh->fBlockSizeX = fBlockSizeX;
	pMesh->fBlockSizeY = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh->vertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(pMesh->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh->indexBuffer);

	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for(int nCntVtxY = 0; nCntVtxY < (pMesh->nNumBlockY + 1); nCntVtxY++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Position.x = -(pMesh->nNumBlockX / 2.0f) * pMesh->fBlockSizeX + nCntVtxX * pMesh->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Position.y = pMesh->nNumBlockY * pMesh->fBlockSizeY - nCntVtxY * pMesh->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Position.z = 0.0f;

				// 法線の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxY;
			}
		}

		GetDeviceContext()->Unmap(pMesh->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for(int nCntVtxY = 0; nCntVtxY < pMesh->nNumBlockY; nCntVtxY++)
		{
			if(nCntVtxY > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxY < (pMesh->nNumBlockY - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + pMesh->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh->indexBuffer, 0);
	}

	if (pMesh->rot.x == XM_PI * 0.5f && pMesh->rot.z == XM_PI * 0.5f)
		g_CeilingWall = g_nNumMeshWall;

	g_nNumMeshWall++;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	MESH_WALL *pMesh;
	int nCntMeshField;

	if (g_Load == FALSE) return;

	for(nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];

		if(pMesh->vertexBuffer)
		{// 頂点バッファの解放
			pMesh->vertexBuffer->Release();
			pMesh->vertexBuffer = NULL;
		}

		if(pMesh->indexBuffer)
		{// インデックスバッファの解放
			pMesh->indexBuffer->Release();
			pMesh->indexBuffer = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	//構造体のメンバ変数を全てリセット
	ResetMeshWall();
	//読み込み数をリセットする
	g_nNumMeshWall = 0;
	g_CeilingWall = 0;
	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	MESH_WALL *pMesh;
	int nCntMeshField;
	
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);


	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	for(nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];
		
		if (pMesh->use != TRUE)continue;

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh->vertexBuffer, &stride, &offset);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(pMesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0);


		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(pMesh->rot.x, pMesh->rot.y, pMesh->rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&pMesh->mtxWorld, mtxWorld);

		// マテリアル設定
		SetMaterial(pMesh->material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[pMesh->texNo]);

		// ポリゴンの描画
		GetDeviceContext()->DrawIndexed(pMesh->nNumVertexIndex, 0, 0);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

void MeshWallHit(XMFLOAT3 pos, float size, float old_x, float old_z)
{
	MESH_WALL *pMesh;
	for (int i = 0; i < g_nNumMeshWall; i++)
	{
		pMesh = &g_aMeshWall[i];
		if (pMesh->use != TRUE)continue;

		//回転量を元にx軸とz軸方向の幅を計算
		float rotatew = cosf(pMesh->rot.y);
		float rotatez = sinf(pMesh->rot.y);
		float width = pMesh->fBlockSizeX * fabsf(rotatew);
		float thickness = pMesh->fBlockSizeX * fabsf(rotatez);

		//壁とプレイヤーの当たり判定。BBで行うため、y座標は現状考慮していない。
		if (CollisionBB(pos, size, size, pMesh->pos, width, thickness) == TRUE
			&& pMesh->rot.x != XM_PI * 0.5f && pMesh->rot.z != XM_PI * 0.5f)
		{
			PLAYER *player = GetPlayer();
			//操作感を上げるために、片方の座標のみを元に戻す
			if (width < pMesh->fBlockSizeX)
				player->pos.x = old_x;
			else if (thickness < pMesh->fBlockSizeX)
				player->pos.z = old_z;

			//両方の座標が戻っているならこれ以上の走査は必要ない
			if (player->pos.x == old_x && player->pos.z == old_z)
				return;
		}
	}

}

void InitFieldMeshWall(void)
{
	//マップ制限の壁
	InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, WALL_A_POS_XZ), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_A_POS_XZ, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_A_POS_XZ, WAVE_POS_Y, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(0.0f, WAVE_POS_Y, -WALL_A_POS_XZ), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_A_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X軸(+),Z軸(+)
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X軸(-),Z軸(-)
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ - WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ - WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X軸(+),Z軸(-)
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ - WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ + WALL_SHIFT, WAVE_POS_Y, -WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ + -WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

	//X軸(-),Z軸(+)
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(WALL_B_POS_XZ - WALL_SHIFT, WAVE_POS_Y, WALL_SHIFT), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);
	InitMeshWall(XMFLOAT3(-WALL_SHIFT, WAVE_POS_Y, -WALL_B_POS_XZ + WALL_SHIFT), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WALL_XZ, WALL_Y, WALL_B_SIZE_XZ, WALL_BLOCK_SIZE_Y, WALL_BLACK);

}

XMFLOAT3 GetMeshWallStPosition(int i)
{
	MESH_WALL *pMesh;
	XMFLOAT3 pos;
	pMesh = &g_aMeshWall[i];
	float rotatew = cosf(pMesh->rot.y);
	float rotatez = sinf(pMesh->rot.y);
	float width = pMesh->fBlockSizeX * fabsf(rotatew);
	float thickness = pMesh->fBlockSizeX * fabsf(rotatez);
	if (width > thickness)
		pos = { pMesh->pos.x + width * 0.5f, 0.0f, pMesh->pos.z };
	else if (thickness > width)
		pos = { pMesh->pos.x, 0.0f, pMesh->pos.z + thickness * 0.5f };

	return pos;
}

XMFLOAT3 GetMeshWallEdPosition(int i)
{
	MESH_WALL *pMesh;
	XMFLOAT3 pos;
	pMesh = &g_aMeshWall[i];
	float rotatew = cosf(pMesh->rot.y);
	float rotatez = sinf(pMesh->rot.y);
	float width = pMesh->fBlockSizeX * fabsf(rotatew);
	float thickness = pMesh->fBlockSizeX * fabsf(rotatez);
	if (width > thickness)
		pos = { pMesh->pos.x - width * 0.5f, 0.0f, pMesh->pos.z };
	else if (thickness > width)
		pos = { pMesh->pos.x, 0.0f, pMesh->pos.z - thickness * 0.5f };

	return pos;
}

int GetMeshWallNum(void)
{
	return g_nNumMeshWall;
}

int GetCeilingWallNum(void)
{
	return g_CeilingWall;
}

void ResetMeshWall(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}
	for (int i = 0; i < MAX_MESH_WALL; i++)
	{
		ZeroMemory(&g_aMeshWall[i].material, sizeof(g_aMeshWall[i].material));
		g_aMeshWall[i].material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_aMeshWall[i].vertexBuffer = NULL;				// 頂点バッファ
		g_aMeshWall[i].indexBuffer = NULL;				// インデックスバッファ
		g_aMeshWall[i].pos = {0.0f, 0.0f, 0.0f};		// ポリゴン表示位置の中心座標
		g_aMeshWall[i].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの回転角
		g_aMeshWall[i].nNumBlockX = 0;
		g_aMeshWall[i].nNumBlockY = 0;					// ブロック数
		g_aMeshWall[i].nNumVertex = 0;					// 総頂点数	
		g_aMeshWall[i].nNumVertexIndex = 0;				// 総インデックス数
		g_aMeshWall[i].nNumPolygon = 0;					// 総ポリゴン数
		g_aMeshWall[i].fBlockSizeX = 0;
		g_aMeshWall[i].fBlockSizeY = 0;					// ブロックサイズ
		g_aMeshWall[i].texNo = WALL_GRAY;						//使用するテクスチャの指定
		g_aMeshWall[i].use = FALSE;
	}
	g_nNumMeshWall = 0;
	g_CeilingWall = 0;
}
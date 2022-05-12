//=============================================================================
//
// アイテムボックス処理 [itembox.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************



enum ITEM_TEXTURE
{
	MATCH_ITEM,
	LIGHTER_ITEM,
	KEY,
	RANDOM_ITEM_2,
	EMPTY,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct BOX
{
	BOOL		use;
	BOOL		open;						//空いているかどうか
	int			slot;						//番号
	int			TexNo;						//アイテムのテクスチャ番号
	float		w, h, tw, th, tx, ty;		// 幅と高さ、テクスチャ座標
	XMFLOAT3	pos;						// ポリゴンの位置
	XMFLOAT4	color;						//色
};

struct ITEM
{
	BOOL		use;
	int			TexNo;						//テクスチャ番号
	int			id;							//アイテムのID
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItembox(void);
void UninitItembox(void);
void UpdateItembox(void);
void DrawItembox(void);
void AddItembox(int TNum);
void IncreaseItembox(int n);
void MoveSelect();
int	ReturnSelect();
void UseItemSelect();
//=============================================================================
//
// その他UI表示処理 [interface.h]
// Author : 
//
//=============================================================================
#pragma once
#define ITEM_MAX					(6)				//アイテムボックスの数
#define ITEM_MIN					(4)				//アイテムボックスの最低限

//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum
{
	CONTROL_TEXTURE,
};


enum UI_TEXTURE
{
	HP_RED_BG,
	HP_RED,
	HP_BAR,
	STAM_BAR,
	OIL_RED,
	OIL_BAR,
	MATCH_NUM,
	MATCH_PIC,
	MIND,
	HURT,
	INSANE,
	ITEM_BOX,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct UI_ELEMENT
{
	BOOL		use;
	int			TexNo;						//テクスチャ番号
	float		w, h, tw, th, tx, ty;		// 幅と高さ、テクスチャ座標
	XMFLOAT3	pos;						// ポリゴンの位置
	XMFLOAT4	color;						//色
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInterface(void);
void UninitInterface(void);
void UpdateInterface(void);
void DrawInterface(void);
void GaugeAnimation(void);
UI_ELEMENT *GetItemBox(void);
int GetItemMax(void);
void IncreaseItemmax(void);
void HurtAnimationOn(void);
void HurtAnimation(void);
void SanityCheck(int sanity);
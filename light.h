//=============================================================================
//
// ���C�g���� [light.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_FOG_ST (50.0f)
#define GAME_FOG_ED (100.0f)
#define GAME_LFOG_ST (100.0f)
#define GAME_LFOG_ED (300.0f)
#define DEBUG_FOG_ST (1000.0f)
#define DEBUG_FOG_ED (1100.0f)
enum
{
	MORNING,
	EVE,
	NIGHT,
	TYPE_NON
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLight(void);
void UpdateLight(void);

void SetLightData(int index, LIGHT *light);
void SetFogData(FOG *fog);
LIGHT *GetLightData(int index);
void UpdateFog(void);
#pragma once
//=============================================================================
//
// �����e�N�X�`�����̕\������ [tutorial_text.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum TUTORIAL_TEXT_NUM
{
	TUTORIAL_LIGHTER,
	TUTORIAL_MATCHI,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorialTex(void);
void UninitTutorialTex(void);
void UpdateTutorialTex(void);
void DrawTutorialTex(void);
void SetTutorialTexture(int i);
BOOL GetTutorialTex(void);
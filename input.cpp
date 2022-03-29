//=============================================================================
//
// ���͏��� [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l
#define DIFFERZONE		50				// �X�e�b�B�N�v�����A�ړ����̗P�\�����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // �}�E�X�̏�����
void UninitMouse();						// �}�E�X�̏I������
HRESULT UpdateMouse();					// �}�E�X�̍X�V����

HRESULT InitializePad(void);			// �p�b�h������
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2   mouseTrigger;	// �����ꂽ�u�Ԃ���ON

//--------------------------------- game pad
static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL, NULL, NULL, NULL };	// �p�b�h�f�o�C�X
static LPDIRECTINPUTEFFECT pGamePadEf[GAMEPADMAX] = { NULL, NULL, NULL, NULL };//�G�t�F�N�g�I�u�W�F�N�g
static DIDEVCAPS pGamePadDevCaps[GAMEPADMAX];
static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padForceFeedbackAxis[GAMEPADMAX] = { NULL, NULL, NULL, NULL };	// �p�b�h�U�����i�����Ή��j
static int		padCount = 0;			// ���o�����p�b�h�̐�
static int		padFFCount = 0;			// �U���p�b�h�̐�
static DIJOYSTATE2		dijs;
static DWORD rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
static LONG  rglDirection[2] = { 1 , 1 };	//0�ɂ������͐U�����Ȃ��B{0 , 1}�Ȃ�E��̂ݐU��

static LONG		countY[GAMEPADMAX];		//Y�����̕ω��𑪂�ϐ�
static LONG		countTime[GAMEPADMAX];	//�ω����Ԃ𑪂�ϐ�
static int		padForceY[GAMEPADMAX];  //Y�����̃X�e�B�b�N�̐����̏��
static BOOL		measureST = FALSE;		//�Q�[�������v�����J�n���邽�߂̃X�C�b�`
static BOOL		measureCntST = FALSE;	//�v���C���[�����v�����J�n����ׂɉ����X�C�b�`
static BOOL		effectExist[GAMEPADMAX] = { FALSE,FALSE,FALSE,FALSE };
//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

 	// �}�E�X�̏�����
	InitializeMouse(hInst, hWnd);
	
	// �p�b�h�̏�����
	InitializePad();

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	// �p�b�h�̏I������
	UninitPad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();
	
	// �}�E�X�̍X�V
	UpdateMouse();
	
	// �p�b�h�̍X�V
	UpdatePad();

}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if(g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if(g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
BOOL GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
BOOL GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
BOOL GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
BOOL GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? TRUE : FALSE;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse,&pMouse,NULL);
	if(FAILED(result) || pMouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// �A�N�Z�X���𓾂�
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void UninitMouse()
{
	if(pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = mouseState;
	// �f�[�^�擾
	result = pMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// �}�E�X�̃{�^�����
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMouse->Acquire();
	}
	return result;
	
}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMouseX(void)
{
	return mouseState.lX;
}
long GetMouseY(void)
{
	return mouseState.lY;
}
long GetMouseZ(void)
{
	return mouseState.lZ;
}
//================================================= game pad
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	if (FAILED(result))return DIENUM_CONTINUE;	// ���̃f�o�C�X���
	return DIENUM_STOP;	
}
//---------------------------------------- �R�[���o�b�N�֐�(�U���@�\)
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = pGamePad[padFFCount]->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	DWORD *pdwNumForceFeedbackAxis = (DWORD*)pContext;
	if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0) (*pdwNumForceFeedbackAxis)++;

	return DIENUM_CONTINUE;
}
//---------------------------------------- ������
HRESULT InitializePad(void)			// �p�b�h������
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, /*DIEDFL_FORCEFEEDBACK*/ DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for ( i=0 ; i<padCount ; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return FALSE; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		//// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		//result = pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//if ( FAILED(result) )
		//	return FALSE; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		
		//�f�o�C�X�̐U���v���p�e�B�̐ݒ�
		pGamePad[i]->EnumObjects(EnumAxesCallback, (VOID*)&padForceFeedbackAxis[i], DIDFT_AXIS);

		//DIEFFECT�\���̂ɒl��ݒ肵�A�U���G�t�F�N�g�𗘗p�ł���悤�ɂ���
		DICONSTANTFORCE cf;
		DIEFFECT        eff;
		ZeroMemory(&eff, sizeof(eff));
		eff.dwSize = sizeof(DIEFFECT);
		eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration = INFINITE;
		eff.dwSamplePeriod = 0;
		eff.dwGain = DI_FFNOMINALMAX;
		eff.dwTriggerButton = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.cAxes = padForceFeedbackAxis[i];
		eff.rgdwAxes = rgdwAxes;
		eff.rglDirection = rglDirection;
		eff.lpEnvelope = 0;
		eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams = &cf;
		eff.dwStartDelay = 0;
		pGamePad[i]->CreateEffect(GUID_ConstantForce, &eff, &pGamePadEf[i], NULL);

		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}
		
	return TRUE;

}
//------------------------------------------- �I������
void UninitPad(void)
{
	for (int i=0 ; i<GAMEPADMAX ; i++) {
		if ( pGamePad[i] )
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
			if (pGamePadEf[i] != NULL) pGamePadEf[i]->Release();
		}
	}

}

//------------------------------------------ �X�V
void UpdatePad(void)
{
	HRESULT			result;
	int				i;
	for ( i=0 ; i<padCount ; i++ ) 
	{
		DWORD lastPadState;
		LONG	oldStickY[GAMEPADMAX];
		LONG	forceStickY = 0;
		oldStickY[i] = dijs.lRz;//0�`65535�̐��l�B(��:0 ��:65535)
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if ( FAILED(result) ) {
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		if (measureCntST && !measureST && oldStickY[i] < dijs.lRz)	//�v���J�n
		{
			measureST = TRUE;
			countY[i] = 0;
			countTime[i] = 0;
		}
		//Y�����̃X�e�B�b�N�ɉ��ω�������A�v���J�n�w�����o�Ă���Ȃ瑬�x�v��
		if ((oldStickY[i] < dijs.lRz) && measureST)
		{
			countY[i] += (dijs.lRz - oldStickY[i]);  //���������Z
			countTime[i] += 1;						 //1�t���[���P�ʂŌv�����Ă���
			padForceY[i] = FORCE_NON;
		}
		else//�ω������������ɂ͂����I����Ă���Ȃ�v�����I�����A������
		{
			if (countY[i] != 0 && countTime[i] >= 3 && countY[i] >= 30)//�v���I���ɐ�����t���鎖�ő��쐫�����߂�
			{
				forceStickY = countY[i] / countTime[i];
				countY[i] = 0;
				countTime[i] = 0;
				measureST = FALSE;
				measureCntST = FALSE;
				if (forceStickY <= 80)			padForceY[i] = FORCE_SLOW;
				else if (forceStickY > 280)		padForceY[i] = FORCE_FAST;
				else if (forceStickY > 80)		padForceY[i] = FORCE_MIDDLE;
			}
			else
			{
				countY[i] = 0;
				countTime[i] = 0;
				measureST = FALSE;
				measureCntST = FALSE;
			}
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if ( dijs.lY < 0 )					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs.lY > 0 )					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs.lX < 0 )					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs.lX > 0 )					padState[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if ( dijs.rgbButtons[0] & 0x80 )	padState[i] |= BUTTON_A;
		//* �a�{�^��
		if ( dijs.rgbButtons[1] & 0x80 )	padState[i] |= BUTTON_B;
		//* �b�{�^��
		if ( dijs.rgbButtons[2] & 0x80 )	padState[i] |= BUTTON_C;
		//* �w�{�^��
		if ( dijs.rgbButtons[3] & 0x80 )	padState[i] |= BUTTON_X;
		//* �x�{�^��
		if ( dijs.rgbButtons[4] & 0x80 )	padState[i] |= BUTTON_Y;
		//* �y�{�^��
		if ( dijs.rgbButtons[5] & 0x80 )	padState[i] |= BUTTON_Z;
		//* �k�{�^��
		if ( dijs.rgbButtons[6] & 0x80 )	padState[i] |= BUTTON_L;
		//* �q�{�^��
		if ( dijs.rgbButtons[7] & 0x80 )	padState[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if ( dijs.rgbButtons[8] & 0x80 )	padState[i] |= BUTTON_START;
		//* �l�{�^��
		if ( dijs.rgbButtons[9] & 0x80 )	padState[i] |= BUTTON_M;

		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
						& padState[i]);					// ��������ON�̂��
		
	}
#ifdef _DEBUG	// �f�o�b�O����\������
	float spot = (float)(dijs.lRz);
	PrintDebugProc("PlayerStick:%f\n", spot);
#endif

}
//----------------------------------------------- ����
BOOL IsButtonPressed(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}

int IsButtonForce(int padNo)
{
	return padForceY[padNo];
}

void SetForceState(BOOL flag)
{
	measureCntST = flag;
}

void InitForce(int padNo)
{
	padForceY[padNo] = FORCE_NON;
}

void padFFAxisStart(int padNo)
{
	if (pGamePadEf[padNo] == NULL)return;	//�G�t�F�N�g�쐬�Ɏ��s���Ă���Ȃ�U�������Ȃ�
	// �U���ݒ�
	if (!effectExist)
	{
		//������:�G�t�F�N�g���Đ������(���݂͖�����U������)
		//������:�G�t�F�N�g�̍Đ����@(0:���G�t�F�N�g�ƍ������čĐ��BDIES_SOLO:���̃G�t�F�N�g��S�Ē�~���Ă���Đ�)
		pGamePadEf[padNo]->Start(1, 0);	//�U���J�n
		effectExist[padNo] = TRUE;
	}
}

void padFFAxisStop(int padNo)
{
	if (pGamePadEf[padNo] == NULL)return;	//�G�t�F�N�g�쐬�Ɏ��s���Ă���Ȃ�U�������Ȃ�

	if (effectExist)
	{
		pGamePadEf[padNo]->Stop();		//�U����~
		effectExist[padNo] = FALSE;
	}
}
//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値
#define DIFFERZONE		50				// ステッィク計測時、移動幅の猶予を取る


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // マウスの初期化
void UninitMouse();						// マウスの終了処理
HRESULT UpdateMouse();					// マウスの更新処理

HRESULT InitializePad(void);			// パッド初期化
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// マウスのダイレクトな状態
static DIMOUSESTATE2   mouseTrigger;	// 押された瞬間だけON

//--------------------------------- game pad
static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL, NULL, NULL, NULL };	// パッドデバイス
static LPDIRECTINPUTEFFECT pGamePadEf[GAMEPADMAX] = { NULL, NULL, NULL, NULL };//エフェクトオブジェクト
static DIDEVCAPS pGamePadDevCaps[GAMEPADMAX];
static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padForceFeedbackAxis[GAMEPADMAX] = { NULL, NULL, NULL, NULL };	// パッド振動情報（複数対応）
static int		padCount = 0;			// 検出したパッドの数
static int		padFFCount = 0;			// 振動パッドの数
static DIJOYSTATE2		dijs;
static DWORD rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
static LONG  rglDirection[2] = { 1 , 1 };	//0にした方は振動しない。{0 , 1}なら右手のみ振動

static LONG		countY[GAMEPADMAX];		//Y方向の変化を測る変数
static LONG		countTime[GAMEPADMAX];	//変化時間を測る変数
static int		padForceY[GAMEPADMAX];  //Y方向のスティックの勢いの情報
static BOOL		measureST = FALSE;		//ゲーム側が計測を開始するためのスイッチ
static BOOL		measureCntST = FALSE;	//プレイヤー側が計測を開始する為に押すスイッチ
static BOOL		effectExist[GAMEPADMAX] = { FALSE,FALSE,FALSE,FALSE };
//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

 	// マウスの初期化
	InitializeMouse(hInst, hWnd);
	
	// パッドの初期化
	InitializePad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// パッドの終了処理
	UninitPad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();
	
	// マウスの更新
	UpdateMouse();
	
	// パッドの更新
	UpdatePad();

}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがありません", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
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
// キーボードの更新
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
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
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
BOOL GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
BOOL GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
BOOL GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
BOOL GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? TRUE : FALSE;
}


//=============================================================================
// マウス関係の処理
//=============================================================================
// マウスの初期化
HRESULT InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// デバイス作成
	result = g_pDInput->CreateDevice(GUID_SysMouse,&pMouse,NULL);
	if(FAILED(result) || pMouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// デバイスの設定
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = pMouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// アクセス権を得る
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
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = pMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = pMouse->Acquire();
	}
	return result;
	
}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
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
//---------------------------------------- コールバック関数
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	if (FAILED(result))return DIENUM_CONTINUE;	// 次のデバイスを列挙
	return DIENUM_STOP;	
}
//---------------------------------------- コールバック関数(振動機能)
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
//---------------------------------------- 初期化
HRESULT InitializePad(void)			// パッド初期化
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, /*DIEDFL_FORCEFEEDBACK*/ DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for ( i=0 ; i<padCount ; i++ ) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return FALSE; // データフォーマットの設定に失敗

		//// モードを設定（フォアグラウンド＆非排他モード）
		//result = pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//if ( FAILED(result) )
		//	return FALSE; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		
		//デバイスの振動プロパティの設定
		pGamePad[i]->EnumObjects(EnumAxesCallback, (VOID*)&padForceFeedbackAxis[i], DIDFT_AXIS);

		//DIEFFECT構造体に値を設定し、振動エフェクトを利用できるようにする
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

		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}
		
	return TRUE;

}
//------------------------------------------- 終了処理
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

//------------------------------------------ 更新
void UpdatePad(void)
{
	HRESULT			result;
	int				i;
	for ( i=0 ; i<padCount ; i++ ) 
	{
		DWORD lastPadState;
		LONG	oldStickY[GAMEPADMAX];
		LONG	forceStickY = 0;
		oldStickY[i] = dijs.lRz;//0～65535の数値。(↑:0 ↓:65535)
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if ( FAILED(result) ) {
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		if (measureCntST && !measureST && oldStickY[i] < dijs.lRz)	//計測開始
		{
			measureST = TRUE;
			countY[i] = 0;
			countTime[i] = 0;
		}
		//Y方向のスティックに下変化があり、計測開始指示が出ているなら速度計測
		if ((oldStickY[i] < dijs.lRz) && measureST)
		{
			countY[i] += (dijs.lRz - oldStickY[i]);  //差分を加算
			countTime[i] += 1;						 //1フレーム単位で計測していく
			padForceY[i] = FORCE_NON;
		}
		else//変化が無いか下にはじき終わっているなら計測を終了し、初期化
		{
			if (countY[i] != 0 && countTime[i] >= 3 && countY[i] >= 30)//計測終了に制限を付ける事で操作性を高める
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

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if ( dijs.lY < 0 )					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs.lY > 0 )					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs.lX < 0 )					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs.lX > 0 )					padState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if ( dijs.rgbButtons[0] & 0x80 )	padState[i] |= BUTTON_A;
		//* Ｂボタン
		if ( dijs.rgbButtons[1] & 0x80 )	padState[i] |= BUTTON_B;
		//* Ｃボタン
		if ( dijs.rgbButtons[2] & 0x80 )	padState[i] |= BUTTON_C;
		//* Ｘボタン
		if ( dijs.rgbButtons[3] & 0x80 )	padState[i] |= BUTTON_X;
		//* Ｙボタン
		if ( dijs.rgbButtons[4] & 0x80 )	padState[i] |= BUTTON_Y;
		//* Ｚボタン
		if ( dijs.rgbButtons[5] & 0x80 )	padState[i] |= BUTTON_Z;
		//* Ｌボタン
		if ( dijs.rgbButtons[6] & 0x80 )	padState[i] |= BUTTON_L;
		//* Ｒボタン
		if ( dijs.rgbButtons[7] & 0x80 )	padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if ( dijs.rgbButtons[8] & 0x80 )	padState[i] |= BUTTON_START;
		//* Ｍボタン
		if ( dijs.rgbButtons[9] & 0x80 )	padState[i] |= BUTTON_M;

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
						& padState[i]);					// しかも今ONのやつ
		
	}
#ifdef _DEBUG	// デバッグ情報を表示する
	float spot = (float)(dijs.lRz);
	PrintDebugProc("PlayerStick:%f\n", spot);
#endif

}
//----------------------------------------------- 検査
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
	if (pGamePadEf[padNo] == NULL)return;	//エフェクト作成に失敗しているなら振動させない
	// 振動設定
	if (!effectExist)
	{
		//第一引数:エフェクトを再生する回数(現在は無限回振動する)
		//第二引数:エフェクトの再生方法(0:他エフェクトと混合して再生。DIES_SOLO:他のエフェクトを全て停止してから再生)
		pGamePadEf[padNo]->Start(1, 0);	//振動開始
		effectExist[padNo] = TRUE;
	}
}

void padFFAxisStop(int padNo)
{
	if (pGamePadEf[padNo] == NULL)return;	//エフェクト作成に失敗しているなら振動させない

	if (effectExist)
	{
		pGamePadEf[padNo]->Stop();		//振動停止
		effectExist[padNo] = FALSE;
	}
}
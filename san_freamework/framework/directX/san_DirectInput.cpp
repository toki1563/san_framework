// DirectInput管理
#include "../../framework.h"
#include "../san_environment.h"

LPDIRECTINPUT8			sanDirectInput::pDI = NULL;				 // DirectInputオブジェクト
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIKeyboard = NULL;		 // Keyboard
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIMouse = NULL;		 // mouse
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIDev[sanJOYSTICK_MAX]; // joystick

DIMOUSESTATE2	sanDirectInput::diMoue;  // マウスの状態
int				sanDirectInput::mouse_x; // マウスの座標X
int				sanDirectInput::mouse_y; // マウスの座標Y
BYTE			sanDirectInput::diKeyboard[256];  // キーボード用
BYTE			sanDirectInput::oldKeyboard[256]; // 1フレーム前のキーボードの状態
DIJOYSTATE		sanDirectInput::diJoystick[sanJOYSTICK_MAX];
DIJOYSTATE		sanDirectInput::oldJoystick[sanJOYSTICK_MAX];

int				sanDirectInput::joystickButtonNum = sizeof(diJoystick[0].rgbButtons) / sizeof(BYTE);
int				sanDirectInput::joystickAxisRange = 1000; // スティックの軸の範囲
float			sanDirectInput::joystickAxisRangeF = 1.0f / (float)joystickAxisRange;

HRESULT sanDirectInput::initialize(void *hinst)
{
	HRESULT hr;

	// IDirectInput8インターフェースの取得
	hr = DirectInput8Create((HINSTANCE)hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	// ジョイスティック情報の初期化
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		pDIDev[i] = NULL;
		ZeroMemory(&diJoystick[i], sizeof(DIJOYSTATE));
		ZeroMemory(&oldJoystick[i], sizeof(DIJOYSTATE));
	}

	// IDirectInputDevice8インターフェースの取得
	// マウス
	hr = pDI->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}
	hr = pDIMouse->SetDataFormat(&c_dfDIMouse2); // データフォーマットの設定
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}

	// キーボード
	hr = pDI->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL);
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}
	hr = pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}

	// ジョイスティック
	// デバイスの列挙
	hr = pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}
	// データ形式の設定
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		if (pDIDev[i] == NULL) continue;

		hr = pDIDev[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr))
		{
			terminate();
			return E_FAIL;
		}
		// フォアグラウンド&非排他モードの設定
		// hr = pDIDev[2]->SetCooperativeLevel(hwndApp,DISCL_NONEEXCLUSIVE | DISCL_FOREGROUND);
		// if(FAILED(hr))
		// {
		// 	  return E_FAIL;
		// }
		// コールバック関数を使って各軸のモードの設定
		hr = pDIDev[i]->EnumObjects(EnumAxisCallback, &i, DIDFT_AXIS);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		// 軸モードを絶対値モードに設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_ABS;
		hr = pDIDev[i]->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		// バッファリングデータを取得するためバッファサイズを設定
		diprop.dwData = 100; // DIDEVICE_BUFFERSIZE
		hr = pDIDev[i]->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		// 入力制御開始
		pDIDev[i]->Acquire();
	}

	if (FAILED(hr)) return hr;
	return S_OK;
}
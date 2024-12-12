//--------------------------------------------------------------//
//	"san_DirectInput.cpp"										//
//	     DirectInput管理											//
//													2024/10/28	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../san_environment.h"

LPDIRECTINPUT8			sanDirectInput::pDI = NULL;				 // DirectInputオブジェクト
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIKeyboard = NULL;		 // Keyboard
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIMouse = NULL;		 // mouse
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIDev[sanJOYSTICK_MAX]; // joystick

DIMOUSESTATE2	sanDirectInput::diMouse;  // マウスの状態
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

	// ジョイスティックの初期化
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
			return E_FAIL;
		}
		// フォアグラウンド&排他モードの設定
		// hr = pDIDev[2]->SetCooperativeLevel(hwndApp,DISCL_NONEEXCLUSIVE | DISCL_FOREGROUND);
		// if(FAILED(hr))
		// {
		// 	  return E_FAIL;
		// }
		// コールバック関数を使用して軸の設定
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
		// バッファリングデータを取得するためのバッファサイズを設定
		diprop.dwData = 100; // DIDEVICE_BUFFERSIZE
		hr = pDIDev[i]->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		// 入力受付開始
		pDIDev[i]->Acquire();
	}

	if (FAILED(hr))  return hr;
	return S_OK;
}

// DirectInputのクリーンアップ
void sanDirectInput::terminate()
{
	// デバイスの入力を終了し、ポインタを開放
	if (pDIKeyboard) pDIKeyboard->Unacquire();
	SAFE_RELEASE(pDIKeyboard);

	if (pDIMouse) pDIMouse->Unacquire();
	SAFE_RELEASE(pDIMouse);

	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		if (pDIDev[i]) pDIDev[i]->Unacquire();
		SAFE_RELEASE(pDIDev[i]);
	}
	SAFE_RELEASE(pDI); // DirectInputインターフェース自体を開放
}

// ジョイスティックを列挙
BOOL CALLBACK sanDirectInput::EnumJoystickCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	DIDEVCAPS g_diDevCaps; // ジョイスティックの能力
	HRESULT hr;

	// 空きを探す
	int id = -1;
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		if (pDIDev[i] == NULL)
		{
			id = i;
			break;
		}
	}
	// 空きがなければ終わり
	if (id < 0)
	{
		return DIENUM_STOP;
	}

	// 列挙されたジョイスティックのインターフェースを取得
	hr = pDI->CreateDevice(pdidInstance->guidInstance, &pDIDev[id], NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	// ジョイスティックの能力を調べる
	g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = pDIDev[id]->GetCapabilities(&g_diDevCaps);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	if (id >= sanJOYSTICK_MAX)
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

// ジョイスティックの軸を列挙
BOOL CALLBACK sanDirectInput::EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	int id = *((int*)pvRef);

	HRESULT hr;
	// 軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -joystickAxisRange;
	diprg.lMax = +joystickAxisRange;
	hr = pDIDev[id]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

void sanDirectInput::setMousePosition(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}

// 入力状態を管理、現在の状態を取得
void sanDirectInput::KeyManager()
{
	HRESULT hr;

	// マウスの状態取得
	hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &diMouse);
	if (FAILED(hr))
	{
		hr = pDIMouse->Acquire();
	}

	// キーボードの状態取得
	memcpy(oldKeyboard, diKeyboard, sizeof(oldKeyboard)); // 前のフレームのキー状態を保存
	hr = pDIKeyboard->GetDeviceState(256, diKeyboard);
	if (FAILED(hr))
	{
		hr = pDIKeyboard->Acquire();
	}

	// ジョイスティック
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		if (pDIDev[i] == NULL) continue;
		memcpy(&oldJoystick[i], &diJoystick[i], sizeof(DIJOYSTATE));
		hr = pDIDev[i]->Poll();
		if (FAILED(hr))	hr = pDIDev[i]->Acquire();
		hr = pDIDev[i]->GetDeviceState(sizeof(DIJOYSTATE), &diJoystick[i]);
	}
	// マウスの状態の設定
	sanMouse::setMouseStatus();
}

// マウスの位置を取得
int sanDirectInput::getMouseX()
{
	return mouse_x;
}

int sanDirectInput::getMouseY()
{
	return mouse_y;
}

// マウスの状態や移動量を取得
DIMOUSESTATE2* sanDirectInput::getMouseState()
{
	return &diMouse;
}

// 現在のキーボードのキーの状態を取得
BYTE *sanDirectInput::getCurrentKeyboardState()
{
	return diKeyboard;
}

// 前のフレームのキーボードのキーの状態を取得
BYTE* sanDirectInput::getOldKeyboardState()
{
	return oldKeyboard;
}

// 現在のジョイスティックの状態を取得
DIJOYSTATE *sanDirectInput::getCurrentJoystick(int id)
{
	if (id < 0 || id >= sanJOYSTICK_MAX) return NULL;
	return &diJoystick[id];
}

// 前のフレームのジョイスティックの状態を取得
DIJOYSTATE *sanDirectInput::getOldJoystick(int id)
{
	if (id < 0 || id >= sanJOYSTICK_MAX) return NULL;
	return &oldJoystick[id];
}

// 指定されたIDのジョイスティックが有効かどうかを判定
bool sanDirectInput::useJoystick(int id)
{
	if (id < 0 || id >= sanJOYSTICK_MAX) return false;
	return pDIDev[id] != NULL;
}

// ジョイスティックのボタン数を返す
int sanDirectInput::getJoystickButtonNum()
{
	return joystickButtonNum;
}

// ジョイスティックの軸の範囲を返す
float sanDirectInput::getJoystickAxisRangeF()
{
	return joystickAxisRangeF;
}
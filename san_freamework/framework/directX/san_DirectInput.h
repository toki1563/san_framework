// DirectInput管理
#pragma once

#define sanJOYSTICK_MAX (4) // 使用できるジョイスティックの最大数

class sanDirectInput
{
private:
	static LPDIRECTINPUT8		pDI;					  // DirectInputオブジェクト
	static LPDIRECTINPUTDEVICE8	pDIKeyboard;			  // デバイス:keyboard
	static LPDIRECTINPUTDEVICE8	pDIMouse;				  // デバイス:mouse
	static LPDIRECTINPUTDEVICE8	pDIDev[sanJOYSTICK_MAX];  // デバイス:joystick

	static DIMOUSESTATE2 diMouse;		   // マウス用
	static int           mouse_x;		   // マウス座標X
	static int           mouse_y;		   // マウス座標ｙ
	static BYTE			 diKeyboard[256];  // キーボード用
	static BYTE          oldKeyboard[256]; // 1フレーム前のキーボードの状態
	static DIJOYSTATE    diJoystick[sanJOYSTICK_MAX];
	static DIJOYSTATE    oldJoystick[sanJOYSTICK_MAX];

	static int           joystickButtonNum;
	static int           joystickAxisRange;
	static float         joystickAxisRangeF;

	static BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static BOOL CALLBACK EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

public :
	static HRESULT initialize(void* hinst);
	static void terminate();

	static void setMousePosition(int x, int y);
	static void KeyManager();

	static int getMouseX();
	static int getMouseY();
	static DIMOUSESTATE2  *getMouseState();
	static BYTE			  *getCurrentKeyboardState();
	static BYTE           *getOldKeyboardState();
	static DIJOYSTATE     *getCurrentJoystick(int id = 0);
	static DIJOYSTATE     *getOldJoystick(int id = 0);
	static bool           useJoystick(int id = 0);

	static int      getJoystickButtomNum();
	static float	getJoystickAxisRangeF();
};
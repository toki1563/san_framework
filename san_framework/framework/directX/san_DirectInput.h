//--------------------------------------------------------------//
//	"san_DirectInput.h"											//
//	     DirectInput管理											//
//													2024/10/28	//
//														sanda	//
//--------------------------------------------------------------//
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
	static DIJOYSTATE    diJoystick[sanJOYSTICK_MAX];  // ジョイスティック用
	static DIJOYSTATE    oldJoystick[sanJOYSTICK_MAX]; // 1フレーム前のジョイスティックの状態

	static int           joystickButtonNum;  // ジョイスティックのボタン数
	static int           joystickAxisRange;  // ジョイスティックの軸範囲
	static float         joystickAxisRangeF; // 軸範囲の正規化(-1~0~1)

	// ジョイスティックを列挙するためのコールバック関数
	static BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	// 軸情報を列挙するためのコールバック関数
	static BOOL CALLBACK EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

public :
	static HRESULT initialize(void* hinst);
	static void terminate();

	// マウスの位置を取得
	static void setMousePosition(int x, int y);
	// キー入力と状態を管理
	static void KeyManager();

	// 現在のマウス座標X/Y取得
	static int getMouseX();
	static int getMouseY();
	static DIMOUSESTATE2  *getMouseState();					// マウスの状態を返す
	static BYTE			  *getCurrentKeyboardState();       // 現在キーボードの状態を返す
	static BYTE           *getOldKeyboardState();			// 1フレーム前のキーボードの状態を返す
	static DIJOYSTATE     *getCurrentJoystick(int id = 0);	// 現在ジョイスティック状態を返す
	static DIJOYSTATE     *getOldJoystick(int id = 0);      // 1フレーム前のジョイスティック状態を返す
	static bool           useJoystick(int id = 0);			// ジョイスティックが使用されているかどうか

	static int      getJoystickButtonNum();	 // 使用しているジョイスティックのボタンの数を返す
	static float	getJoystickAxisRangeF(); // ジョイスティックの軸範囲(正規化)を返す
};
// DirectInput管理
#pragma once

#define sanJOYSTICK_MAX (4) // 使用できるｼﾞｮｲｽﾃｨｯｸの数

class sanDirectInput
{
private:
	static LPDIRECTINPUT8		pDI;					  // DirectInputオブジェクト
	static LPDIRECTINPUTDEVICE8	pDIKeyboard;			  // デバイス:keyboard
	static LPDIRECTINPUTDEVICE8	pDIMouse;				  // デバイス:mouse
	static LPDIRECTINPUTDEVICE8	pDIDev[sanJOYSTICK_MAX];  // デバイス:joystick

};
// DirectInput�Ǘ�
#pragma once

#define sanJOYSTICK_MAX (4) // �g�p�ł���ޮ��è���̐�

class sanDirectInput
{
private:
	static LPDIRECTINPUT8		pDI;					  // DirectInput�I�u�W�F�N�g
	static LPDIRECTINPUTDEVICE8	pDIKeyboard;			  // �f�o�C�X:keyboard
	static LPDIRECTINPUTDEVICE8	pDIMouse;				  // �f�o�C�X:mouse
	static LPDIRECTINPUTDEVICE8	pDIDev[sanJOYSTICK_MAX];  // �f�o�C�X:joystick

};
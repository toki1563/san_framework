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

	static DIMOUSESTATE2 diMoue;		   // �}�E�X�p
	static int           mouse_x;		   // �}�E�X����W
	static int           mouse_y;		   // �}�E�X�̍��W
	static BYTE			 diKeyboard[256];  // �L�[�{�[�h�p
	static BYTE          oldKeyboard[256]; // 1�t���[���O�̃L�[�{�[�h�̏��
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
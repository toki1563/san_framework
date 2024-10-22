// DirectInput�Ǘ�
#include "../../framework.h"
#include "../san_environment.h"

LPDIRECTINPUT8			sanDirectInput::pDI = NULL;				 // DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIKeyboard = NULL;		 // Keyboard
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIMouse = NULL;		 // mouse
LPDIRECTINPUTDEVICE8	sanDirectInput::pDIDev[sanJOYSTICK_MAX]; // joystick

DIMOUSESTATE2	sanDirectInput::diMoue;  // �}�E�X�̏��
int				sanDirectInput::mouse_x; // �}�E�X�̍��WX
int				sanDirectInput::mouse_y; // �}�E�X�̍��WY
BYTE			sanDirectInput::diKeyboard[256];  // �L�[�{�[�h�p
BYTE			sanDirectInput::oldKeyboard[256]; // 1�t���[���O�̃L�[�{�[�h�̏��
DIJOYSTATE		sanDirectInput::diJoystick[sanJOYSTICK_MAX];
DIJOYSTATE		sanDirectInput::oldJoystick[sanJOYSTICK_MAX];

int				sanDirectInput::joystickButtonNum = sizeof(diJoystick[0].rgbButtons) / sizeof(BYTE);
int				sanDirectInput::joystickAxisRange = 1000; // �X�e�B�b�N�̎��͈̔�
float			sanDirectInput::joystickAxisRangeF = 1.0f / (float)joystickAxisRange;

HRESULT sanDirectInput::initialize(void *hinst)
{
	HRESULT hr;

	// IDirectInput8�C���^�[�t�F�[�X�̎擾
	hr = DirectInput8Create((HINSTANCE)hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	// �W���C�X�e�B�b�N���̏�����
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		pDIDev[i] = NULL;
		ZeroMemory(&diJoystick[i], sizeof(DIJOYSTATE));
		ZeroMemory(&oldJoystick[i], sizeof(DIJOYSTATE));
	}

	// IDirectInputDevice8�C���^�[�t�F�[�X�̎擾
	// �}�E�X
	hr = pDI->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}
	hr = pDIMouse->SetDataFormat(&c_dfDIMouse2); // �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}

	// �L�[�{�[�h
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

	// �W���C�X�e�B�b�N
	// �f�o�C�X�̗�
	hr = pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}
	// �f�[�^�`���̐ݒ�
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		if (pDIDev[i] == NULL) continue;

		hr = pDIDev[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr))
		{
			terminate();
			return E_FAIL;
		}
		// �t�H�A�O���E���h&��r�����[�h�̐ݒ�
		// hr = pDIDev[2]->SetCooperativeLevel(hwndApp,DISCL_NONEEXCLUSIVE | DISCL_FOREGROUND);
		// if(FAILED(hr))
		// {
		// 	  return E_FAIL;
		// }
		// �R�[���o�b�N�֐����g���Ċe���̃��[�h�̐ݒ�
		hr = pDIDev[i]->EnumObjects(EnumAxisCallback, &i, DIDFT_AXIS);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		// �����[�h���Βl���[�h�ɐݒ�
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
		// �o�b�t�@�����O�f�[�^���擾���邽�߃o�b�t�@�T�C�Y��ݒ�
		diprop.dwData = 100; // DIDEVICE_BUFFERSIZE
		hr = pDIDev[i]->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		// ���͐���J�n
		pDIDev[i]->Acquire();
	}

	if (FAILED(hr)) return hr;
	return S_OK;
}
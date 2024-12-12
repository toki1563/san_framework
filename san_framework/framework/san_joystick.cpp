// ジョイスティック入力(DirectInput)
#include "../framework.h"
#include "../framework//san_environment.h"

bool sanJoystick::isValid(int id)
{
	return sanDirectInput::useJoystick(id);
}

bool sanJoystick::on(int button, int id)
{
	if (button < 0 || button >= sanDirectInput::getJoystickButtonNum()) return false;
	DIJOYSTATE* js = sanDirectInput::getCurrentJoystick(id);
	return js->rgbButtons[button] != 0;
}

bool sanJoystick::trg(int button, int id)
{
	if (button < 0 || button >= sanDirectInput::getJoystickButtonNum()) return false;
	DIJOYSTATE *jsc = sanDirectInput::getCurrentJoystick(id);
	DIJOYSTATE *jso = sanDirectInput::getOldJoystick(id);
	return jsc->rgbButtons[button] != 0 && jso->rgbButtons[button] == 0;
}

bool sanJoystick::rel(int button, int id)
{
	if (button < 0 || button >= sanDirectInput::getJoystickButtonNum()) return false;
	DIJOYSTATE *jsc = sanDirectInput::getCurrentJoystick(id);
	DIJOYSTATE *jso = sanDirectInput::getOldJoystick(id);
	return jsc->rgbButtons[button] == 0 && jso->rgbButtons[button] != 0;
}

float sanJoystick::leftX(int id)
{
	DIJOYSTATE *js = sanDirectInput::getCurrentJoystick(id);
	return (float)js->lX * sanDirectInput::getJoystickAxisRangeF();
}

float sanJoystick::leftY(int id)
{
	DIJOYSTATE *js = sanDirectInput::getCurrentJoystick(id);
	return (float)js->lY * sanDirectInput::getJoystickAxisRangeF();
}

float sanJoystick::rightX(int id)
{
	DIJOYSTATE *js = sanDirectInput::getCurrentJoystick(id);
	return (float)js->lZ * sanDirectInput::getJoystickAxisRangeF();
}

float sanJoystick::rightY(int id)
{
	DIJOYSTATE *js = sanDirectInput::getCurrentJoystick(id);
	return (float)js->lRz * sanDirectInput::getJoystickAxisRangeF();
}

int sanJoystick::pov(int id)
{
	DIJOYSTATE *js = sanDirectInput::getCurrentJoystick(id);
	return js->rgdwPOV[0];
}
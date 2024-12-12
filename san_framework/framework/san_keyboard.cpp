//--------------------------------------------------------------//
//	"san_keyboard.cpp"											//
//	     キーボード入力(DirectInput)								//
//													2024/11/09	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework//san_environment.h"

bool sanKeyboard::on(BYTE key)
{
	BYTE *cur = sanDirectInput::getCurrentKeyboardState();
	return (cur[key] != 0);
}

bool sanKeyboard::trg(BYTE key)
{
	BYTE *cur = sanDirectInput::getCurrentKeyboardState();
	BYTE *old = sanDirectInput::getOldKeyboardState();
	return (cur[key] != 0 && old[key] == 0);
}

bool sanKeyboard::rel(BYTE key)
{
	BYTE *cur = sanDirectInput::getCurrentKeyboardState();
	BYTE *old = sanDirectInput::getOldKeyboardState();
	return (cur[key] == 0 && old[key] != 0);
}
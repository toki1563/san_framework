//--------------------------------------------------------------//
//	"san_joystick_test.cpp"  						    		//
//	     ジョイスティックテスト									//
//													2024/11/23	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

#define LINE_POS(y)	((float)(y)*16.0f+40.0f)

// 初期化関数
bool SceneJoystickTest::initialize()
{
	testXInput = false;

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		vibrate[i] = false;
	}

	return true;
}

// 終了関数
void SceneJoystickTest::terminate()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		sanXInput::vibration();
	}
	sanFont::setColor(0xffffffff);
}

// 処理関数
void SceneJoystickTest::execute()
{
	if (sanKeyboard::trg(DIK_F1))
	{
		testXInput = !testXInput;
	}

	sanFont::setColor(0xffffffff);
	if (testXInput == false)
	{
		sanFont::print(10.0f, 10.0f, L"test mode : DirectInput");
		sanFont::print(230.0f, 10.0f, L"(press F1 to switch test mode)");
		executeDInput();
	}
	else
	{
		sanFont::print(10.0f, 10.0f, L"test mode : XInput");
		sanFont::print(230.0f, 10.0f, L"(press F1 to switch test mode)");
		executeXInput();
	}
	sanScene::execute();
}

// DirectInput処理
void SceneJoystickTest::executeDInput()
{
	for (int i = 0; i < sanJOYSTICK_MAX; i++)
	{
		float x = 250.0f * (float)i + 100.0f;
		int y = 0;
		DWORD col = 0xffffffff;
		if (sanJoystick::isValid(i) == true)
		{
			sanFont::print(x, LINE_POS(y), col, L"Joystick[%d] : enable.", i);
		}
		else
		{
			col = 0x80ffffff;
			sanFont::print(x, LINE_POS(y), col, L"Joystick[%d] : disable.", i);
		}
		y += 2;

		sanFont::print(x, LINE_POS(y), col, L"leftX : %.3f", sanJoystick::leftX(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"leftY : %.3f", sanJoystick::leftY(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"rightX : %.3f", sanJoystick::rightX(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"rightY : %.3f", sanJoystick::rightY(i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"pov : %d", sanJoystick::pov(i));	y++;

		y++;
		for (int j = 0; j < sanDirectInput::getJoystickButtonNum(); j++)
		{
			sanFont::print(x, LINE_POS(y), col, L"Button[%d] : %d", j, sanJoystick::on(j, i));
			y++;
		}
	}
	sanFont::setColor(0xffffffff);
}

// XInput処理
void SceneJoystickTest::executeXInput()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		float x = 250.0f * (float)i + 100.0f;
		int y = 0;
		DWORD col = 0xffffffff;
		if (sanXInput::isValid(i) == true)
		{
			sanFont::print(x, LINE_POS(y), col, L"Controller[%d] : enable.", i);
		}
		else
		{
			col = 0x80ffffff;
			sanFont::print(x, LINE_POS(y), col, L"Controller[%d] : disable.", i);
		}
		y += 2;

		sanFont::print(x, LINE_POS(y), col, L"leftX : %.3f", sanXInput::leftX(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"leftY : %.3f", sanXInput::leftY(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"rightX : %.3f", sanXInput::rightX(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"rightY : %.3f", sanXInput::rightY(i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"leftT : %.3f", sanXInput::leftTrigger(i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"rightT : %.3f", sanXInput::rightTrigger(i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"DPAD_UP : %d", sanXInput::on(XINPUT_GAMEPAD_DPAD_UP, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"DPAD_DOWN : %d", sanXInput::on(XINPUT_GAMEPAD_DPAD_DOWN, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"DPAD_LEFT : %d", sanXInput::on(XINPUT_GAMEPAD_DPAD_LEFT, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"DPAD_RIGHT : %d", sanXInput::on(XINPUT_GAMEPAD_DPAD_RIGHT, i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"START : %d", sanXInput::on(XINPUT_GAMEPAD_START, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"BACK : %d", sanXInput::on(XINPUT_GAMEPAD_BACK, i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"LEFT_THUMB : %d", sanXInput::on(XINPUT_GAMEPAD_LEFT_THUMB, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"RIGHT_THUMB : %d", sanXInput::on(XINPUT_GAMEPAD_RIGHT_THUMB, i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"LEFT_SHOULDER : %d", sanXInput::on(XINPUT_GAMEPAD_LEFT_SHOULDER, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"RIGHT_SHOULDER : %d", sanXInput::on(XINPUT_GAMEPAD_RIGHT_SHOULDER, i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"A : %d", sanXInput::on(XINPUT_GAMEPAD_A, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"B : %d", sanXInput::on(XINPUT_GAMEPAD_B, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"X : %d", sanXInput::on(XINPUT_GAMEPAD_X, i));	y++;
		sanFont::print(x, LINE_POS(y), col, L"Y : %d", sanXInput::on(XINPUT_GAMEPAD_Y, i));	y++;

		y++;

		sanFont::print(x, LINE_POS(y), col, L"press \"%d\" to vibrate", i + 1);	y++;

		if (sanKeyboard::trg(DIK_1 + i))
		{
			vibrate[i] = !vibrate[i];
			sanXInput::vibration(vibrate[i] ? 0x7fff : 0, i);
		}
	}
	sanFont::setColor(0xffffffff);
}

// 描画関数
void SceneJoystickTest::render()
{
	sanScene::render();
}
//--------------------------------------------------------------//
//	"san_XInput.cpp"			    							//
//	     XInput管理												//
//													2024/11/05	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../san_environment.h"

constexpr float sanXInput::triggerMax;
constexpr float sanXInput::analogMax;

int sanXInput::connectionCheckCount = 0;

sanXInput::stState sanXInput::state[XUSER_MAX_COUNT];

HRESULT sanXInput::initialize()
{
	// XInputEnable(true);

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state[i], sizeof(stState));
	}
	return S_OK;
}

void sanXInput::terminate()
{
	// XInputEnable(false);
}

void sanXInput::KeyManager()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		// 接続状態のチェック(connectionCheckInterval(60)フレーム毎)
		if (connectionCheckCount != 0)
		{
			if (state[i].enable != ERROR_SUCCESS)
			{
				continue;
			}
		}

		// 過去の情報を保存
		memcpy_s(&state[i].past, sizeof(XINPUT_STATE), &state[i].state, sizeof(XINPUT_STATE));

		// 入力状態を取得
		state[i].enable = XInputGetState(i, &state[i].state);

		if (state[i].enable != ERROR_SUCCESS) continue;
		
		// トリガーデータの正規化
		if (state[i].state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			state[i].LeftTrigger = state[i].state.Gamepad.bLeftTrigger * triggerMax;
		}
		else
		{
			state[i].LeftTrigger = 0.0f;
		}
		if (state[i].state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			state[i].RightTrigger = state[i].state.Gamepad.bRightTrigger * triggerMax;
		}
		else
		{
			state[i].RightTrigger = 0.0f;
		}

		// アナログデータの正規化
		// absは絶対値にする関数
		if (abs(state[i].state.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			state[i].LeftX = state[i].state.Gamepad.sThumbLX * analogMax;
		}
		else
		{
			state[i].LeftX = 0.0f;
		}
		if (abs(state[i].state.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			state[i].LeftY = state[i].state.Gamepad.sThumbLY * analogMax;
		}
		else
		{
			state[i].LeftY = 0.0f;
		}
		if (abs(state[i].state.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			state[i].RightX = state[i].state.Gamepad.sThumbRX * analogMax;
		}
		else
		{
			state[i].RightX = 0.0f;
		}
		if (abs(state[i].state.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			state[i].RightY = state[i].state.Gamepad.sThumbRY * analogMax;
		}
		else
		{
			state[i].RightY = 0.0f;
		}
	}

	if (++connectionCheckCount >= connectionCheckInterval)
	{
		connectionCheckCount = 0;
	}
}

bool sanXInput::isValid(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT) return false;
	return state[id].enable == ERROR_SUCCESS;
}

bool sanXInput::on(WORD button, int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT) return false;
	return (state[id].state.Gamepad.wButtons & button) != 0;
}

bool sanXInput::trg(WORD button, int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return false;
	return ((state[id].state.Gamepad.wButtons & button) != 0) && ((state[id].past.Gamepad.wButtons & button) == 0);
}

bool sanXInput::rel(WORD button, int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return false;
	return ((state[id].state.Gamepad.wButtons & button) == 0) && ((state[id].past.Gamepad.wButtons & button) != 0);
}

float sanXInput::leftX(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return 0.0f;
	return state[id].LeftX;
}

float sanXInput::leftY(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return 0.0f;
	return state[id].LeftY;
}

float sanXInput::rightX(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return 0.0f;
	return state[id].RightX;
}

float sanXInput::rightY(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return 0.0f;
	return state[id].RightY;
}

float sanXInput::leftTrigger(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return 0.0f;
	return state[id].LeftTrigger;
}

float sanXInput::rightTrigger(int id)
{
	if (id < 0 || id >= XUSER_MAX_COUNT)return 0.0f;
	return state[id].RightTrigger;
}

void sanXInput::vibration(WORD speed, int id)
{
	if (!isValid(id)) return;
	XINPUT_VIBRATION vib = { speed, speed };
	XInputSetState(id, &vib);
}

void sanXInput::vibration(WORD leftSpeed, WORD rightSpeed,  int id)
{
	if (!isValid(id)) return;
	XINPUT_VIBRATION vib = { leftSpeed, rightSpeed };
	XInputSetState(id, &vib);
}
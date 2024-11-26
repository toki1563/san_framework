//--------------------------------------------------------------//
//	"san_joystick_test.h"  						    			//
//	     ジョイスティックテスト									//
//													2024/11/23	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneJoystickTest : public sanScene
{
private:
	bool testXInput;			   // DirectInput / XInput 切り替え
	bool vibrate[XUSER_MAX_COUNT]; // XInput:振動の動作

	void executeDInput(); // DirectInput処理
	void executeXInput(); // XInput処理

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
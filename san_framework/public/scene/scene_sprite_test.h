//--------------------------------------------------------------//
//	"san_sprite._test.h"  						    			//
//	     スプライトテスト											//
//													2024/11/26	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneSpriteTest : public sanScene
{
private:
	sanSprite* pSprite;

	int Cursor;

	// 操作の種類
	enum eOperation
	{
		PositionX,
		PositionY,
		ScaleX,
		ScaleY,
		Rotate,
		OperationMax,
	};

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
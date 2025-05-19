#pragma once

#include "../object/skinned_cube.h"

class SceneSkinTest : public sanScene
{
private:
	cSkinnedCube* pCube;

	//操作の種類
	enum eOperation
	{
		PositionX,
		PositionY,
		PositionZ,
		RotateX,
		RotateY,
		RotateZ,
		ScaleX,
		ScaleY,
		ScaleZ,
		OperationMax,
	};

	int	Cursor;
	int Select;

	// 極座標の情報
	float radius;
	float theta;
	float phi;
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
	void operateCamera(); // カメラ処理
};

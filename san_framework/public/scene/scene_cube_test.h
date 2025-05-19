//--------------------------------------------------------------//
//	"san_cube_test.h"											//
//	     キューブ												//
//													2024/11/22	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

#include "../object/cube.h"

class SceneCubeTest : public sanScene
{
private:
	cCube* pCube;

	// 極座標の情報
	float radius; // 半径
	float theta; // 平面角
	float phi;   // 仰角
	float value_t;
	float value_r;
	float value_s;
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool  gridAxisActive;  // グリッド&軸表示非表示

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

	int	Cursor; // 番号
	XMVECTOR resetPos;
	XMVECTOR resetRot;
	XMVECTOR resetScl;

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
	void operateCamera(); // カメラ処理
};
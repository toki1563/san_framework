//--------------------------------------------------------------//
//	"san_pick_test.h"	 						    			//
//	     当たり判定												//
//													2024/11/25	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once
#define SPHERE_NUM (7)       // 球の数

class ScenePickTest : public sanScene
{
	// 極座標の情報
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float radians;   // ラジアン
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値

	// 当たり判定モデル
	sanModel* pSphere[SPHERE_NUM];

	// 球の情報
	XMVECTOR sphereCenter;
	float	 sphereRadius;

	//　直線の情報
	XMVECTOR linePos;
	XMVECTOR lineDir;

public:
	bool initialize();
	void terminate();
	void execute();
	void render();

	// 操作の種類
	enum eOperation
	{
		LinePositionX,
		LinePositionY,
		LinePositionZ,
		SpherePositionX,
		SpherePositionY,
		SpherePositionZ,
		OperationMax,
	};
};
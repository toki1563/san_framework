//--------------------------------------------------------------//
//	"san_collision_test.h"	    								//
//	     当たり判定												//
//													2024/11/22	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneCollisionTest : public sanScene
{
private:
	// 極座標の情報
	int	  cursor;	// 番号
	float radius;
	float theta;
	float phi;
	float value_r;
	float radians;
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool gridActive;		// グリッド表示/非表示
	bool axisActive;		// 軸表示/非表示

	// デバック用交差点表示用モデル
	sanModel* pPoint;

	// 直線の情報
	XMVECTOR linePos;
	XMVECTOR lineDir;

	// 平面の情報
	// 平面方程式 ax + by + cz = d
	XMVECTOR planeNormal; // (a, b, c)
	float planeDistance;  // d

	// ヒット判定の情報
	XMVECTOR pointNormal;
	float pointDots[3];

	XMVECTOR point[5]; // 三角形と線のデバッグ用
	XMVECTOR add[3];   // 足し算用

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
		Triangle1PositionX,
		Triangle1PositionY,
		Triangle1PositionZ,
		Triangle2PositionX,
		Triangle2PositionY,
		Triangle2PositionZ,
		Triangle3PositionX,
		Triangle3PositionY,
		Triangle3PositionZ,
		OperationMax,
	};
};
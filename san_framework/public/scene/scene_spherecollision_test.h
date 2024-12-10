//--------------------------------------------------------------//
//	"san_spherecollision_test.h"								//
//	     球の当たり判定											//
//													2024/11/24	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneSphereCollisionTest : public sanScene
{
private:
	// 極座標の情報
	int cursor;    // 番号
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float radians;   // ラジアン
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool  gridActive;  // グリッド表示非表示
	bool  axisActive;  // 軸表示非表示

	// デバッグ用モデル(最近棒距離を示す直線状の1点)
	sanModel* pPoint;
	sanModel* pHitModel[2];
	// 当たり判定モデル
	sanModel* pSphere;

	// 直線の情報
	XMVECTOR linePos;
	XMVECTOR lineDir;

	// 球の情報
	XMVECTOR sphereCenter;
	float	 sphereRadius;

	// 平面情報
	// 平面方程式 ax + by + cz = d
	XMVECTOR planeNormal;  // (a, b, c)
	float planeDistance;   // d

	// ヒット判定の情報
	XMVECTOR pointNormal;
	float    pointDots[3];

	XMVECTOR point[2];  // 三角形と線のデバッグ用
	XMVECTOR add[3];	// 足し算用
	XMVECTOR spherePos; // 球の位置

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
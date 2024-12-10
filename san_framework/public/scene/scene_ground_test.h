//--------------------------------------------------------------//
//	"san_ground_test.h"  									    //
//	     グラウンド												//
//													2024/11/23	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once
#define FENCE_NUM (32)		 // フェンスの数
#define FENCE_RADIUS (37.5f) // フェンスの半径
#define FIELD_RADIUS (36.5f) // フェンスの当たり判定を取る半径

class SceneGroundTest : public sanScene
{
private:
	sanModel* pPlayer;
	sanModel* pGround;
	sanModel* pSky;

	// 重力
	XMVECTOR velocity;
	XMVECTOR gravity;

	// 直線の情報
	XMVECTOR linePos;
	XMVECTOR lineDir;

	// 平面の情報
	XMVECTOR planeNormal;
	float planeDistance;

	// ヒット判定の情報
	XMVECTOR pointNormal;
	float pointDots[3];

	// 極座標の情報
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float moveSpeed; // objectの移動スピード
	float radians;   // ラジアン
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool  gridActive;  // グリッド表示非表示
	bool  axisActive;  // 軸表示非表示
	bool  input; // 入力があるかを示すフラグ
	
public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
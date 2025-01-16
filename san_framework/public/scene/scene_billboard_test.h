//--------------------------------------------------------------//
//	"san_billboard_test.h"  									//
//	     ビルボード												//
//													2024/11/22	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneBillBoardTest : public sanScene
{
private:
	sanBillboard* pBillboard;

	// 極座標の情報
	float radius; // 座標設定
	float theta;
	float phi;
	float value_r;
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool  gridAxisActive;  // グリッド&軸表示非表示
	bool isActiveBillboard; // ビルボードの表示/非表示

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
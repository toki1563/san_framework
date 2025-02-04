//--------------------------------------------------------------//
//	"san_effect_test.h"									        //
//	     エフェクトクラス					                		//
//													2024/12/12	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneEffectTest : public sanScene
{
private:
	sanEffect* pEmitter;

	// 極座標の情報
	int cursor;    // 番号
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float radians;   // ラジアン
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool  gridAxisActive;  // グリッド&軸表示非表示

public:

	bool initialize();
	void terminate();

	void execute();
	void render();
};
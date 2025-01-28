#pragma once
#include "../object/character.h"

class SceneKeyframetest : public sanScene
{
	// キャラクターオブジェクト
	cCharacter* pCharacter;

	// モーションファイルから読み込んだデータ
	sanMotionData *mot_idle;

	
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float radians;   // ラジアン
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool gridAxisActive;  // グリッド&軸表示非表示

public:
	//初期化
	bool initialize();
	//終了
	void terminate();
	//処理
	void execute();
	//描画
	void render();
};
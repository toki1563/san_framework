#pragma once

class SceneKeyframetest : public sanScene
{
	bool debugLine;

	struct stKeyframe
	{
		float time;  // 時間
		float value; // 値
	};

	stKeyframe keyPx[4]; // キーフレーム配列 positionX用
	stKeyframe keyPz[4]; // キーフレーム配列 positionZ用

	void InitializeKeyFramesPx()
	{
		keyPx[0] = { 0.0f, -5.0f };
		keyPx[1] = { 60.0f,  5.0f };
		keyPx[2] = { 120.0f,  0.0f };
		keyPx[3] = { 180.0f, -5.0f };
	}

	void InitializeKeyFramesPz()
	{
		keyPx[0] = { 0.0f, 0.0f };
		keyPx[1] = { 60.0f, 0.0f };
		keyPx[2] = { 120.0f, 5.0f };
		keyPx[3] = { 180.0f, 0.0f };
	}

	float time; // 時間経過

	sanModel* pCube;
	sanModel* pCube2;

	float radius; // 半径
	float theta;
	float phi;

	int Cursor;
	float scrollMove; // 右クリック時のスクロール値


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
#pragma once

class SceneKeyframetest : public sanScene
{
	// キーフレーム構造体
	struct stKeyFrame
	{
		float time;  // 時間
		float value; // 値
	};

	// アニメーションパーツのチャンネル
	struct stMotionChannel
	{
		int partsID;     // パーツの種類
		int channelID;	 // チャンネルの番号
		int keyframeNum;
		stKeyFrame *pkey; // 可変させる
	};

	stMotionChannel channel[2];

	// どこを動かすかの構造体
	enum eChannel
	{
		PosX,
		PosY,
		PosZ,
		RotX,
		RotY,
		RotZ,
	};

	enum eParts
	{
		Body,
		Head,
		ArmL,
		ArmR,
		LegL,
		LegR,
		Max,
	};
	sanModel* pParts[6];

	float time; // 時間経過
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float radians;   // ラジアン
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool reverse;     // 反転フラグ
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
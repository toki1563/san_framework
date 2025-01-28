#pragma once

// キーフレーム構造体
struct stKeyframe
{
	float time;  // 時間
	float value; // 値
};

// アニメーションパーツのチャンネル
struct stMotionChannel
{
	int partsID;     // パーツの種類
	int channelID;	 // チャンネルの番号
	int keyframeNum; // キーフレームの総量
	stKeyframe* pKey; // 可変させる
};

// 動作確認として使用
struct stMotion
{
	// どこを動かすかの構造体
	enum eChannel
	{
		PosX,
		PosY,
		PosZ,
		RotX,
		RotY,
		RotZ,
		ChannelMax,
	};

	int Length;
	int ChannelNum;
	stMotionChannel* pChannel;
};


// 部位ごとに作成
//stMotionChannel walk[2];
//stMotionChannel run[5];
//stMotionChannel jump[8];
//stMotionChannel atk[4];


class cCharacter : public sanObject
{
public:
	enum eParts
	{
		Body,
		Head,
		ArmL,
		ArmR,
		LegL,
		LegR,
		PartsMax,
	};

private:
	sanModel* pParts[eParts::PartsMax];

	float time; // 時間経過

	// 再生中のモーションデータ
	// stMotion* pMotion;
	sanMotionData* pMotion;

public:
	cCharacter();
	~cCharacter();

	void execute();

	// バインドポーズ(キャラの標準状態)に戻す
	void bindPose();

	// void setMotion(stMotion* p);
	void setMotion(sanMotionData* p);

	sanModel* getParts(int i);
	sanModel* getParts(char* name);
};
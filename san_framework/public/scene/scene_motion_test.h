#pragma once

class SceneMotionTest : public sanScene
{
private:

	int Cursor;

	//キャラクターオブジェクト
	cCharacter* pCharacter;

	//モーションの数
	int motionNum;
	//読み込んだモーションデータ
	sanMotionData** motion;

	//モーションファイル読み込み関数
	sanMotionData* loadMotionFile(const WCHAR* motFile);

	//回転補正
	void rotRoll(sanMotionData* p);
	
public:	
	bool initialize();

	void terminate();

	void execute();
	void render();
};

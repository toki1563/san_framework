#pragma once

#include "../object/character.h"

class SceneKeyframeTest : public sanScene
{
private:
	//キャラクターオブジェクト
	cCharacter* pCharacter;

	//読み込んだモーションデータ
	sanMotionData* mot_idle;
	sanMotionData* mot_walk;

	//モーションファイル読み込み関数
	sanMotionData *loadMotionFile(const WCHAR* motFile);
	
public:	
	bool initialize();

	void terminate();

	void execute();
	void render();
};

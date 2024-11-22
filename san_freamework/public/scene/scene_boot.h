#pragma once
// シーン(起動/シーンセレクト)

class SceneBoot : public sanScene
{
private:
	int Cursor;
	static int currentBootScene;

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
	static int getCurrentBootScene();
};
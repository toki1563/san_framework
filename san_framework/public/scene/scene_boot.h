#pragma once
// シーン(起動/シーンセレクト)

class SceneBoot : public sanScene
{
private:
	int Cursor;

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
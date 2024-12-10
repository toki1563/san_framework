//--------------------------------------------------------------//
//	"san_font_test.h"									        //
//	     フォント												//
//													2024/11/22	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneFontTest : public sanScene
{
private:
	int Cursor;  // 選択
	int FontNum; // 使用できるフォントの数

	IDWriteTextFormat** textFormat; // 作成されたフォント

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
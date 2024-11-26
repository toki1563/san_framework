//--------------------------------------------------------------//
//	"san_se._test.h"  						    				//
//	     サウンド												//
//													2024/11/25	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneSeTest : public sanScene
{
private:
	int fileNum;
	sanSound **pSound;

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
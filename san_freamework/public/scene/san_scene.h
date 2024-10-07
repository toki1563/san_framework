// シーンクラス
#pragma once

#define sanOBJECT3D_MAX (512) // 2Dオブジェクトを登録できる最大数
#define sanOBJECT3D_MAX (512) // 3Dオブジェクトを登録できる最大数

class sanScene
{
private:

public:
	sanScene();
	virtual ~sanScene();

	virtual bool initialize() = 0;
	virtual void terminate() = 0;

	virtual void execute();
	virtual void render();
}; 
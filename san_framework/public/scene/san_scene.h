//--------------------------------------------------------------//
//	"san_scene.cpp"												//
//	     シーンクラス												//
//													2024/11/20	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

#define sanOBJECT2D_MAX (512) // 2Dオブジェクトを登録できる最大数
#define sanOBJECT3D_MAX (512) // 3Dオブジェクトを登録できる最大数

class sanScene
{
private:
	sanSprite* pObject2D_Array[sanOBJECT2D_MAX]; // 2Dオブジェクトを登録できる最大数
	sanObject* pObject3D_Array[sanOBJECT3D_MAX]; // 3Dオブジェクトを登録できる最大数

	// 2Dオブジェクトをソートする必要があるか
	static bool UpdateRenderPriority;

public:
	sanScene();
	virtual ~sanScene();

	virtual bool initialize() = 0;
	virtual void terminate() = 0;

	virtual void execute();
	virtual void render();

	// 登録された2Dオブジェクトの描画優先順位が変更されたので、配列をソートをする必要がある
	static void setUpdateRenderPriority();

	// オブジェクトの登録(配列の空きに入れる)
	bool registerObject(sanSprite* p);

	// オブジェクトの破棄(配列から削除/オブジェクト自体をdeleteする)
	void deleteObject(sanSprite* p);

	// オブジェクトの登録(配列の空きを入れる)
	bool registerObject(sanObject* p);

	// オブジェクトの破棄(配列から削除/オブジェクト自体をdeleteする)
	void deleteObject(sanObject* p);
}; 
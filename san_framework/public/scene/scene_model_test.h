//--------------------------------------------------------------//
//	"san_model_test.h"  						    			//
//	     モデル													//
//													2024/11/25	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class SceneModelTest : public sanScene
{
private:
	sanModel* pModel;

	// 極座標の情報
	float radius;
	float theta; 
	float phi;   
	float value_t;
	float value_r;
	float value_s;
	float radiusRoll; // ミドル回転時の値
	float scrollMove; // 右クリック時のスクロール値
	bool  gridActive;  // グリッド表示非表示
	bool  axisActive;  // 軸表示非表示

	//操作の種類
	enum eOperation
	{
		PositionX,
		PositionY,
		PositionZ,
		RotateX,
		RotateY,
		RotateZ,
		ScaleX,
		ScaleY,
		ScaleZ,
		OperationMax,
	};

	int	Cursor;
	XMVECTOR resetPos;
	XMVECTOR resetRot;
	XMVECTOR resetScl;
	XMVECTOR resetCam;
	XMVECTOR resetTag;

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
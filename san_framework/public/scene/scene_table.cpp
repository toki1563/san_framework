// シーンテーブル
#include "../../framework.h"
#include "../../framework/san_environment.h"

//起動時のシーン
eSceneTable initialScene = eSceneTable::Boot;
//現在のシーン
eSceneTable currentScene = (eSceneTable)-1;
//切り替え予約のシーン
eSceneTable reserveScene = initialScene;

// 各シーンの名前
WCHAR SceneName[(int)eSceneTable::SceneMax][32] =
{
	L"Boot",
	L"Game Test",
	L"BillBoard Test",
	L"Ground Test",
	L"Model Test",
	L"Cube Test",
	L"Pick Test",
	L"SphereCollision Test",
	L"Collision Test",
	L"Sprite Test",
	L"Joystick Test",
	L"SE Test",
	L"Font Test",
};

//ステージ切り替え関数(予約)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

// 現在のステージ取得(実処理)
// 0 : セレクト画面
// 1 : テスト画面
// 2 : ゲーム画面
void switchScene()
{
	if (currentScene == reserveScene)
	{
		return;
	}

	sanMainFrame::terminateScene();

	sanScene *pScene = NULL;
	switch (reserveScene)
	{
		case Boot:				  pScene = new SceneBoot();			       break;
		case GameTest:			  pScene = new SceneGameTest();			   break;
		case BillBoardTest:		  pScene = new SceneBillBoardTest();	   break;
		case GroundTest:		  pScene = new SceneGroundTest;			   break;
		case ModelTest:			  pScene = new SceneModelTest();           break;
		case CubeTest:			  pScene = new SceneCubeTest();		       break;
		case PickTest:			  pScene = new ScenePickTest();		       break;
		case SphereCollisionTest: pScene = new SceneSphereCollisionTest(); break;
		case CollisionTest:	      pScene = new SceneCollisionTest();	   break;
		case SpriteTest:	      pScene = new SceneSpriteTest();		   break;
		case JoystickTest:	      pScene = new SceneJoystickTest();	       break;
		case SeTest:		      pScene = new SceneSeTest();	           break;
		case FontTest:		      pScene = new SceneFontTest();		       break;
	}

	sanMainFrame::initializeScene(pScene);

	currentScene = reserveScene;
}
// シーンテーブル
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 起動時のシーン
eSelectTable initialSelectScene  = eSelectTable::SelectBoot;
eTestTable initialTestScene		 = eTestTable::TestBoot;
eGameTable initialGameScene		 = eGameTable::GameBoot;
// 現在のシーン				    
eSelectTable currentSelectScene  = (eSelectTable)-1;
eTestTable currentTestScene		 = (eTestTable)-1;
eGameTable currentGameScene		 = (eGameTable)-1;
// 切り替え予約のシーン			 
eSelectTable reserveSelectScene  = initialSelectScene;
eTestTable reserveTestScene		 = initialTestScene;
eGameTable reserveGameScene		 = initialGameScene;

// 各シーンの名前
WCHAR SelectSceneName[(int)eSelectTable::SceneSelectMax][32] =
{
	L"Boot",
	L"Test Table",
	L"Game Table",
};

WCHAR TestSceneName[(int)eTestTable::SceneTestMax][32] =
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

WCHAR GameSceneName[(int)eGameTable::SceneGameMax][32] =
{
	L"Boot",
};

// 各ステージ切り替え関数(予約)
void switchScene(eTestTable scene)
{
	reserveTestScene = scene;
}

void switchScene(eGameTable scene)
{
	reserveGameScene = scene;
}

// 現在のステージ取得(実処理)
// 0 : セレクト画面
// 1 : テスト画面
// 2 : ゲーム画面
void switchScene(int currentBootScene)
{
	switch (currentBootScene)
	{
	case 1: // テスト画面の時
		if (currentTestScene == reserveTestScene)
		{
			return;
		}
		break;
	case 2: // ゲーム画面の時
		if (currentGameScene == reserveGameScene)
		{
			return;
		}
		break;
	}

	sanMainFrame::terminateScene();

	sanScene *pScene = NULL;
	switch (currentBootScene)
	{
	case 1: // テスト画面の時
		//switch (reserveTestScene)
		//{
		//case Boot:				  pScene = new SceneBoot();			       break;
		//case GameTest:			  pScene = new GameTest();				   break;
		//case BillBoardTest:		  pScene = new SceneBillBoardTest();	   break;
		//case GroundTest:		  pScene = new SceneGroundTest;			   break;
		//case ModelTest:			  pScene = new SceneModelTest();           break;
		//case CubeTest:			  pScene = new SceneCubeTest();		       break;
		//case PickTest:			  pScene = new ScenePickTest();		       break;
		//case SphereCollisionTest: pScene = new SceneSphereCollisionTest(); break;
		//case CollisionTest:	      pScene = new SceneCollisionTest();	   break;
		//case SpriteTest:	      pScene = new SceneSpriteTest();		   break;
		//case JoystickTest:	      pScene = new SceneJoystickTest();	       break;
		//case SeTest:		      pScene = new SceneSeTest();	           break;
		//case FontTest:		      pScene = new SceneFontTest();		       break;
		//}
		break;
	case 2: // ゲーム画面の時
		//switch (reserveTestScene)
		//{
		//case Boot:				  pScene = new SceneBoot();			       break;
		//}
		break;
	}

	sanMainFrame::initializeScene(pScene);

	switch (currentBootScene)
	{
	case 1: // テスト画面の時
		currentTestScene = reserveTestScene;
		break;
	case 2: // ゲーム画面の時
		currentTestScene = reserveTestScene;
		break;
	}
}
// ブート
#include "../../framework.h"
#include "../../framework/san_environment.h"

#define CURSOR_X		(80.0f)  // カーソルのX座標
#define SCENE_NAME_X	(100.0f) // シーン名のX座標	
#define SCENE_NAME_Y	(100.0f) // シーン名のY座標
#define SCENE_NAME_LINE (20.0f)  // シーン名の改行幅

extern WCHAR SceneSelectName [(int)eSelectTable::SceneSelectMax][32];
extern WCHAR SceneTestName[(int)eTestTable::SceneTestMax][32];
extern WCHAR SceneGameName[(int)eGameTable::SceneGameMax][32];

// 初期化関数
bool SceneBoot::initialize()
{
	Cursor = 0;
	currentBootScene = 0;
	return	true;
}

// 終了関数
void SceneBoot::terminate()
{
}

// 処理関数
void SceneBoot::execute()
{
	// 現在のシーン
	switch (currentBootScene)
	{
	case 0:
		if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
		{	//↑キーでカーソルを上に移動
			Cursor = (int)eSelectTable::SceneSelectMax - 1;
		}
		else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= (int)eSelectTable::SceneSelectMax)
		{	//↓キーでカーソルを下に移動
			Cursor = 0;
		}
		else if (sanKeyboard::trg(DIK_SPACE) || sanKeyboard::trg(DIK_RETURN))
		{	//SPACEキーでシーンを切り替え
			if (Cursor == 1) // テストを選択したとき
			{
				currentBootScene = 1;
			}
			else if (Cursor == 2) // ゲームを選択したとき
			{
				currentBootScene = 2;
			}
			Cursor = 0;
		}

		//カーソルの表示
		sanFont::print(CURSOR_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)Cursor, 0xffffff00, L"→");
		//シーン名の表示
		for (int i = 0; i < (int)eSelectTable::SceneSelectMax; i++)
		{
			DWORD col = i == Cursor ? 0xffffff80 : 0xffffffff;
			sanFont::print(SCENE_NAME_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)i, col, SceneSelectName[i]);
		}
		break;
	case 1:
		if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
		{	//↑キーでカーソルを上に移動
			Cursor = (int)eTestTable::SceneTestMax - 1;
		}
		else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= (int)eTestTable::SceneTestMax)
		{	//↓キーでカーソルを下に移動
			Cursor = 0;
		}
		else if (sanKeyboard::trg(DIK_SPACE) || sanKeyboard::trg(DIK_RETURN))
		{	//SPACEキーでシーンを切り替え
			switchScene((eTestTable)Cursor);
		}

		//カーソルの表示
		sanFont::print(CURSOR_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)Cursor, 0xffffff00, L"→");
		//シーン名の表示
		for (int i = 0; i < (int)eTestTable::SceneTestMax; i++)
		{
			DWORD col = i == Cursor ? 0xffffff80 : 0xffffffff;
			sanFont::print(SCENE_NAME_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)i, col, SceneTestName[i]);
		}
		break;
	case 2:
		if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
		{	//↑キーでカーソルを上に移動
			Cursor = (int)eGameTable::SceneGameMax - 1;
		}
		else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= (int)eGameTable::SceneGameMax)
		{	//↓キーでカーソルを下に移動
			Cursor = 0;
		}
		else if (sanKeyboard::trg(DIK_SPACE) || sanKeyboard::trg(DIK_RETURN))
		{	//SPACEキーでシーンを切り替え
			switchScene((eGameTable)Cursor);
		}

		//カーソルの表示
		sanFont::print(CURSOR_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)Cursor, 0xffffff00, L"→");
		//シーン名の表示
		for (int i = 0; i < (int)eGameTable::SceneGameMax; i++)
		{
			DWORD col = i == Cursor ? 0xffffff80 : 0xffffffff;
			sanFont::print(SCENE_NAME_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)i, col, SceneGameName[i]);
		}
		break;
	}

	sanScene::execute();
}

// 描画関数
void SceneBoot::render()
{
	sanScene::render();
}

// getter
int SceneBoot::getCurrentBootScene()
{
	return currentBootScene;
}
#include "../../framework.h"
#include "../../framework/san_environment.h"


//初期化関数
bool SceneMainGame::initialize()
{
	pPlayer = new player(L"data/model/BoxUnityChan/", L"BoxUnityChan.bone");
	pBoss = new boss(L"data/model/BoxUnityChan/", L"BoxUnityChan.bone");
	pGround = new sanModel(L"data/model/stage/", L"stage.vnm");
	pSky = new sanModel(L"data/model/", L"skydome.vnm");
	pBgm = new sanSound(L"data/sound/gamebgm.wav");
	pGameUI = new gameUI();
	pGameCamera = new gameCamera();

	pSky->setLighting(false); // ライティング無効

	pPlayer->setScale(1.5f, 1.5f, 1.5f);
	pBoss->setScale(2.0f, 2.0f, 2.0f);

	// シーンの基底クラスへ登録
	registerObject(pPlayer);
	for (int i = 0; i < pPlayer->getPartsNum(); i++)
	{ // パーツも読み込み
		registerObject(pPlayer->getParts(i));
	}
	registerObject(pBoss);
	for (int i = 0; i < pBoss->getPartsNum(); i++)
	{ // パーツも読み込み
		registerObject(pBoss->getParts(i));
	}
	registerObject(pGround);
	registerObject(pSky);

	// プレイヤーの位置を設定
	pPlayer->setPositionZ(-5.0f);

	// BGMを少し小さく
	pBgm->setVolume(0.2f);

	return true;
}

//終了関数
void SceneMainGame::terminate()
{
	delete pGameCamera;
	delete pGameUI;
	delete pBgm;
	deleteObject(pSky);
	deleteObject(pGround);
	deleteObject(pBoss);
	deleteObject(pPlayer);
}

//処理関数
void SceneMainGame::execute()
{
	// 登録されたオブジェクト実行
	sanScene::execute();

	// BGM再生
	//if (pBgm->isPlaying() == false)
	//{
	//	pBgm->play();
	//}

	pPlayer->execute(pBoss);
	pBoss->execute(pPlayer);
	pGameUI->execute(pPlayer, pBoss);
	pGameCamera->battleCamera(pPlayer, pBoss);
}

//描画関数
void SceneMainGame::render()
{
	pGround->render();
	pSky->render();
	pGameUI->render();
	// ダメージ受け時の表示処理
	if (!pPlayer->getTakeDamageDisPlay())
	{
		pPlayer->playerAllRender();
		for (int i = 0; i < pPlayer->getPartsNum(); i++)
		{ // パーツも読み込み
			pPlayer->getParts(i)->render();
		}
	}
	if (!pBoss->getTakeDamageDisPlay())
	{
		pBoss->playerAllRender();
		for (int i = 0; i < pBoss->getPartsNum(); i++)
		{ // パーツも読み込み
			pBoss->getParts(i)->render();
		}
	}
}


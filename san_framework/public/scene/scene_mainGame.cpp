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
	pSky->setLighting(false); // ライティング無効
	gameUI.initialize();

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

	radians = 180.0f / 3.141592f;
	playerMoveSpeed = 0.1f;
	fallRange = 12.8f;

	// プレイヤーの位置を設定
	pPlayer->setPositionZ(-5.0f);

	// BGMを少し小さく
	pBgm->setVolume(0.2f);

	// プレイヤーからカメラまでの距離と高さ
	cameraDistance = 4.0f;
	cameraHeight = 8.0f;

	// プレイヤーとエネミーの位置を取得
	XMVECTOR playerPosition = *pPlayer->getPosition();
	XMVECTOR bossPosition = *pBoss->getPosition();

	// プレイヤーの向きを取得
	XMMATRIX playerWorld = *pPlayer->getWorld();
	XMVECTOR playerForward = playerWorld.r[2]; // プレイヤーの前方向

	// プレイヤーの後ろの位置を計算
	XMVECTOR cameraPosition = XMVectorAdd(playerPosition, XMVectorScale(playerForward, -cameraDistance));
	cameraPosition = XMVectorAdd(cameraPosition, XMVectorSet(0.0f, cameraHeight, 0.0f, 0.0f)); // 高さ調整

	// プレイヤーの後ろの位置にカメラを設定
	sanCamera::setPosition(&cameraPosition);

	// カメラのターゲットをボスの位置に設定
	sanCamera::setTarget(&bossPosition);

	return true;
}

//終了関数
void SceneMainGame::terminate()
{
	gameUI.terminate();
	delete pBgm;
	deleteObject(pSky);
	deleteObject(pGround);
	deleteObject(pBoss);
	deleteObject(pPlayer);
}

//処理関数
void SceneMainGame::execute()
{
	 // プレイヤーとエネミーの位置を取得
	XMVECTOR playerPosition = *pPlayer->getPosition();
	XMVECTOR bossPosition = *pBoss->getPosition();

	// プレイヤーの向きを取得
	XMMATRIX playerWorld = *pPlayer->getWorld();
	XMVECTOR playerForward = playerWorld.r[2]; // プレイヤーの前方向

	// プレイヤーの後ろの位置を計算
	XMVECTOR cameraPosition = XMVectorAdd(playerPosition, XMVectorScale(playerForward, -cameraDistance));
	cameraPosition = XMVectorAdd(cameraPosition, XMVectorSet(0.0f, cameraHeight, 0.0f, 0.0f)); // 高さ調整

	// プレイヤーの後ろの位置にカメラを設定
	sanCamera::setPosition(&cameraPosition);

	// カメラのターゲットをボスの位置に設定
	sanCamera::setTarget(&bossPosition);

	// 登録されたオブジェクト実行
	sanScene::execute();


	// BGM再生
	//if (pBgm->isPlaying() == false)
	//{
	//	pBgm->play();
	//}

	// ゲームクリア時の処理
	if (pBoss->getIsDead())
	{
		// カメラを上に向けてIsDeadと表示させる
	}
	// ゲームオーバー時の処理
	else if (pPlayer->getIsDead())
	{

	}

	pPlayer->execute(pBoss);
	pBoss->execute(pPlayer);
	gameUI.execute(pPlayer, pBoss);
}

//描画関数
void SceneMainGame::render()
{
	pGround->render();
	pSky->render();
	gameUI.render();
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


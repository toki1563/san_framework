#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneMainGame::initialize()
{
	pPlayer = new player(L"data/model/Player_1/", L"Player.vnm");
	pBoss = new boss(L"data/model/Player_2/", L"Player.vnm");
	pGround = new sanModel(L"data/model/stage/", L"stage.vnm");
	pSky = new sanModel(L"data/model/", L"skydome.vnm");
	pBgm = new sanSound(L"data/sound/gamebgm.wav");
	pSky->setLighting(false); // ライティング無効

	// シーンの基底クラスへ登録
	registerObject(pPlayer);
	registerObject(pBoss);
	registerObject(pGround);
	registerObject(pSky);

	radians = 180.0f / 3.141592f;
	playerMoveSpeed = 0.1f;
	fallRange = 12.8f;

	// プレイヤーの位置を設定
	pPlayer->setPositionZ(-5.0f);

	// BGMを少し小さく
	pBgm->setVolume(0.2f);

	// カメラ座標の計算
	// 回転してない基準となる軸ベクトル
	XMVECTOR v = XMVectorSet(0.0f, -7.0f, -25.0f, 0.0f);
	// 回転マトリクスを作成
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(0.6f, 0.0f, 0.0f);
	// 基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する)
	v = XMVector3TransformNormal(v, rotate);
	// XMVECTORの変数にまとめる
	XMVECTOR CamPos = v;
	// カメラの座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);
	return true;
}

//終了関数
void SceneMainGame::terminate()
{
	delete pBgm;
	deleteObject(pSky);
	deleteObject(pGround);
	deleteObject(pBoss);
	deleteObject(pPlayer);
}

//処理関数
void SceneMainGame::execute()
{
	sanScene::execute();

	// BGM再生
	//if (pBgm->isPlaying() == false)
	//{
	//	pBgm->play();
	//}

	// プレイヤーの移動
	pPlayer->execute(pBoss);
	pBoss->execute(pPlayer);
}

//描画関数
void SceneMainGame::render()
{
	pGround->render();
	pSky->render();
	// ダメージ受け時の表示処理
	if (!pPlayer->getTakeDamageDisPlay())
	{
		pPlayer->playerAllRender();
	}
	if (!pBoss->getTakeDamageDisPlay())
	{
		pBoss->playerAllRender();
	}
}
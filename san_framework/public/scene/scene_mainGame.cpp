#include "../../framework.h"
#include "../../framework/san_environment.h"

#define OUTPUT_MOTION_DATA (0)

#define FULL_PATH_MAX	(256)
#define FILE_PATH_MAX	(128)

WCHAR playerMotionFolder[] = L"data/model/BoxUnityChan/motion/";

WCHAR playerMotionFile[][FILE_PATH_MAX] =
{
	L"DAMAGED00.mot", //  0 : ちょいダメージ
	L"DAMAGED01.mot", //  1 : 吹っ飛び
	L"HANDUP00_R.mot",//  2 : 片手上げ
	L"JUMP00.mot",	  //  3 : 高ジャンプ
	L"JUMP00B.mot",	  //  4 : 低ジャンプ
	L"JUMP01.mot",	  //  5 : 優勝喜び1　
	L"JUMP01B.mot",	  //  6 : 優勝喜び2
	L"LOSE00.mot",	  //  7 : 頭抱え込む
	L"REFLESH00.mot", //  8 : 疲れた膝に手を置く
	L"RUN00_F.mot",	  //  9 : 走る
	L"RUN00_L.mot",	  // 10 : 走る左向く
	L"RUN00_R.mot",	  // 11 : 走る右向く
	L"SLIDE00.mot",	  // 12 : スライディング
	L"UMATOBI00.mot", // 13 : 跳び箱
	L"WAIT00.mot",	  // 14 : 通常立ち
	L"WAIT01.mot",	  // 15 : 手を前に伸ばしてのびのび
	L"WAIT02.mot",	  // 16 : 左右にルンルン
	L"WAIT03.mot",	  // 17 : にっこにっこにー
	L"WAIT04.mot",	  // 18 : まわし蹴り
	L"WALK00_B.mot",  // 19 : 後ろ見ながら歩く
	L"WALK00_F.mot",  // 20 : 正面見ながら歩く
	L"WALK00_L.mot",  // 21 : 左見ながら歩く
	L"WALK00_R.mot",  // 22 : 右見ながら歩く
	L"WIN00.mot",	  // 23 : 勝利ポーズ
};


//初期化関数
bool SceneMainGame::initialize()
{
	pPlayer = new player(L"data/model/BoxUnityChan/", L"BoxUnityChan.bone");
	pBoss = new boss(L"data/model/Player_2/", L"DogPBR.vnm");
	pGround = new sanModel(L"data/model/stage/", L"stage.vnm");
	pSky = new sanModel(L"data/model/", L"skydome.vnm");
	pBgm = new sanSound(L"data/sound/gamebgm.wav");
	pSky->setLighting(false); // ライティング無効
	gameUI.initialize();

	pPlayer->setScale(1.5f, 1.5f, 1.5f);
	pBoss->setScale(2.0f, 2.0f, 2.0f);

	//モーションファイルの読み込み
	playerMotionNum = sizeof(playerMotionFile) / (sizeof(WCHAR) * FILE_PATH_MAX);
	playerMotion = new sanMotionData * [playerMotionNum];
	for (int i = 0; i < playerMotionNum; i++)
	{
		WCHAR path[FULL_PATH_MAX];
		wsprintf(path, L"%s%s", playerMotionFolder, &playerMotionFile[i][0]);

		playerMotion[i] = loadMotionFile(path);
		assert(playerMotion[i] != NULL);

		//回転の補正
		rotRoll(playerMotion[i]);
	}

	// シーンの基底クラスへ登録
	registerObject(pPlayer);
	for (int i = 0; i < pPlayer->getPartsNum(); i++)
	{ // パーツも読み込み
		registerObject(pPlayer->getParts(i));
	}
	registerObject(pBoss);
	registerObject(pGround);
	registerObject(pSky);

	radians = 180.0f / 3.141592f;
	playerMoveSpeed = 0.1f;
	fallRange = 12.8f;

	// プレイヤーの位置を設定
	pPlayer->setPositionZ(-5.0f);

	pPlayer->setMotion(playerMotion[14]);

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
	for (int i = 0; i < playerMotionNum; i++)
	{
		if (!playerMotion[i])continue;
		delete[] playerMotion[i];
		playerMotion[i] = NULL;
	}
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

	if (pPlayer->getIsCanAtk())
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
	}
}

//モーションファイル読み込み関数
sanMotionData* SceneMainGame::loadMotionFile(const WCHAR* motFile)
{
	if (!motFile)return NULL;

	//戻り値用変数
	sanMotionData* pMot = NULL;

	//モーションファイルの読み込み
	FILE* fp = NULL;
	if (_wfopen_s(&fp, motFile, L"rb") != 0)
	{
		//ファイルが開けなかった
		return NULL;
	}

	//ファイルサイズの取得
	long size = 0;
	fseek(fp, 0, SEEK_END);	//ファイルポインタを最後尾に移動
	size = ftell(fp);	//現在のファイルポインタ位置を取得(Byte)
	fseek(fp, 0, SEEK_SET);	//ファイルポインタを先頭に移動

	//ファイルサイズ分のメモリを確保
	pMot = (sanMotionData*)new BYTE[size];

	//ファイルの中身を全て取得
	fread(pMot, size, 1, fp);

	//ファイルを閉じる
	fclose(fp);

#if OUTPUT_MOTION_DATA
	//出力Windowへのログ表示
	sanFont::output(L"Length : %.3f\n", pMot->Length);
	sanFont::output(L"ChannelNum : %d\n", pMot->ChannelNum);
	sanFont::output(L"KeyframeNum : %d\n", pMot->KeyFrameNum);

	//チャンネルデータへのアクセス
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)pMot) + pMot->ChannelAccess);

	for (int i = 0; i < (int)pMot->ChannelNum; i++)
	{
		//char文字列をWCHAR文字列に変換
		WCHAR name[256];
		size_t ret;
		mbstowcs_s(&ret, name, 256, channel[i].Name, strlen(channel[i].Name));

		sanFont::output(L"Channel[%d].Name : %s\n", i, name);
		sanFont::output(L"Channel[%d].ChannelID : %d\n", i, channel[i].ChannelID);
		sanFont::output(L"Channel[%d].KeyFrameNum : %d\n", i, channel[i].KeyFrameNum);
		sanFont::output(L"Channel[%d].StartIndex : %d\n", i, channel[i].StartIndex);
	}

	//キーフレームデータへのアクセス
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)pMot) + pMot->KeyFrameAccess);

	for (int i = 0; i < (int)pMot->KeyFrameNum; i++)
	{
		sanFont::output(L"key[%d].Time : %f\n", i, key[i].Time);
		sanFont::output(L"key[%d].Value : %f\n", i, key[i].Value);
	}
#endif

	return pMot;
}


//回転補正
void SceneMainGame::rotRoll(sanMotionData* p)
{
	if (!p)return;

	//チャンネルデータへのアクセス
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)p) + p->ChannelAccess);

	//キーフレームデータへのアクセス
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)p) + p->KeyFrameAccess);

	for (int i = 0; i < (int)p->ChannelNum; i++)
	{
		//Rotationのチャンネルのみを対象とする
		if (channel[i].ChannelID != eMotionChannel::RotX && channel[i].ChannelID != eMotionChannel::RotY && channel[i].ChannelID != eMotionChannel::RotZ)continue;

		//各チャンネルにおけるキーフレーム配列の最初
		sanMotionData_KeyFrame* k = key + channel[i].StartIndex;

		//チャンネル内での最大値と最小値を求める
		float min = 100.0f;
		float max = -100.0f;
		for (int j = 0; j < channel[i].KeyFrameNum; j++)
		{
			if (k[j].Value > max)max = k[j].Value;
			if (k[j].Value < min)min = k[j].Value;
		}

		//最大値と最小値の差が270度以上あれば補正する(中間値以上の値を360度マイナスする)
		if (max - min < XMConvertToRadians(270.0f))continue;
		float avr = (min + max) * 0.5f;
		for (int j = 0; j < channel[i].KeyFrameNum; j++)
		{
			if (k[j].Value > avr)k[j].Value -= XMConvertToRadians(360.0f);
		}
	}
}

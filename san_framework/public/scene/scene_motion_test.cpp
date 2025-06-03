#include "../../framework.h"
#include "../../framework/san_environment.h"

#define OUTPUT_MOTION_DATA (0)

#define FULL_PATH_MAX	(256)
#define FILE_PATH_MAX	(128)

WCHAR motionFolder[] = L"data/model/BoxUnityChan/motion/";



WCHAR motionFile[][FILE_PATH_MAX] =
{
	L"DAMAGED00.mot",
	L"DAMAGED01.mot",
	L"HANDUP00_R.mot",
	L"JUMP00.mot",
	L"JUMP00B.mot",
	L"JUMP01.mot",
	L"JUMP01B.mot",
	L"LOSE00.mot",
	L"REFLESH00.mot",
	L"RUN00_F.mot",
	L"RUN00_L.mot",
	L"RUN00_R.mot",
	L"SLIDE00.mot",
	L"UMATOBI00.mot",
	L"WAIT00.mot",
	L"WAIT01.mot",
	L"WAIT02.mot",
	L"WAIT03.mot",
	L"WAIT04.mot",
	L"WALK00_B.mot",
	L"WALK00_F.mot",
	L"WALK00_L.mot",
	L"WALK00_R.mot",
	L"WIN00.mot",
};


//初期化
bool SceneMotionTest::initialize()
{
	sanLight::initialize();
	sanLight::setLightDir(-3.0f, -7.0f, -4.0f);
	sanLight::setLightCol(0.5f, 0.5f, 0.5f);
	sanLight::setAmbient(0.5f, 0.5f, 0.5f);

	sanCamera::initialize();
	sanCamera::setPosition(0.0f, 2.0f, 3.5f);
	sanCamera::setTarget(0.0f, 1.0f, 0.0f);

	//モーションファイルの読み込み
	motionNum = sizeof(motionFile) / (sizeof(WCHAR) * FILE_PATH_MAX);

	motion = new sanMotionData*[motionNum];
	for (int i = 0; i < motionNum; i++)
	{
		WCHAR path[FULL_PATH_MAX];
		wsprintf(path, L"%s%s", motionFolder, &motionFile[i][0]);

		motion[i] = loadMotionFile(path);
		assert(motion[i] != NULL);

		//回転の補正
		rotRoll(motion[i]);
	}

	//キャラクターの作成
	pCharacter = new cCharacter(L"data/model/BoxUnityChan/", L"BoxUnityChan.bone");

	//キャラクターとパーツの登録
	registerObject(pCharacter);
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		registerObject(pCharacter->getParts(i));
	}

	// pCharacter->setMotion(motion[0]);

	Cursor = 0;

	return true;
}

//終了関数
void SceneMotionTest::terminate()
{
	for (int i = 0; i < motionNum; i++)
	{
		if (!motion[i])continue;
		delete[] motion[i];
		motion[i] = NULL;
	}
	deleteObject(pCharacter);
}

//処理関数
void SceneMotionTest::execute()
{
	if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = motionNum - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= motionNum)
	{
		Cursor = 0;
	}
	if (sanKeyboard::on(DIK_LEFT))
	{
		pCharacter->addRotationY(XMConvertToRadians(3.0f));
	}
	else if (sanKeyboard::on(DIK_RIGHT))
	{
		pCharacter->addRotationY(-XMConvertToRadians(3.0f));
	}
	else if (sanKeyboard::trg(DIK_SPACE))
	{
		pCharacter->setMotion(motion[Cursor]);
	}

	//操作方法の表示
	sanFont::print(30.0f, 10.0f, 0xffffffff, L"↑↓  : カーソル移動");
	sanFont::print(30.0f, 30.0f, 0xffffffff, L"SPACE : モーション再生");
	sanFont::print(30.0f, 50.0f, 0xffffffff, L"←→  : モデル回転 (RotY = %.3f)", XMConvertToDegrees(pCharacter->getRotationY()));
	sanFont::print(350.0f, 50.0f, 0xffffffff, L"モデルのRotV : %d", pCharacter->getRotationY());

	//カーソルの表示
	sanFont::print(10.0f, 130.0f + 20.0f * (float)Cursor, 0xffffff00, L"→");

	//見出しの表示
	sanFont::print(30.0f, 90.0f, 0xffffffff, L"[MotionFile]");
	sanFont::print(200.0f, 90.0f, 0xffffffff, L"[ChannelNum]");
	sanFont::print(350.0f, 90.0f, 0xffffffff, L"[KeyFrameNum]");
	//モーション情報の表示
	for (int i = 0; i < motionNum; i++)
	{   
		DWORD col = pCharacter->getMotion() == motion[i] ? 0xffffff00 : 0xffffffff;
		sanFont::print(30.0f, 130.0f + 20.0f * (float)i, col, &motionFile[i][0]);
		sanFont::print(200.0f, 130.0f + 20.0f * (float)i, col, L"%d", motion[i]->ChannelNum);
		sanFont::print(350.0f, 130.0f + 20.0f * (float)i, col, L"%d", motion[i]->KeyFrameNum);
	}

	// キャラクターのパーツを取得してsanObject* 
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		sanObject* pChild = pCharacter->getParts(i);
		if (pChild == NULL) continue;
		sanObject* pParent = pChild->getParent();
		if (pParent == NULL) continue;

		XMVECTOR v, m;
		pChild->getWorldPosition(&v);
		pParent->getWorldPosition(&m);
		sanDebugDraw::Line(&v, &m);
	}

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	sanScene::execute();
}

//描画関数
void SceneMotionTest::render()
{
	sanScene::render();
}

//モーションファイル読み込み関数
sanMotionData* SceneMotionTest::loadMotionFile(const WCHAR* motFile)
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
void SceneMotionTest::rotRoll(sanMotionData* p)
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

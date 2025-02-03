#include "../../framework.h"
#include "../../framework/san_environment.h"

#define OUTPUT_MOTION_DATA (1)

//初期化
bool SceneKeyframeTest::initialize()
{
	sanLight::initialize();
	sanLight::setLightDir(-3.0f, -7.0f, -4.0f);

	sanCamera::initialize();
	sanCamera::setPosition(-3.0f, 5.0f, 10.0f);
	sanCamera::setTarget(0.0f, 1.5f, 0.0f);



	//モーションファイルの読み込み
	mot_idle = loadMotionFile(L"data/motion/motion_idle.mot");
	assert(mot_idle != NULL);

	mot_walk = loadMotionFile(L"data/motion/motion_walk.mot");
	assert(mot_walk != NULL);



	//キャラクターの作成
	pCharacter = new cCharacter(L"data/model/bear_part/", L"bear_part.bone");

	//キャラクターとパーツの登録
	registerObject(pCharacter);
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		registerObject(pCharacter->getParts(i));
	}

	//モーションの再生
	pCharacter->setMotion(mot_idle);

	return true;
}

//終了関数
void SceneKeyframeTest::terminate()
{
	deleteObject(pCharacter);
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		deleteObject(pCharacter->getParts(i));
	}

	//モーションデータの削除
	if (!mot_idle)
	{
		delete[] mot_idle;
		mot_idle = NULL;
	}
	if (!mot_walk)
	{
		delete[] mot_walk;
		mot_walk = NULL;
	}
}

//処理関数
void SceneKeyframeTest::execute()
{
	//モーションの切り替え
	if (sanKeyboard::trg(DIK_1))
	{	//待機
		pCharacter->setMotion(mot_idle);
	}
	else if (sanKeyboard::trg(DIK_2))
	{	//歩き
		pCharacter->setMotion(mot_walk);
	}
	else if (sanKeyboard::trg(DIK_3))
	{	//モーションなし
		pCharacter->setMotion(NULL);
	}

	sanFont::print(100.0f, 100.0f, pCharacter->getMotion() == mot_idle ? 0xffffff00 : 0xffffffff, L"1. idle");
	sanFont::print(100.0f, 120.0f, pCharacter->getMotion() == mot_walk ? 0xffffff00 : 0xffffffff, L"2. walk");
	sanFont::print(100.0f, 140.0f, pCharacter->getMotion() == NULL ? 0xffffff00 : 0xffffffff, L"3. null");

	sanFont::print(100.0f, 180.0f, 0xffffffff, L"time : %.1f / %.1f", pCharacter->getTime(), pCharacter->getMotion()!=NULL ? pCharacter->getMotion()->Length : 0.0f);

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	sanScene::execute();
}

//モーションファイル読み込み関数
sanMotionData *SceneKeyframeTest::loadMotionFile(const WCHAR* motFile)
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
	sanMotionData_Channel* channel =	(sanMotionData_Channel*)(((BYTE*)pMot) + pMot->ChannelAccess);

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


//描画関数
void SceneKeyframeTest::render()
{
	sanScene::render();
}

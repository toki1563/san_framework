#include "../../framework.h"
#include "../../framework/san_environment.h"

#include <process.h>

//--------------------------------
//※非同期読み込み中にシーンを出ると、シーンが切り替わっても、読み込みスレッドが残ったまま止まらずにエラーが起きます。
//--------------------------------

#define MODEL_NUM_X (5)
#define MODEL_NUM_Y (5)
#define MODEL_NUM (MODEL_NUM_X*MODEL_NUM_Y)

#define MODEL_INTERVAL (2.5f)

CRITICAL_SECTION SceneAsyncTest::cs;

int SceneAsyncTest::count = 0;

unsigned __stdcall SceneAsyncTest::loadAsync(void *lpx)
{
	ThreadArg* arg = (ThreadArg*)lpx;

	for (int i = 0; i < arg->num; i++)
	{
		Sleep(200);	//わかりやすくするため、わざと少し待つ

		arg->ppModel[i] = new sanModel(L"data/model/bear/", &arg->path[256 * i]);

		//クリティカルセクションによるデータの保護(他スレッドからの競合状態を防ぐ)
		EnterCriticalSection(&cs);
		count++;
		LeaveCriticalSection(&cs);

	}
	return 0;
}

//初期化
bool SceneAsyncTest::initialize()
{
	//カメラの初期配置
	sanCamera::setPosition(0.0f, 5.0f, 10.0f);
	sanCamera::setTarget(0.0f, 0.0f, 0.0f);

	//非同期読み込みの準備
	count = 0;
	num = 0;
	ZeroMemory(path, sizeof(path));

	//読み込むモデルの数とファイル名の用意
	for (int i = 0; i < MODEL_NUM; i++)
	{
		wsprintf(&path[num++][0], L"bear.vnm");
	}

	//モデルデータを作成するポインタの用意
	pModel = new sanModel * [num];
	ZeroMemory(pModel, sizeof(sanModel*) * num);

	//クリティカルセクションの初期化
	InitializeCriticalSection(&cs);

	//スレッドに送る情報の作成
	unsigned int thID;
	arg.num = num;
	arg.path = (WCHAR*)path;
	arg.ppModel = pModel;

	//非同期読み込み用スレッドの作成と開始
	hTh = (HANDLE)_beginthreadex(
		NULL,			//SECURITY_ATTRIBUTES 構造体へのポインター
		0,				//新しいスレッドのスタック サイズまたは 0
		loadAsync,		//新しいスレッドの実行を開始するルーチンの開始アドレス
		&arg,			//新しいスレッドに渡される引数リストまたは NULL
		0,				//新しいスレッドの初期状態を制御するフラグ
		&thID);			//スレッド識別子を受け取る 32 ビット変数へのポインター
	if (hTh == 0) { sanFont::output(L"fail to create thread %d.\n"); }

	pastCount = 0;
	time = 0;

	return true;
}

//終了
void SceneAsyncTest::terminate()
{
	for (int i = 0; i < num; i++)
	{
		if (pModel[i] == NULL)continue;
		sanScene::deleteObject(pModel[i]);
		pModel[i] = NULL;
	}
	delete pModel;
}

//処理関数
void SceneAsyncTest::execute()
{
	if (hTh)	//読み込み中
	{
		//作成が完了したモデルの初期設定とシーン基底クラスへの登録
		while (pastCount < count)
		{
			float sx = -((float)(MODEL_NUM_X - 1) * MODEL_INTERVAL) / 2.0f;
			float sz = -((float)(MODEL_NUM_Y - 1) * MODEL_INTERVAL) / 2.0f;

			float x = (float)(pastCount % MODEL_NUM_X);
			float z = (float)(pastCount / MODEL_NUM_Y);

			pModel[pastCount]->setPosition(sx + x * (float)MODEL_INTERVAL, 0.0f, sz + z * (float)MODEL_INTERVAL);

			sanScene::registerObject(pModel[pastCount]);

			pastCount++;
		}

		//スレッドの状態を調べる
		DWORD exitCode;
		BOOL error_code = GetExitCodeThread(
			hTh,      // スレッドハンドル
			&exitCode   // 終了コード
		);
		if (exitCode != STILL_ACTIVE)
		{	//スレッドの処理が完了
			CloseHandle(hTh);	//ハンドルをクローズ
			hTh = 0;

			DeleteCriticalSection(&cs);	//クリティカルセクションを削除
		}

		//メッセージの表示
		int msg = (++time / 30);
		if (msg >= 4)
		{
			msg = 0;
			time = 0;
		}
		switch (msg)
		{
		case 0:	sanFont::print(10, 10, L"Now Loading ");    break;
		case 1:	sanFont::print(10, 10, L"Now Loading .");	break;
		case 2:	sanFont::print(10, 10, L"Now Loading ..");	break;
		case 3:	sanFont::print(10, 10, L"Now Loading ...");	break;
		}

	}
	else
	{	//読み込み完了後の通常execute
		sanFont::print(10, 10, L"Load Finish !");
	}

	//進捗状況表示
	sanFont::print(200, 10, L"progress : %d / %d", count, num);

	//読み込み終わったモデルから処理
	for (int i = 0; i < num; i++)
	{
		if (pModel[i] == NULL)continue;
		pModel[i]->addRotationY(XMConvertToRadians(3.0f));
	}

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	sanScene::execute();
}

//描画関数
void SceneAsyncTest::render()
{
	sanScene::render();
}

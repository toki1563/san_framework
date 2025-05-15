#pragma once

class SceneAsyncTest : public sanScene
{
public:
	//スレッドに送る引数の構造体
	struct ThreadArg
	{
		int num;
		WCHAR *path;
		sanModel** ppModel;
	};

private:
	//スレッドに送る引数の構造体
	ThreadArg arg;

	//クリティカルセクション
	static CRITICAL_SECTION cs;

	//モデルデータのパス
	WCHAR path[256][256];

	//読み込みモデル数
	int num;

	//読み込み完了モデル数
	static int count;

	//非同期読み込み関数
	static unsigned __stdcall loadAsync(void* lpx);

	//スレッドのハンドル
	HANDLE hTh;

	//読み込み完了検出用
	int pastCount;

	//モデル配列
	sanModel** pModel;

	//フレームカウンタ
	int time;

public:
	//初期化
	bool initialize();

	//終了
	void terminate();

	//処理
	virtual void execute();

	//描画
	virtual void render();
};

#pragma once

class SceneMainGame : public sanScene
{
private:
	player* pPlayer;
	boss* pBoss;
	sanModel* pGround;
	sanModel* pSky;
	sanSound* pBgm;
	gameUI gameUI;

	//モーションの数
	int playerMotionNum;
	//読み込んだモーションデータ
	sanMotionData** playerMotion;

	//モーションファイル読み込み関数
	sanMotionData* loadMotionFile(const WCHAR* motFile);

	//回転補正
	void rotRoll(sanMotionData* p);


	// 極座標の情報
	float radians;   // ラジアン
	float fallRange; // 落下距離
	float playerMoveSpeed; // プレイヤーの移動スピード
	float cameraDistance;  // カメラの距離
	float cameraHeight;    // カメラの高さ

public:
	//初期化
	bool initialize();
	//終了
	void terminate();
	//処理
	void execute();
	//描画
	void render();
};
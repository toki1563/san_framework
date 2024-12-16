#pragma once
#define FENCE_NUM (32)       // フェンスの数
#define FENCE_RADIUS (37.5f) // フェンスの半径

class SceneMainGame : public sanScene
{
private:
	player* pPlayer;
	boss* pBoss;
	sanModel* pGround;
	sanModel* pSky;
	sanSound* pBgm;

	// 極座標の情報
	float radians;   // ラジアン
	float fallRange; // 落下距離
	float playerMoveSpeed; // プレイヤーの移動スピード

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
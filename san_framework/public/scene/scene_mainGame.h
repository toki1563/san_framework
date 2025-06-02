#pragma once

class SceneMainGame : public sanScene
{
private:
	player* pPlayer;
	boss* pBoss;
	sanModel* pGround;
	sanModel* pFence;
	sanModel* pSky;
	sanSound* pBgm;
	gameUI* pGameUI;
	gameCamera* pGameCamera;

public:
	//初期化
	bool initialize();
	//終了
	void terminate();
	//処理
	void execute();
	//描画
	void render();

	static float slowRate; // スローの際の処理倍率
};
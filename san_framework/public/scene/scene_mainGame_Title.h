#pragma once

class SceneMainGameTitle : public sanScene
{
private:
	sanSprite* pSprite;
	sanSprite* pSprite2;

	float radius; // 座標設定
	float theta;
	float phi;

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
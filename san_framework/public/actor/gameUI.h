#pragma once

class gameUI
{
private:
	// プレイヤーの画像
	sanSprite* pPHpImg; // HPの画像
	sanSprite* pPStaminaImg; //スタミナの画像
	sanSprite* pPHpBackImg;  // HPの画像の背景
	sanSprite* pPStaminaBackImg; // スタミナの画像の背景
	// プレイヤーの画像の初期Xの位置
	float initPHpImgPosX; 
	float initPStaminaImgPosX;

	// ボスの画像
	sanSprite* pBHpImg; // HP画像
	sanSprite* pBHPBackImg; // HPの画像の背景
	float initBHpImgPosX;



public:
	bool initialize();
	void terminate();
	void execute(player* pPlayer, boss* pBoss);
	void render();
};
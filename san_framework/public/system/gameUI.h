#pragma once

// スタミナのUI量変化
enum staminaUI
{
	Large,  // 大
	Medium, // 中
	Small,  // 小
	StaminaKindMax, // スタミナの種類の上限
};

class gameUI
{
private:
	// プレイヤーの画像
	sanSprite* pPHpImg; // HPの画像
	sanSprite* pPHpDamageImg; // ダメージアニメの画像
	sanSprite* pPStaminaImg[3]; //スタミナの画像(大・中・小)量によって変更
	sanSprite* pPHpBackImg;  // HPの画像の背景
	sanSprite* pPStaminaBackImg; // スタミナの画像の背景
	sanSprite* pUIBackGround[2]; // UIの背景
	// プレイヤーの画像の初期Xの位置
	float initPHpImgPosX; 
	float initPStaminaImgPosX;
	float initStaminaImgWidth;

	staminaUI staminaUIState; // スタミナ状態

	// ボスの画像
	sanSprite* pBHpImg; // HP画像
	sanSprite* pBHpDamageImg; // ダメージアニメの画像
	sanSprite* pBHPBackImg; // HPの画像の背景
	float initBHpImgPosX;
	float minBHpImgPosX;



public:
	gameUI();
	~gameUI();

	void execute(player* pPlayer, boss* pBoss);
	void render();
};
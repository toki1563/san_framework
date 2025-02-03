#include "../../framework.h"
#include "../../framework/san_environment.h"
#include "gameUI.h"

// 初期化関数
bool gameUI::initialize()
{
	// 初期位置設定
	initPHpImgPosX = 225.0f;
	initPStaminaImgPosX = 185.0f;
	initBHpImgPosX = 1100.0f;

	// 画像設定
	pPHpImg = new sanSprite(initPHpImgPosX, 40.f, 450.0f, 24.0f, L"data/image/game/playerHpImg.png");
	pPStaminaImg = new sanSprite(initPStaminaImgPosX, 70.f, 370.0f, 24.0f, L"data/image/game/playerStaminaImg.png");
	pPHpBackImg = new sanSprite(220.f, 40.f, 450.0f, 24.0f, L"data/image/game/playerHpBackImg.png");
	pPStaminaBackImg = new sanSprite(180.f, 70.f, 370.0f, 24.0f, L"data/image/game/staminaBackImg.png");
	pBHpImg = new sanSprite(1100.f, 40.f, 370.0f, 24.0f, L"data/image/game/enemyHpImg.png");
	pBHPBackImg = new sanSprite(1100.f, 40.f, 370.0f, 24.0f, L"data/image/game/enemyHpBackImg.png");

	return true;
}

// 終了関数
void gameUI::terminate()
{
	delete pBHPBackImg;
	delete pBHpImg;
	delete pPStaminaBackImg;
	delete pPHpBackImg;
	delete pPStaminaImg;
	delete pPHpImg;
}			

// 処理関数
void gameUI::execute(player* pPlayer, boss* pBoss)
{
	// プレイヤーのＨＰ割合を出してinitPosXから-initPosXまで位置を動的に変更
	float hpRatio = pPlayer->status.health / pPlayer->status.maxHealth;
	float playerHpImgPosX = initPHpImgPosX * 2 * hpRatio - initPHpImgPosX;

	// 大きさは変えずに位置だけ左にずらす
	if (hpRatio <= 0.0f) // HPがなくなったの時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.1f) // HPが1割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.2f) // HPが2割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.3f) // HPが3割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.4f) // HPが4割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.5f) // HPが5割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.6f) // HPが6割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.7f) // HPが7割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.8f) // HPが8割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else if (hpRatio <= 0.9f) // Hが9割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
	}
	else // HPが満タンの時
	{
		pPHpImg->posX = playerHpImgPosX;
	}

	// ボスのＨＰ割合を出して    位置を動的に変更
	hpRatio = pBoss->status.health / pBoss->status.maxHealth;
	float bossHpImgPosX = initBHpImgPosX * 2 * hpRatio - initBHpImgPosX;
	// 大きさは変えずに位置だけ左にずらす
	if (hpRatio <= 0.0f) // HPがなくなったの時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.1f) // HPが1割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.2f) // HPが2割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.3f) // HPが3割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.4f) // HPが4割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.5f) // HPが5割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.6f) // HPが6割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.7f) // HPが7割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.8f) // HPが8割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else if (hpRatio <= 0.9f) // Hが9割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
	}
	else // HPが満タンの時
	{
		pBHpImg->posX = bossHpImgPosX;
	}



	// スタミナ割合を出してinitPosXから-initPosXまで位置を動的に変更
	float staminaRatio = pPlayer->status.stamina / pPlayer->status.maxStamina;
	float imgstaminaPosX = initPStaminaImgPosX * 2 * staminaRatio - initPStaminaImgPosX;

	// 大きさは変えずに位置だけ左にずらす
	if (staminaRatio <= 0.0f) // スタミナがなくなったの時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.1f) // スタミナが1割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.2f) // スタミナが2割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.3f) // スタミナが3割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.4f) // スタミナが4割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.5f) // スタミナが5割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.6f) // スタミナが6割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.7f) // スタミナが7割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.8f) // スタミナが8割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else if (staminaRatio <= 0.9f) // スタミナが9割以上の時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
	else // スタミナが満タンの時
	{
		pPStaminaImg->posX = imgstaminaPosX;
	}
}

// 描画関数
void gameUI::render()
{
	// ゲージの背景
	pPHpBackImg->render();
	pPStaminaBackImg->render();
	pBHPBackImg->render();

	// UI表示
	pPHpImg->render();
	pPStaminaImg->render();
	pBHpImg->render();
}
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool gameUI::initialize()
{
	// 初期位置設定
	initPHpImgPosX = 175.0f;
	initPStaminaImgPosX = 125.0f;
	initBHpImgPosX = 1120.0f;
	minBHpImgPosX = 1460.0;

	// 画像設定
	pPHpImg = new sanSprite(initPHpImgPosX, 60.f, 350.0f, 24.0f, L"data/image/game/playerHpImg.png");
	pPHpDamageImg = new sanSprite(initPHpImgPosX, 60.f, 350.0f, 24.0f, L"data/image/game/hpDamageImg.png");
	pPStaminaImg = new sanSprite(initPStaminaImgPosX, 90.f, 250.0f, 24.0f, L"data/image/game/playerStaminaImg.png");
	pPHpBackImg = new sanSprite(initPHpImgPosX, 60.f, 350.0f, 24.0f, L"data/image/game/hpBackImg.png");
	pPStaminaBackImg = new sanSprite(initPStaminaImgPosX, 90.f, 250.0f, 24.0f, L"data/image/game/hpBackImg.png");
	pBHpImg = new sanSprite(initBHpImgPosX, 75.0f, 350.0f, 30.0f, L"data/image/game/enemyHpImg.png");
	pBHpDamageImg = new sanSprite(initBHpImgPosX, 75.f, -350.0f, -30.0f, L"data/image/game/hpDamageImg.png"); // 反転させて使用
	pBHPBackImg = new sanSprite(initBHpImgPosX, 75.0f, -350.0f, -30.0f, L"data/image/game/hpBackImg.png");    // 同上
	pUIBackGround[0] = new sanSprite(initPHpImgPosX, 60.f, 380.0f, 100.0f, L"data/image/game/UIBackGround.png");
	pUIBackGround[1] = new sanSprite(initBHpImgPosX, 60.f, 380.0f, 100.0f, L"data/image/game/UIBackGround.png");
	// 画像透過処理
	pUIBackGround[0]->vtx[0].a = 0.5f;	pUIBackGround[0]->vtx[1].a = 0.5f;
	pUIBackGround[0]->vtx[2].a = 0.5f;	pUIBackGround[0]->vtx[3].a = 0.5f;
	pUIBackGround[1]->vtx[0].a = 0.5f;	pUIBackGround[1]->vtx[1].a = 0.5f;
	pUIBackGround[1]->vtx[2].a = 0.5f;	pUIBackGround[1]->vtx[3].a = 0.5f;

	return true;
}

// 終了関数
void gameUI::terminate()
{
	delete pUIBackGround[1];
	delete pUIBackGround[0];
	delete pBHPBackImg;
	delete pBHpDamageImg;
	delete pBHpImg;
	delete pPStaminaBackImg;
	delete pPHpBackImg;
	delete pPStaminaImg;
	delete pPHpDamageImg;
	delete pPHpImg;
}			

// 処理関数
void gameUI::execute(player* pPlayer, boss* pBoss)
{
	// HP割合に応じたプレイヤーのHPバーの位置計算
	float hpRatio = pPlayer->status.health / pPlayer->status.maxHealth;
	float playerHpImgPosX = initPHpImgPosX * 2 * hpRatio - initPHpImgPosX;

	// 大きさは変えずに位置だけ左にずらす
	if (hpRatio <= 0.0f) // HPがなくなったの時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.1f) // HPが1割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.2f) // HPが2割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.3f) // HPが3割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.4f) // HPが4割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.5f) // HPが5割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.6f) // HPが6割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.7f) // HPが7割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.8f) // HPが8割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else if (hpRatio <= 0.9f) // Hが9割以上の時
	{
		pPHpImg->posX = playerHpImgPosX;
		if (pPHpDamageImg->posX >= playerHpImgPosX)
		{
			pPHpDamageImg->posX -= 0.8f;
		}
	}
	else // HPが満タンの時
	{
		pPHpImg->posX = playerHpImgPosX;
		pPHpDamageImg->posX = playerHpImgPosX;
	}

	// HP割合に応じたボスのHPバーの位置計算
	hpRatio = pBoss->status.health / pBoss->status.maxHealth;
	float bossHpImgPosX = initBHpImgPosX + (minBHpImgPosX - initBHpImgPosX) * (1.0f - hpRatio);
	// 大きさは変えずに位置だけ左にずらす
	if (hpRatio <= 0.0f) // HPがなくなったの時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.1f) // HPが1割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.2f) // HPが2割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.3f) // HPが3割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.4f) // HPが4割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.5f) // HPが5割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.6f) // HPが6割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.7f) // HPが7割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.8f) // HPが8割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else if (hpRatio <= 0.9f) // Hが9割以上の時
	{
		pBHpImg->posX = bossHpImgPosX;
		if (pBHpDamageImg->posX <= bossHpImgPosX)
		{
			pBHpDamageImg->posX += 0.8f;
		}
	}
	else // HPが満タンの時
	{
		pBHpImg->posX = bossHpImgPosX;
		pBHpDamageImg->posX = bossHpImgPosX;
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

	// 名前表示
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 30));
	sanFont::print(10.0f, 8.0f, L"-Player-");
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 35));
	sanFont::print(970.0f, 15.0f, L"-Boss-");
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 16));

}

// 描画関数
void gameUI::render()
{
	pUIBackGround[0]->render();
	pUIBackGround[1]->render();

	// ゲージの背景
	pPHpBackImg->render();
	pPStaminaBackImg->render();
	pBHPBackImg->render();

	// UI表示
	pPHpDamageImg->render();
	pBHpDamageImg->render();
	pPHpImg->render();
	pBHpImg->render();
	pPStaminaImg->render();
}
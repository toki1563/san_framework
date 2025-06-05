#include "../../framework.h"
#include "../../framework/san_environment.h"

gameUI::gameUI()
{
	// 初期位置設定
	initPHpImgPosX = 175.0f;
	initPStaminaImgPosX = 640.0f;
	initStaminaImgWidth = 300.0f;
	initBHpImgPosX = 1120.0f;
	minBHpImgPosX = 1460.0;
	staminaUIState = staminaUI::Large;

	// 画像設定
	pPHpImg = new sanSprite(initPHpImgPosX, 75.f, 350.0f, 30.0f, L"data/image/game/playerHpImg.png");
	pPHpDamageImg = new sanSprite(initPHpImgPosX, 75.f, 350.0f, 30.0f, L"data/image/game/hpDamageImg.png");
	pPStaminaImg[Large] = new sanSprite(initPStaminaImgPosX, 680.f, initStaminaImgWidth, 20.0f, L"data/image/game/playerStaminaLargeImg.png");
	pPStaminaImg[Medium] = new sanSprite(initPStaminaImgPosX, 680.f, initStaminaImgWidth, 20.0f, L"data/image/game/playerStaminaMediumImg.png");
	pPStaminaImg[Small] = new sanSprite(initPStaminaImgPosX, 680.f, initStaminaImgWidth, 20.0f, L"data/image/game/playerStaminaSmallImg.png");
	pPHpBackImg = new sanSprite(initPHpImgPosX, 75.f, 350.0f, 30.0f, L"data/image/game/hpBackImg.png");
	pPStaminaBackImg = new sanSprite(initPStaminaImgPosX, 680.f, initStaminaImgWidth, 20.0f, L"data/image/game/StaminaBackImg.png");
	pBHpImg = new sanSprite(initBHpImgPosX, 75.0f, 350.0f, 30.0f, L"data/image/game/enemyHpImg.png");
	pBHpDamageImg = new sanSprite(initBHpImgPosX, 75.f, -350.0f, -30.0f, L"data/image/game/hpDamageImg.png"); // 反転させて使用
	pBHPBackImg = new sanSprite(initBHpImgPosX, 75.0f, -350.0f, -30.0f, L"data/image/game/hpBackImg.png");    // 同上
	pUIBackGround[0] = new sanSprite(initPHpImgPosX, 60.f, 380.0f, 90.0f, L"data/image/game/UIBackGround.png");
	pUIBackGround[1] = new sanSprite(initBHpImgPosX, 60.f, 380.0f, 90.0f, L"data/image/game/UIBackGround.png");

	// ポインタ初期化
	pPlayer = nullptr;
	pBoss = nullptr;

	// 画像透過処理
	pPStaminaBackImg->vtx[0].a = 0.5f;	pPStaminaBackImg->vtx[1].a = 0.5f;
	pUIBackGround[0]->vtx[0].a = 0.6f;	pUIBackGround[0]->vtx[1].a = 0.6f;
	pUIBackGround[0]->vtx[2].a = 0.6f;	pUIBackGround[0]->vtx[3].a = 0.6f;
	pUIBackGround[1]->vtx[0].a = 0.6f;	pUIBackGround[1]->vtx[1].a = 0.6f;
	pUIBackGround[1]->vtx[2].a = 0.6f;	pUIBackGround[1]->vtx[3].a = 0.6f;
}

gameUI::~gameUI()
{
	delete pUIBackGround[1];
	delete pUIBackGround[0];
	delete pBHPBackImg;
	delete pBHpDamageImg;
	delete pBHpImg;
	delete pPStaminaBackImg;
	delete pPHpBackImg;
	for (int i = 0; i < StaminaKindMax; i++)
	{
		delete pPStaminaImg[i];
	}
	delete pPHpDamageImg;
	delete pPHpImg;
}

// 処理関数
void gameUI::execute()
{
	if (pPlayer == nullptr || pBoss == nullptr) return; // 値がなければ処理しない

	// HP割合に応じたプレイヤーのHPバーの位置計算
	float hpRatio = pPlayer->status.health / pPlayer->status.maxHealth;
	float playerHpImgPosX = initPHpImgPosX * 2 * hpRatio - initPHpImgPosX;
	float hpDamageAnimSpeed = 0.4f; // HPのダメージアニメスピード

	// 大きさは変えずに位置だけ左にずらす
	// pPHpDamageImgはダメージアニメーション
	pPHpImg->posX = playerHpImgPosX;
	if (pPHpDamageImg->posX >= playerHpImgPosX)
	{
		pPHpDamageImg->posX -= hpDamageAnimSpeed;
	}

	// HP割合に応じたボスのHPバーの位置計算
	hpRatio = pBoss->status.health / pBoss->status.maxHealth;
	float bossHpImgPosX = initBHpImgPosX + (minBHpImgPosX - initBHpImgPosX) * (1.0f - hpRatio);

	// 大きさは変えずに位置だけ右にずらす
	// pBHpDamageImgはダメージアニメーション
	pBHpImg->posX = bossHpImgPosX;
	if (pBHpDamageImg->posX <= bossHpImgPosX)
	{
		pBHpDamageImg->posX += hpDamageAnimSpeed;
	}


	// スタミナの割合表示（0.0f～1.0f）
	float staminaRatio = pPlayer->status.stamina / pPlayer->status.maxStamina;

	// スタミナの割合によって現在のスタミナ状況を入れる
	if (staminaRatio > 0.6f)
	{
		staminaUIState = staminaUI::Large;
	}
	else if (staminaRatio > 0.3f)
	{
		staminaUIState = staminaUI::Medium;
	}
	else
	{
		staminaUIState = staminaUI::Small;
	}

	// 元の画像サイズと初期位置（中央表示と仮定）
	float fullWidth = initStaminaImgWidth;
	float centerX = initPStaminaImgPosX;

	for (int i = 0; i < StaminaKindMax; i++)
	{
		// スプライトの新しい横幅（スケール）
		pPStaminaImg[i]->scaleX = staminaRatio;

		// スプライトの新しいX座標（左に寄せる）
		pPStaminaImg[i]->posX = centerX - ((1.0f - staminaRatio) * fullWidth * 0.5f);
	}

	// 名前表示
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 30));
	sanFont::print(10.0f, 15.0f, L"-Player-");
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

	switch (staminaUIState)
	{
	case Large:
		pPStaminaImg[Large]->render();
		break;
	case Medium:
		pPStaminaImg[Medium]->render();
		break;
	case Small:
		pPStaminaImg[Small]->render();
		break;
	}
}

// キャラのポインタをセット
void gameUI::setCharaPtr(player* pPlayer, boss* pBoss)
{
	this->pPlayer = pPlayer;
	this->pBoss = pBoss;
}

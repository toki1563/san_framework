#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneMainGameTitle::initialize()
{
	// 画像の作成
	pTitleImg = new sanSprite(650.f, 520.f, 280.0f, 200.0f, L"data/image/game/select2.png");
	pSelectImg1 = new sanSprite(565.f, 485.f, 24.0f, 24.0f, L"data/image/game/select.png");
	pSelectImg2 = new sanSprite(565.f, 565.f, 24.0f, 24.0f, L"data/image/game/select.png");

	registerObject(pTitleImg);
	registerObject(pSelectImg1);
	registerObject(pSelectImg2);

	radius = 7.0f;
	theta = 0.0f;
	phi = 0.0f / 180.0f * 3.14;
	isGameStartSelect = true;

	// カメラ座標の計算
	// 回転してない基準となる軸ベクトル
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	// 回転マトリクスを作成
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);
	// 基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する)
	v = XMVector3TransformNormal(v, rotate);

	// XMVECTORの変数はx, y, z, wの情報が入っている
	XMVECTOR CamPos = v;

	// 少しカメラを上げる
	CamPos = XMVectorAdd(CamPos, XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f));

	// カメラ座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);


	return true;
}

//終了関数
void SceneMainGameTitle::terminate()
{
	deleteObject(pSelectImg2);
	deleteObject(pSelectImg1);
	deleteObject(pTitleImg);
}

//処理関数
void SceneMainGameTitle::execute()
{
	if (sanKeyboard::trg(DIK_UPARROW))
	{
		isGameStartSelect = true;
	}
	else if (sanKeyboard::trg(DIK_DOWN))
	{
		isGameStartSelect = false;
	}
	// ゲームスタートかどうか
	if (isGameStartSelect)
	{
		if (sanKeyboard::trg(DIK_SPACE) || sanKeyboard::trg(DIK_RETURN))
		{
			switchScene(MainGame);
		}
	}
	else
	{
		if (sanKeyboard::trg(DIK_SPACE) || sanKeyboard::trg(DIK_RETURN))
		{
			switchScene(Boot);
		}
	}

	sanScene::execute();
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 60));
	sanFont::print(370.f, 200.f, L"3Dアクションゲーム");
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 42));
	sanFont::print(600.f, 455.f, L"始める");
	sanFont::print(600.f, 535.f, L"終わる");
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 16));
}

//描画関数
void SceneMainGameTitle::render()
{
	pTitleImg->render();
	if (isGameStartSelect)
	{
		pSelectImg1->render();
	}
	else
	{
		pSelectImg2->render();
	}
}
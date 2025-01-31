#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneMainGameTitle::initialize()
{
	// スプライトの作成
	pSprite2 = new sanSprite(640.f, 520.f, 300.0f, 200.0f, L"data/image/game/select2.png");
	pSprite = new sanSprite(550.f, 480.f, 24.0f, 24.0f, L"data/image/game/select.png");

	registerObject(pSprite2);
	registerObject(pSprite);

	radius = 7.0f;
	theta = 0.0f;
	phi = 0.0f / 180.0f * 3.14;

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
	deleteObject(pSprite2);
	deleteObject(pSprite);
}

//処理関数
void SceneMainGameTitle::execute()
{
	sanScene::execute();
	sanFont::print(500.f, 200.f, L"3Dアクションゲーム");
}

//描画関数
void SceneMainGameTitle::render()
{
	sanScene::render();
}
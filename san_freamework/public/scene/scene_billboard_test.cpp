// ビルボード
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool SceneBillBoardTest::initialize()
{
	// ビルボードオブジェクト生成
	pBillboard = new sanBillboard(1.0f, 1.0f, L"data/image/icon_exc.png");
	pBillboard->setPositionY(0.9f);
	registerObject(pBillboard);

	radius = 7.0f;
	theta = 0.0f;
	phi = 0.0f / 180.0f * 3.14;
	value_r = 1.0f * 3.141592f / 180.0f;
	radiusRoll = 0.5f;
	scrollMove = 0.05f;
	gridActive = true;
	axisActive = true;
	isActiveBillboard = true;
	return true;
}

// 終了関数
void SceneBillBoardTest::terminate()
{
	deleteObject(pBillboard);
}

// 処理関数
void SceneBillBoardTest::execute()
{
	// カメラの回転
	if (sanMouse::getR() >= 0.1) // ﾏｳｽホイール値取得
	{
		radius += radiusRoll;
	}
	else if (sanMouse::getR() <= -0.1)
	{
		radius -= radiusRoll;
	}
	if (sanMouse::onR() && sanMouse::getDX() >= 10) // ｽｸﾛｰﾙ
	{
		theta += scrollMove;
	}
	if (sanMouse::onR() && sanMouse::getDX() <= -10)
	{
		theta -= scrollMove;
	}
	if (sanMouse::onR() && sanMouse::getDY() <= -10)
	{
		phi -= scrollMove;
	}
	if (sanMouse::onR() && sanMouse::getDY() >= 10)
	{
		phi += scrollMove;
	}
	if (sanKeyboard::trg(DIK_RETURN))
	{
		isActiveBillboard = !isActiveBillboard;
		pBillboard->setRenderEnable(isActiveBillboard);
	}
	if (sanKeyboard::trg(DIK_L)) // グリッド・軸の表示/非表示
	{
		gridActive = !gridActive;
		axisActive = !axisActive;
	}

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

	// デバッグ情報
	sanFont::print(20.0f, 40.0f, L"LキーでGrid&Axis表示・非表示");
	sanFont::print(20.0f, 60.0f, L"EnterキーでBillBoard表示・非表示");
	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridActive);
	sanDebugDraw::Axis(5.0f, axisActive);

	sanScene::execute();
}

// 描画関数
void SceneBillBoardTest::render()
{
	sanScene::render();
}
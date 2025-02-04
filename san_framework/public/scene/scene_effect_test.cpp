//--------------------------------------------------------------//
//	"san_effect_test.cpp"									    //
//	     エフェクトクラス						                	//
//													2024/12/12	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

#include "scene_effect_test.h"

//初期化関数
bool SceneEffectTest::initialize()
{
	sanEffect::stEffectDesc desc;
	pEmitter = new sanEffect(&desc);
	registerObject(pEmitter);
	cursor = 0;
	radius = 10.0f; // 座標設定
	theta = 10.0f;
	phi = -10.0f;
	radiusRoll = 0.5f;
	scrollMove = 0.05f;
	gridAxisActive = true;

	return true;
}

//終了関数
void SceneEffectTest::terminate()
{
	deleteObject(pEmitter);
}

//処理関数
void SceneEffectTest::execute()
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
		theta -= scrollMove;
	}
	if (sanMouse::onR() && sanMouse::getDX() <= -10)
	{
		theta += scrollMove;
	}
	if (sanMouse::onR() && sanMouse::getDY() <= -10)
	{
		phi += scrollMove;
	}
	if (sanMouse::onR() && sanMouse::getDY() >= 10)
	{
		phi -= scrollMove;
	}

	if (sanKeyboard::trg(DIK_L))
	{
		gridAxisActive = !gridAxisActive;
	}

	// 仰角(Φ)の計算
	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi) * radius;
	float pz = sin(theta) * cosf(phi) * radius;

	// カメラの座標
	sanCamera::setPosition(px, py, pz);

	sanFont::print(20.0f, 20.0f, L"LキーでGrid&Axis表示・非表示");
	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
	sanDebugDraw::Axis(5.0f, gridAxisActive);

	sanScene::execute();
}

//描画関数
void SceneEffectTest::render()
{
	sanScene::render();
}

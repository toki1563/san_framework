// キューブ
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool SceneCubeTest::initialize()
{
	pCube = new cCube();

	resetPos = XMVectorSet(pCube->getPositionX(), pCube->getPositionY(), pCube->getPositionZ(), 0.0f);
	resetRot = XMVectorSet(pCube->getRotationX(), pCube->getRotationY(), pCube->getRotationZ(), 0.0f);
	resetScl = XMVectorSet(pCube->getScaleX(), pCube->getScaleY(), pCube->getScaleZ(), 0.0f);

	radius  = 8.0f; // 座標設定
	theta   = 10.0f;
	phi     = -10.0f;
	value_t = 0.01f;
	value_r = 1.0f * 3.141592f / 180.0f;
	value_s = 0.01f;
	Cursor  = 0;
	radiusRoll = 0.5f; // ミドル回転時の値
	scrollMove = 0.05f; // 右クリック時のスクロール値
	gridActive = true;
	axisActive = true;

	return true;
}

// 終了関数
void SceneCubeTest::terminate()
{
	delete pCube;
}

// 処理関数
void SceneCubeTest::execute()
{
	if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = OperationMax - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
	{
		Cursor = 0;
	}
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
		gridActive = !gridActive;
		axisActive = !axisActive;
	}
	if (sanKeyboard::on(DIK_RIGHT))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->addPositionX(value_t);	break;
		case PositionY:	pCube->addPositionY(value_t);	break;
		case PositionZ:	pCube->addPositionZ(value_t);	break;
		case RotateX:	pCube->addRotationX(value_r);	break;
		case RotateY:	pCube->addRotationY(value_r);	break;
		case RotateZ:	pCube->addRotationZ(value_r);	break;
		case ScaleX:	pCube->addScaleX(value_s);		break;
		case ScaleY:	pCube->addScaleY(value_s);		break;
		case ScaleZ:	pCube->addScaleZ(value_s);		break;
		}
	}
	else if (sanKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->addPositionX(-value_t);	break;
		case PositionY:	pCube->addPositionY(-value_t);	break;
		case PositionZ:	pCube->addPositionZ(-value_t);	break;
		case RotateX:	pCube->addRotationX(-value_r);	break;
		case RotateY:	pCube->addRotationY(-value_r);	break;
		case RotateZ:	pCube->addRotationZ(-value_r);	break;
		case ScaleX:	pCube->addScaleX(-value_s);		break;
		case ScaleY:	pCube->addScaleY(-value_s);		break;
		case ScaleZ:	pCube->addScaleZ(-value_s);		break;
		}
	}
	else if (sanKeyboard::on(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->setPositionX(XMVectorGetX(resetPos));	break;
		case PositionY:	pCube->setPositionY(XMVectorGetY(resetPos));	break;
		case PositionZ:	pCube->setPositionZ(XMVectorGetZ(resetPos));	break;
		case RotateX:	pCube->setRotationX(XMVectorGetX(resetRot));	break;
		case RotateY:	pCube->setRotationY(XMVectorGetY(resetRot));	break;
		case RotateZ:	pCube->setRotationZ(XMVectorGetZ(resetRot));	break;
		case ScaleX:	pCube->setScaleX(XMVectorGetX(resetScl));		break;
		case ScaleY:	pCube->setScaleY(XMVectorGetY(resetScl));		break;
		case ScaleZ:	pCube->setScaleZ(XMVectorGetZ(resetScl));		break;
		}
	}

	// 仰角(Φ)の計算
	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi) * radius;
	float pz = sin(theta) * cosf(phi) * radius;

	// カメラの座標
	sanCamera::setPosition(px, py, pz);

	int line = 0;
	sanFont::print(0.0f, (float)(20 + Cursor * 16), L"→");
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"PositionX : %.3f", pCube->getPositionX());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"PositionY : %.3f", pCube->getPositionY());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"PositionZ : %.3f", pCube->getPositionZ());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"RotationX : %.3f", pCube->getRotationX() / 3.141592f * 180.0f);
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"RotationY : %.3f", pCube->getRotationY() / 3.141592f * 180.0f);
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"RotationZ : %.3f", pCube->getRotationZ() / 3.141592f * 180.0f);
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"ScaleX : %.3f", pCube->getScaleX());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"ScaleY : %.3f", pCube->getScaleY());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"ScaleZ : %.3f", pCube->getScaleZ());
	sanFont::print(20.0f, 170.0f, L"LキーでGrid&Axis表示・非表示");
	sanFont::print(20.0f, 170.0f, L"spaceキーでCubeリセット");

	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridActive);
	sanDebugDraw::Axis(5.0f, axisActive);
	sanScene::execute();
}

// 描画関数
void SceneCubeTest::render()
{
	pCube->render();
	sanScene::render();
}
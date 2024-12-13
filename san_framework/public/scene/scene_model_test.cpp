//--------------------------------------------------------------//
//	"san_model_test.cpp"  						    			//
//	     モデル													//
//													2024/11/25	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool SceneModelTest::initialize()
{
	pModel = new sanModel(L"data/model/primitive/", L"sphere.vnm"); // モデルにデータの情報を入れる
	registerObject(pModel);

	resetPos = XMVectorSet(pModel->getPositionX(), pModel->getPositionY(), pModel->getPositionZ(), 0.0f);
	resetRot = XMVectorSet(pModel->getRotationX(), pModel->getRotationY(), pModel->getRotationZ(), 0.0f);
	resetScl = XMVectorSet(pModel->getScaleX(), pModel->getScaleY(), pModel->getScaleZ(), 0.0f);
	resetCam = XMVectorSet(sanCamera::getPositionX(), sanCamera::getPositionY(), sanCamera::getPositionZ(), 0.0f);
	resetTag = XMVectorSet(sanCamera::getTargetX(), sanCamera::getTargetY(), sanCamera::getTargetZ(), 0.0f);

	radius = 5.0f; // 座標設定
	theta = 10.0f;
	phi = -10.0f;
	value_t = 0.01f;
	value_r = 1.0f * 3.141592f / 180.0f;
	value_s = 0.01f;
	radiusRoll = 0.5f; // ミドル回転時の値
	scrollMove = 0.05f; // 右クリック時のスクロール値
	gridActive = true;
	axisActive = true;

	Cursor = 0;

	return true;
}

// 終了関数
void SceneModelTest::terminate()
{
	// オブジェクトの削除
	deleteObject(pModel);
}

// 処理関数
void SceneModelTest::execute()
{
	if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = OperationMax - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
	{
		Cursor = 0;
	}
	else if (sanMouse::getR() >= 0.1) // ﾏｳｽホイール値取得
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
		case PositionX:	pModel->addPositionX(value_t);	break;
		case PositionY:	pModel->addPositionY(value_t);	break;
		case PositionZ:	pModel->addPositionZ(value_t);	break;
		case RotateX:	pModel->addRotationX(value_r);	break;
		case RotateY:	pModel->addRotationY(value_r);	break;
		case RotateZ:	pModel->addRotationZ(value_r);	break;
		case ScaleX:	pModel->addScaleX(value_s);		break;
		case ScaleY:	pModel->addScaleY(value_s);		break;
		case ScaleZ:	pModel->addScaleZ(value_s);		break;
		}
	}
	else if (sanKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pModel->addPositionX(-value_t);	break;
		case PositionY:	pModel->addPositionY(-value_t);	break;
		case PositionZ:	pModel->addPositionZ(-value_t);	break;
		case RotateX:	pModel->addRotationX(-value_r);	break;
		case RotateY:	pModel->addRotationY(-value_r);	break;
		case RotateZ:	pModel->addRotationZ(-value_r);	break;
		case ScaleX:	pModel->addScaleX(-value_s);	break;
		case ScaleY:	pModel->addScaleY(-value_s);	break;
		case ScaleZ:	pModel->addScaleZ(-value_s);	break;
		}
	}
	else if (sanKeyboard::on(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pModel->setPositionX(XMVectorGetX(resetPos));	break;
		case PositionY:	pModel->setPositionY(XMVectorGetY(resetPos));	break;
		case PositionZ:	pModel->setPositionZ(XMVectorGetZ(resetPos));	break;
		case RotateX:	pModel->setRotationX(XMVectorGetX(resetRot));	break;
		case RotateY:	pModel->setRotationY(XMVectorGetY(resetRot));	break;
		case RotateZ:	pModel->setRotationZ(XMVectorGetZ(resetRot));	break;
		case ScaleX:	pModel->setScaleX(XMVectorGetX(resetScl));		break;
		case ScaleY:	pModel->setScaleY(XMVectorGetY(resetScl));		break;
		case ScaleZ:	pModel->setScaleZ(XMVectorGetZ(resetScl));		break;
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
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"PositionX : %.3f", pModel->getPositionX());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"PositionY : %.3f", pModel->getPositionY());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"PositionZ : %.3f", pModel->getPositionZ());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"RotationX : %.3f", pModel->getRotationX() / 3.141592f * 180.0f);
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"RotationY : %.3f", pModel->getRotationY() / 3.141592f * 180.0f);
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"RotationZ : %.3f", pModel->getRotationZ() / 3.141592f * 180.0f);
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"ScaleX : %.3f", pModel->getScaleX());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"ScaleY : %.3f", pModel->getScaleY());
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"ScaleZ : %.3f", pModel->getScaleZ());
	sanFont::print(20.0f, 30.0f + (float)(line++ * 16), L"LキーでGrid&Axis表示・非表示");
	sanFont::print(20.0f, 30.0f + (float)(line++ * 16), L"spaceキーでリセット");

	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridActive);
	sanDebugDraw::Axis(5.0f, axisActive);

	sanScene::execute();

}

// 描画関数
void SceneModelTest::render()
{
	// キューブの描画
	pModel->render();

	sanScene::render();
}
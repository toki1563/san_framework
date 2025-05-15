#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneSkinTest::initialize()
{
	sanCamera::setTarget(0.0f, 2.0f, 0.0f);

	XMVECTOR dist = *sanCamera::getPosition() - *sanCamera::getTarget();
	float dX = XMVectorGetX(dist);
	float dY = XMVectorGetY(dist);
	float dZ = XMVectorGetZ(dist);
	
	radius = 10.0f; // 座標設定
	theta = 10.0f;
	phi = -10.0f;
	radiusRoll = 0.5f; // ミドル回転時の値
	scrollMove = 0.05f; // 右クリック時のスクロール値

	pCube = new cSkinnedCube();

	Cursor = 0;

	return true;
}

//終了関数
void SceneSkinTest::terminate()
{
	//オブジェクトの削除
	delete pCube;
}

//処理関数
void SceneSkinTest::execute()
{
	float value_t = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;
	float value_s = 0.01f;

	for (int i = 0; i < 9 && i<pCube->BoneNum; i++)
	{
		if (sanKeyboard::trg(DIK_1 + i))
		{
			Select = i;
			break;
		}
	}

	if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = OperationMax - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
	{
		Cursor = 0;
	}
	else if (sanKeyboard::on(DIK_RIGHT))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->bones[Select].Pos = XMVectorAdd(pCube->bones[Select].Pos, XMVectorSet(value_t, 0.0f, 0.0f, 0.0f));	break;
		case PositionY:	pCube->bones[Select].Pos = XMVectorAdd(pCube->bones[Select].Pos, XMVectorSet(0.0f, value_t, 0.0f, 0.0f));	break;
		case PositionZ:	pCube->bones[Select].Pos = XMVectorAdd(pCube->bones[Select].Pos, XMVectorSet(0.0f, 0.0f, value_t, 0.0f));	break;
		case RotateX:	pCube->bones[Select].Rot = XMVectorAdd(pCube->bones[Select].Rot, XMVectorSet(value_r, 0.0f, 0.0f, 0.0f));	break;
		case RotateY:	pCube->bones[Select].Rot = XMVectorAdd(pCube->bones[Select].Rot, XMVectorSet(0.0f, value_r, 0.0f, 0.0f));	break;
		case RotateZ:	pCube->bones[Select].Rot = XMVectorAdd(pCube->bones[Select].Rot, XMVectorSet(0.0f, 0.0f, value_r, 0.0f));	break;
		case ScaleX:	pCube->bones[Select].Scl = XMVectorAdd(pCube->bones[Select].Scl, XMVectorSet(value_s, 0.0f, 0.0f, 0.0f));	break;
		case ScaleY:	pCube->bones[Select].Scl = XMVectorAdd(pCube->bones[Select].Scl, XMVectorSet(0.0f, value_s, 0.0f, 0.0f));	break;
		case ScaleZ:	pCube->bones[Select].Scl = XMVectorAdd(pCube->bones[Select].Scl, XMVectorSet(0.0f, 0.0f, value_s, 0.0f));	break;
		}
	}
	else if (sanKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->bones[Select].Pos = XMVectorSubtract(pCube->bones[Select].Pos, XMVectorSet(value_t, 0.0f, 0.0f, 0.0f));	break;
		case PositionY:	pCube->bones[Select].Pos = XMVectorSubtract(pCube->bones[Select].Pos, XMVectorSet(0.0f, value_t, 0.0f, 0.0f));	break;
		case PositionZ:	pCube->bones[Select].Pos = XMVectorSubtract(pCube->bones[Select].Pos, XMVectorSet(0.0f, 0.0f, value_t, 0.0f));	break;
		case RotateX:	pCube->bones[Select].Rot = XMVectorSubtract(pCube->bones[Select].Rot, XMVectorSet(value_r, 0.0f, 0.0f, 0.0f));	break;
		case RotateY:	pCube->bones[Select].Rot = XMVectorSubtract(pCube->bones[Select].Rot, XMVectorSet(0.0f, value_r, 0.0f, 0.0f));	break;
		case RotateZ:	pCube->bones[Select].Rot = XMVectorSubtract(pCube->bones[Select].Rot, XMVectorSet(0.0f, 0.0f, value_r, 0.0f));	break;
		case ScaleX:	pCube->bones[Select].Scl = XMVectorSubtract(pCube->bones[Select].Scl, XMVectorSet(value_s, 0.0f, 0.0f, 0.0f));	break;
		case ScaleY:	pCube->bones[Select].Scl = XMVectorSubtract(pCube->bones[Select].Scl, XMVectorSet(0.0f, value_s, 0.0f, 0.0f));	break;
		case ScaleZ:	pCube->bones[Select].Scl = XMVectorSubtract(pCube->bones[Select].Scl, XMVectorSet(0.0f, 0.0f, value_s, 0.0f));	break;
		}
	}
	else if (sanKeyboard::trg(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->bones[Select].Pos = XMVectorSetX(pCube->bones[Select].Pos, XMVectorGetX(pCube->bones[Select].BindPos));	break;
		case PositionY:	pCube->bones[Select].Pos = XMVectorSetY(pCube->bones[Select].Pos, XMVectorGetY(pCube->bones[Select].BindPos));	break;
		case PositionZ:	pCube->bones[Select].Pos = XMVectorSetZ(pCube->bones[Select].Pos, XMVectorGetZ(pCube->bones[Select].BindPos));	break;
		case RotateX:	pCube->bones[Select].Rot = XMVectorSetX(pCube->bones[Select].Rot, XMVectorGetX(pCube->bones[Select].BindRot));	break;
		case RotateY:	pCube->bones[Select].Rot = XMVectorSetY(pCube->bones[Select].Rot, XMVectorGetY(pCube->bones[Select].BindRot));	break;
		case RotateZ:	pCube->bones[Select].Rot = XMVectorSetZ(pCube->bones[Select].Rot, XMVectorGetZ(pCube->bones[Select].BindRot));	break;
		case ScaleX:	pCube->bones[Select].Scl = XMVectorSetX(pCube->bones[Select].Scl, XMVectorGetX(pCube->bones[Select].BindScl));	break;
		case ScaleY:	pCube->bones[Select].Scl = XMVectorSetY(pCube->bones[Select].Scl, XMVectorGetY(pCube->bones[Select].BindScl));	break;
		case ScaleZ:	pCube->bones[Select].Scl = XMVectorSetZ(pCube->bones[Select].Scl, XMVectorGetZ(pCube->bones[Select].BindScl));	break;
		}
	}


	int line = 0;
	sanFont::print(80.0f, (float)(100 + (Cursor+1) * 16), 0xffffffff, L"→");
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"BoneID : %d", Select);
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"PositionX : %.3f", XMVectorGetX(pCube->bones[Select].Pos));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"PositionY : %.3f", XMVectorGetY(pCube->bones[Select].Pos));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"PositionZ : %.3f", XMVectorGetZ(pCube->bones[Select].Pos));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"RotationX : %.3f", XMConvertToDegrees(XMVectorGetX(pCube->bones[Select].Rot)));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"RotationY : %.3f", XMConvertToDegrees(XMVectorGetY(pCube->bones[Select].Rot)));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"RotationZ : %.3f", XMConvertToDegrees(XMVectorGetZ(pCube->bones[Select].Rot)));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"ScaleX : %.3f", XMVectorGetX(pCube->bones[Select].Scl));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"ScaleY : %.3f", XMVectorGetY(pCube->bones[Select].Scl));
	sanFont::print(100.0f, 100.0f + (float)(line++ * 16), L"ScaleZ : %.3f", XMVectorGetZ(pCube->bones[Select].Scl));
	sanFont::print(100.0f, 110.0f + (float)(line++ * 16), L"BoneIDの切り替えは 番号１or２で切り替え可能", Select);

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	operateCamera(); // カメラ処理
	sanScene::execute();
}

//描画関数
void SceneSkinTest::render()
{
	//キューブの描画
	pCube->render();

	sanScene::render();
}

// カメラ処理
void SceneSkinTest::operateCamera()
{
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

	// 仰角(Φ)の計算
	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi) * radius;
	float pz = sin(theta) * cosf(phi) * radius;

	// カメラの座標
	sanCamera::setPosition(px, py, pz);
}


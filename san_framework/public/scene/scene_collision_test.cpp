//--------------------------------------------------------------//
//	"san_collision_test.cpp"									//
//	     当たり判定												//
//													2024/11/22	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"
#include "scene_collision_test.h"

// 初期化関数
bool SceneCollisionTest::initialize()
{
	cursor = 0;
	radius = 10.0f;
	theta = 0.0f;
	phi = 0.0f / 180.0f * 3.14f;
	value_r = 1.0f * 3.141592f / 180.0f;
	radians = 180.0f / 3.141592f;
	radiusRoll = 0.5f;
	scrollMove = 0.05f;
	gridAxisActive = true;

	point[0] = XMVectorSet(-2.0f, 4.0f, -2.0f, 0.0f);
	point[1] = XMVectorSet(3.0f, -4.0f, 3.0f, 0.0f);
	point[2] = XMVectorSet(-1.0f, 0.0f, 1.0f, 0.0f);
	point[3] = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
	point[4] = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
	add[0] = XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
	add[1] = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
	add[2] = XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);

	pPoint = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);
	registerObject(pPoint);	// シーンの基底クラスへ登録

	return true;
}

// 終了関数
void SceneCollisionTest::terminate()
{
	deleteObject(pPoint);
}

// 処理関数
void SceneCollisionTest::execute()
{
	if (sanKeyboard::trg(DIK_UP) && --cursor < 0)
	{
		cursor = OperationMax - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++cursor >= OperationMax)
	{
		cursor = 0;
	}
	else if (sanKeyboard::on(DIK_RIGHT))
	{
		switch (cursor)
		{
		case LinePositionX:			point[0] = XMVectorAdd(point[0], add[0]);	break;
		case LinePositionY:			point[0] = XMVectorAdd(point[0], add[1]);	break;
		case LinePositionZ:			point[0] = XMVectorAdd(point[0], add[2]);	break;
		case Triangle1PositionX:	point[2] = XMVectorAdd(point[2], add[0]);	break;
		case Triangle1PositionY:	point[2] = XMVectorAdd(point[2], add[1]);	break;
		case Triangle1PositionZ:	point[2] = XMVectorAdd(point[2], add[2]);	break;
		case Triangle2PositionX:	point[3] = XMVectorAdd(point[3], add[0]);	break;
		case Triangle2PositionY:	point[3] = XMVectorAdd(point[3], add[1]);	break;
		case Triangle2PositionZ:	point[3] = XMVectorAdd(point[3], add[2]);	break;
		case Triangle3PositionX:	point[4] = XMVectorAdd(point[4], add[0]);	break;
		case Triangle3PositionY:	point[4] = XMVectorAdd(point[4], add[1]);	break;
		case Triangle3PositionZ:	point[4] = XMVectorAdd(point[4], add[2]);	break;
		}
	}
	else if (sanKeyboard::on(DIK_LEFT))
	{
		switch (cursor)
		{
		case LinePositionX:			point[0] = XMVectorSubtract(point[0], add[0]);	break;
		case LinePositionY:			point[0] = XMVectorSubtract(point[0], add[1]);	break;
		case LinePositionZ:			point[0] = XMVectorSubtract(point[0], add[2]);	break;
		case Triangle1PositionX:	point[2] = XMVectorSubtract(point[2], add[0]);	break;
		case Triangle1PositionY:	point[2] = XMVectorSubtract(point[2], add[1]);	break;
		case Triangle1PositionZ:	point[2] = XMVectorSubtract(point[2], add[2]);	break;
		case Triangle2PositionX:	point[3] = XMVectorSubtract(point[3], add[0]);	break;
		case Triangle2PositionY:	point[3] = XMVectorSubtract(point[3], add[1]);	break;
		case Triangle2PositionZ:	point[3] = XMVectorSubtract(point[3], add[2]);	break;
		case Triangle3PositionX:	point[4] = XMVectorSubtract(point[4], add[0]);	break;
		case Triangle3PositionY:	point[4] = XMVectorSubtract(point[4], add[1]);	break;
		case Triangle3PositionZ:	point[4] = XMVectorSubtract(point[4], add[2]);	break;
		}
	}
	else if (sanKeyboard::on(DIK_SPACE))
	{
		//　リセット機能
		point[0] = XMVectorSet(-2.0f, 4.0f, -2.0f, 0.0f);
		point[1] = XMVectorSet(3.0f, -4.0f, 3.0f, 0.0f);
		point[2] = XMVectorSet(-1.0f, 0.0f, 1.0f, 0.0f);
		point[3] = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
		point[4] = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
	}
	else if (sanKeyboard::trg(DIK_L))
	{
		gridAxisActive = !gridAxisActive;
	}

	// 直線の情報を作る
	linePos = point[0];
	lineDir = point[1] - point[0];
	lineDir = XMVector3Normalize(lineDir);

	// 平面情報を作る
	XMVECTOR v1, v2;
	// 方向ベクトルを作成
	v1 = point[3] - point[2];
	v2 = point[4] - point[2];
	planeNormal = XMVector3Cross(v1, v2);
	planeNormal = XMVector3Normalize(planeNormal);

	XMVECTOR vDot = XMVector3Dot(planeNormal, point[2]);
	planeDistance = -XMVectorGetX(vDot);

	// 直線と平面の衝突座標を求める
	float d, s;
	XMVECTOR vS = XMVector3Dot(planeNormal, linePos);
	s = XMVectorGetX(vS);
	s += planeDistance;

	XMVECTOR vD = XMVector3Dot(planeNormal, linePos + lineDir);
	d = XMVectorGetX(vD);
	d += planeDistance;

	XMVECTOR hit = linePos + lineDir * (s / (s - d));
	pPoint->setPosition(&hit);


	v1 = point[3] - point[2];
	v2 = hit - point[2];
	pointNormal = XMVector3Cross(v1, v2);
	vDot = XMVector3Dot(pointNormal, planeNormal);
	pointDots[0] = XMVectorGetX(vDot);

	v1 = point[4] - point[3];
	v2 = hit - point[3];
	pointNormal = XMVector3Cross(v1, v2);
	vDot = XMVector3Dot(pointNormal, planeNormal);
	pointDots[1] = XMVectorGetX(vDot);

	v1 = point[2] - point[4];
	v2 = hit - point[4];
	pointNormal = XMVector3Cross(v1, v2);
	vDot = XMVector3Dot(pointNormal, planeNormal);
	pointDots[2] = XMVectorGetX(vDot);

	if ((pointDots[0] >= 0 && pointDots[1] >= 0 && pointDots[2] >= 0) ||
		(pointDots[0] <= 0 && pointDots[1] <= 0 && pointDots[2] <= 0))
	{
		pPoint->setDiffuse(1, 0, 0, 1); // 赤にする
	}
	else
	{
		pPoint->setDiffuse(1, 1, 1, 1);	//白にする
	}

	operateCamera();
	sanScene::execute();
	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
	sanDebugDraw::Axis(5.0f, gridAxisActive);
	sanDebugDraw::Line(&point[0], &point[1], 0xffffff00);
	sanDebugDraw::Line(&point[2], &point[3], 0xffff00ff);
	sanDebugDraw::Line(&point[3], &point[4], 0xffff00ff);
	sanDebugDraw::Line(&point[4], &point[2], 0xffff00ff);

	int line = 0;
	sanFont::print(0.0f, (float)(20 + cursor * 16), L"→");
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"LinePositionX : %.3f", XMVectorGetX(point[0]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"LinePositionY : %.3f", XMVectorGetY(point[0]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"LinePositionZ : %.3f", XMVectorGetZ(point[0]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle1PositionX : %.3f", XMVectorGetX(point[2]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle1PositionY : %.3f", XMVectorGetY(point[2]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle1PositionZ : %.3f", XMVectorGetZ(point[2]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle2PositionX : %.3f", XMVectorGetX(point[3]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle2PositionY : %.3f", XMVectorGetY(point[3]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle2PositionZ : %.3f", XMVectorGetZ(point[3]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle3PositionX : %.3f", XMVectorGetX(point[4]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle3PositionY : %.3f", XMVectorGetY(point[4]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"Triangle3PositionZ : %.3f", XMVectorGetZ(point[4]));
	sanFont::print(20.0f, 30.0f + (float)(line++ * 16), L"LキーでGrid&Axis表示・非表示");
	sanFont::print(20.0f, 30.0f + (float)(line++ * 16), L"spaceキーでBillBoard表示・非表示");

}

// 描画関数
void SceneCollisionTest::render()
{
	sanScene::render();
}

// カメラ処理
void SceneCollisionTest::operateCamera()
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

	// カメラ座標の計算
	// 回転してない基準となる軸ベクトル
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	// 回転マトリクスを作成
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	// 基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する)
	v = XMVector3TransformNormal(v, rotate);

	// XMVECTORの変数はx,y,z,wの情報が入っている
	XMVECTOR CamPos = v;

	// 少しだけ上げる
	XMVECTOR CamBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos, CamBias);

	// カメラの座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);
}
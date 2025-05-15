//--------------------------------------------------------------//
//	"san_spherecollision_test.cpp"								//
//	     球の当たり判定											//
//													2024/11/24	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"
#include "scene_spherecollision_test.h"

// 初期化関数
bool SceneSphereCollisionTest::initialize()
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
	spherePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	add[0] = XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
	add[1] = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
	add[2] = XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);

	pPoint = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);
	pSphere = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pHitModel[0] = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pHitModel[0]->setScale(0.1f, 0.1f, 0.1f);
	pHitModel[1] = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pHitModel[1]->setScale(0.1f, 0.1f, 0.1f);

	registerObject(pPoint);	// シーンの基底クラスへ登録
	registerObject(pHitModel[0]);
	registerObject(pHitModel[1]);
	registerObject(pSphere);

	return true;
}

// 終了関数
void SceneSphereCollisionTest::terminate()
{
	deleteObject(pSphere);
	deleteObject(pHitModel[1]);
	deleteObject(pHitModel[0]);
	deleteObject(pPoint);
}

// 処理関数
void SceneSphereCollisionTest::execute()
{
	if (sanKeyboard::trg(DIK_UP) && --cursor < 0)
	{
		cursor = OperationMax - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++cursor >= OperationMax)
	{
		cursor = 0;
	}
	if (sanKeyboard::on(DIK_RIGHT))
	{
		switch (cursor)
		{
		case LinePositionX: point[0] = XMVectorAdd(point[0], add[0]);	break;
		case LinePositionY: point[0] = XMVectorAdd(point[0], add[1]);	break;
		case LinePositionZ: point[0] = XMVectorAdd(point[0], add[2]);	break;
		case SpherePositionX:	spherePos = XMVectorAdd(spherePos, add[0]);	break;
		case SpherePositionY:	spherePos = XMVectorAdd(spherePos, add[1]);	break;
		case SpherePositionZ:	spherePos = XMVectorAdd(spherePos, add[2]);	break;
		}
		pSphere->setPosition(&spherePos);
	}

	else if (sanKeyboard::on(DIK_LEFT))
	{
		switch (cursor)
		{
		case LinePositionX:	point[0] = XMVectorSubtract(point[0], add[0]);	break;
		case LinePositionY:	point[0] = XMVectorSubtract(point[0], add[1]);	break;
		case LinePositionZ:	point[0] = XMVectorSubtract(point[0], add[2]);	break;
		case SpherePositionX:	spherePos = XMVectorSubtract(spherePos, add[0]);	break;
		case SpherePositionY:	spherePos = XMVectorSubtract(spherePos, add[1]);	break;
		case SpherePositionZ:	spherePos = XMVectorSubtract(spherePos, add[2]);	break;
		}
		pSphere->setPosition(&spherePos);
	}
	else if (sanKeyboard::on(DIK_SPACE))
	{
		//　リセット機能
		point[0] = XMVectorSet(-2.0f, 4.0f, -2.0f, 0.0f);
		point[1] = XMVectorSet(3.0f, -4.0f, 3.0f, 0.0f);
		spherePos = XMVectorSet(-1.0f, 0.0f, 1.0f, 0.0f);
		pSphere->setPosition(&spherePos);
	}

	{
		// 直線の情報を作る
		linePos = point[0];
		lineDir = point[1] - point[0];
		lineDir = XMVector3Normalize(lineDir);

		// 球の情報
		sphereCenter = spherePos;
		sphereRadius = 1.0f;

		// 最近傍距離を示す直線上の1点を求める
		XMVECTOR v = sphereCenter - linePos;
		XMVECTOR vDot = XMVector3Dot(v, lineDir);
		float dot = XMVectorGetX(vDot);

		XMVECTOR closestPosition = linePos + lineDir * dot;
		pPoint->setPosition(&closestPosition);

		sanDebugDraw::Line(&closestPosition, &spherePos, 0xffffff00);

		XMVECTOR vDist = sphereCenter - closestPosition;
		vDist = XMVector3Length(vDist);
		float dist = XMVectorGetX(vDist);
		sanFont::print(20.0f, 120.0f, L"直線と中心の距離 : %.3f", dist);

		if (dist <= sphereRadius)
		{
			sanFont::print(20.0f, 140.0f, L"当たっている");
		}
		else
		{
			sanFont::print(20.0f, 140.0f, L"当たっていない");
		}
	}

	// 直線と球の衝突座標を求める
	{
		int n = 0; // 衝突座標の数
		XMVECTOR hit[2]; // 衝突座標
		XMVECTOR v = linePos - sphereCenter;
		XMVECTOR nv = XMVector3Normalize(v);
		XMVECTOR vdt = XMVector3Dot(nv, lineDir);
		XMVECTOR vdl = XMVector3Length(v);
		float dt = XMVectorGetX(vdt);
		float dl = XMVectorGetX(vdl);
		float dr = dl * sqrtf(1.0f - dt * dt); // 球中心と直線の距離
		if (dr <= sphereRadius)
		{
			// 球中心と直線の距離が半径以内なら衝突
			float dq = sqrtf(sphereRadius * sphereRadius - dr * dr);
			float ds = -dl * dt;
			float dp = ds + dq;
			float dm = ds - dq;

			// hitの中には0と1に値が入り、nは最終的に2になるので2つ入っているという情報を得る
			hit[n++] = linePos + lineDir * dm;
			if (dq != 0.0f)
			{
				// 交差(衝突座標が)2つ以上の場合
				hit[n++] = linePos + lineDir * dp;
			}
		}

		// デバッグ表示
		for (int i = 0; i < 2; i++)
		{
			if (i < n)
			{
				pHitModel[i]->setPosition(&hit[i]);
				pHitModel[i]->setRenderEnable(true);
				pPoint->setRenderEnable(false);
			}
			else
			{
				pHitModel[i]->setRenderEnable(false);
				pPoint->setRenderEnable(true);
			}
		}
	}

	operateCamera(); // カメラ処理
	sanScene::execute();
	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
	sanDebugDraw::Axis(5.0f, gridAxisActive);
	sanDebugDraw::Line(&point[0], &point[1], 0xffffff00);

	int line = 0;
	sanFont::print(0.0f, (float)(20.0f + cursor * 16), L"→");
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"LinePositionX : %.3f", XMVectorGetX(point[0]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"LinePositionY : %.3f", XMVectorGetY(point[0]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"LinePositionZ : %.3f", XMVectorGetZ(point[0]));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"SpherePositionX : %.3f", XMVectorGetX(spherePos));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"SpherePositionY : %.3f", XMVectorGetY(spherePos));
	sanFont::print(20.0f, 20.0f + (float)(line++ * 16), L"SpherePositionZ : %.3f", XMVectorGetZ(spherePos));
	sanFont::print(20.0f, 170.0f, L"LキーでGrid&Axis表示・非表示");
	sanFont::print(20.0f, 190.0f, L"spaceキーでリセット");}

// 描画関数
void SceneSphereCollisionTest::render()
{
	sanScene::render();
}

// カメラ処理
void SceneSphereCollisionTest::operateCamera()
{
	// カメラの回転
	if (sanMouse::getR() >= 0.1) // マウスホイール値取得
	{
		radius += radiusRoll;
	}
	else if (sanMouse::getR() <= -0.1)
	{
		radius -= radiusRoll;
	}
	if (sanMouse::onR() && sanMouse::getDX() >= 10) // スクロール
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
	if (sanKeyboard::trg(DIK_L))
	{
		gridAxisActive = !gridAxisActive;
	}

	// カメラ座標の計算
	// 回転してない基準となる軸ベクトル
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	// 回転マトリクスを作成
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	// 基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する)
	v = XMVector3TransformNormal(v, rotate);

	XMVECTOR CamPos = v;

	// 少しだけ上げる
	XMVECTOR CamBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos, CamBias);

	// カメラの座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);
}
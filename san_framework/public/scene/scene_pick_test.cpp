//--------------------------------------------------------------//
//	"san_pick_test.cpp"  						    			//
//	     当たり判定												//
//													2024/11/25	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool ScenePickTest::initialize()
{
	radius = 10.0f; // 座標設定
	theta = 0.0f;// -90.0f / 180.0f * 3.14f;
	phi = 30.0f / 180.0f * 3.14f;
	value_r = 1.0f * 3.141592f / 180.0f;
	radians = 180.0f / 3.141592f;
	radiusRoll = 0.5f;
	scrollMove = 0.05f;

	// 球の生成と設定
	for (int i = 0; i < SPHERE_NUM; i++)
	{
		pSphere[i] = new sanModel(L"data/model/primitive/", L"sphere.vnm");
		registerObject(pSphere[i]);
		// 最後の一つを真ん中に設定
		if (i < SPHERE_NUM - 1)
		{
			// 角度
			float degree = (float)i * 360.0f / (float)6.0f;
			float radian = degree * 3.141592f / 180.0f;
			// 座標の計算(極座標⇒直交座標)
			float x = cosf(radian) * 3.0f;
			float z = sinf(radian) * 3.0f;
			// オブジェクトへの設定
			pSphere[i]->setPosition(z, 0.0f, x);
		}
		else
		{
			pSphere[i]->setPosition(0.0f, 0.0f, 0.0f);
		}
	}
	return true;
}

// 終了関数
void ScenePickTest::terminate()
{
	for (int i = 0; i < 3; i++)
	{
		deleteObject(pSphere[i]);
	}
}

// 処理関数
void ScenePickTest::execute()
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
	XMVECTOR CamPos = v;//XMVectorSet(px, py, pz, 0.0f);

	// 少しだけ上げる
	XMVECTOR CamBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos, CamBias);

	// カメラの座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);

	if (sanMouse::on())
	{
		// ビューポート座標が中心が原点で(-1~0~1)マウス座標が左上が原点で(1280,720)
		// マウス座標(スクリーン座標)→ビューポート座標
		float viewport_x = (float)sanMouse::getX() /
			(float)sanMainFrame::screenWidth * 2.0f - 1.0f;
		float viewport_y = -(float)sanMouse::getY() /
			(float)sanMainFrame::screenHeight * 2.0f + 1.0f;
		sanFont::print(10.0f, 30.0f, L"ビューポート(マウス)座標(%.3f, %.3f)", viewport_x, viewport_y);

		// マウス座標(ビューポート座標)をFarプレーンに置く(z = 1, w = 1)
		XMVECTOR v = XMVectorSet(viewport_x, viewport_y, 1.0f, 1.0f);

		// ビューポート座標→ワールド座標
		XMMATRIX iVP = *sanCamera::getScreen();
		// 逆行列を計算
		iVP = XMMatrixInverse(NULL, iVP);
		// Determinat : 行列式(数値)
		// 一つ目の引数にアドレスを入れて逆行列が計算できたかの確認ができる(エラーチェック)
		// エラーの時は0が入る
		// 変換後の体積比
		XMVECTOR wld = XMVector3Transform(v, iVP);

		// w除算(w成分を1にする)
		XMVECTOR w = XMVectorSplatW(wld); // xyzw -> wwww
		wld = XMVectorDivide(wld, w); // x=x/w, y=y/w, z=z/w, w=w/w
		sanFont::print(10.0f, 50.0f, L"ワールド座標(%.3f, %.3f, %.3f, %.3f)",
			XMVectorGetX(wld),
			XMVectorGetY(wld),
			XMVectorGetZ(wld),
			XMVectorGetW(wld));

		// 直線の情報を作る(2点から直線情報の作成)
		linePos = *sanCamera::getPosition();
		lineDir = wld - linePos;
		lineDir = XMVector3Normalize(lineDir);

		int hitClosestSphereIndex = -1;      // 当たった球番号保管用
		float hitClosestDistance = D3D12_FLOAT32_MAX;  // 当たった球の距離保管用

		for (int i = 0; i < SPHERE_NUM; i++)
		{
			sphereCenter = *pSphere[i]->getPosition(); // 0を入れる
			sphereRadius = 1.0f;

			// カメラ位置と球の中心のベクトル
			XMVECTOR v = linePos - sphereCenter;
			XMVECTOR nv = XMVector3Normalize(v);
			// カメラから球への方向ベクトルとの内積を計算
			XMVECTOR vdt = XMVector3Dot(nv, lineDir);
			XMVECTOR vdl = XMVector3Length(v);
			// カメラから球中心までの距離
			float dt = XMVectorGetX(vdt);
			float dl = XMVectorGetX(vdl);
			// 球中心と直線の距離
			float dr = dl * sqrtf(1.0f - dt * dt);

			// 球に当たっているかつ前の情報より距離が近い場合は更新
			if (dr <= sphereRadius && dl < hitClosestDistance)
			{
				hitClosestSphereIndex = i;
				hitClosestDistance = dl;
			}
		}
		for (int i = 0; i < SPHERE_NUM; i++)
		{
			if (i == hitClosestSphereIndex)
			{
				pSphere[i]->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				pSphere[i]->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		sanFont::print(10.0f, 10.0f, L"マウス座標(%d, %d)", sanMouse::getX(), sanMouse::getY());

		sanScene::execute();
	}
	else
	{
		for (int i = 0; i < SPHERE_NUM; i++)
		{
			pSphere[i]->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}
}

// 描画関数
void ScenePickTest::render()
{
	sanScene::render();
}
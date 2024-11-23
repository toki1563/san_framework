//--------------------------------------------------------------//
//	"san_ground_test.cpp"  									    //
//	     グラウンド												//
//													2024/11/23	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool SceneGroundTest::initialize()
{
	pPlayer = new sanModel(L"data/model/bear/", L"bear.vnm");
	//pGround = new sanModel(L"data/model/", L"ground.vnm");
	//pGround = new sanModel(L"data/model/", L"noised_ground.vnm");
	//pGround = new sanModel(L"data/model/", L"overpass.vnm");
	pGround = new sanModel(L"data/model/", L"overpass_000.vnm");
	pSky = new sanModel(L"data/model/", L"skydome.vnm");
	pSky->setLighting(false); // ライティング無効

	// シーンの基底クラスへの登録
	registerObject(pPlayer);
	registerObject(pGround);
	registerObject(pSky);

	pPlayer->setPosition(0.0f, 0.0f, 0.0f);
	pGround->addScaleX(2.0f);
	pGround->addScaleY(2.0f);

	radius = 10.0f; 
	theta = 0.0f;
	phi = 0.0f / 180.0f * 3.14f;
	value_r = 1.0f * 3.141592f / 180.0f;
	moveSpeed = 0.3f;
	radians = 180.0f / 3.141592f;
	radiusRoll = 0.5f;
	scrollMove = 0.05f;

	velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	gravity = XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f); 

	gridActive = true;
	axisActive = true;
	input = false;

	return true;
}

// 終了関数
void SceneGroundTest::terminate()
{
	// 基底クラスの登録から削除
	deleteObject(pPlayer);
	deleteObject(pGround);
	deleteObject(pSky);
}

// 処理関数
void SceneGroundTest::execute()
{
	input = false; // 入力されていないときはfalse

	// 移動ベクトル
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// 移動
		// 移動
	if (sanKeyboard::on(DIK_W))
	{
		vMove = XMVectorSetZ(vMove, 1.0f);
		input = true;
	}
	if (sanKeyboard::on(DIK_S))
	{
		vMove = XMVectorSetZ(vMove, -1.0f);
		input = true;
	}
	if (sanKeyboard::on(DIK_A))
	{
		vMove = XMVectorSetX(vMove, -1.0f);
		input = true;
	}
	if (sanKeyboard::on(DIK_D))
	{
		vMove = XMVectorSetX(vMove, 1.0f);
		input = true;
	}
	// 移動ベクトルにスピードを適用(長さを変える)
	vMove = XMVectorScale(vMove, moveSpeed);

	// 視線ベクトルを計算
	XMVECTOR cam_pos = *sanCamera::getPosition();
	XMVECTOR cam_trg = *sanCamera::getTarget();
	XMVECTOR eye = XMVectorSubtract(cam_trg, cam_pos); // 引き算

	// Y軸の回転値を計算
	float cam_rotY = atan2f(XMVectorGetX(eye), XMVectorGetZ(eye));

	// カメラのY軸周り(theta)の回転値
	XMMATRIX mRotY = XMMatrixRotationY(cam_rotY); /*theta*/
	// 移動ベクトルを回転させる
	vMove = XMVector3TransformNormal(vMove, mRotY);

	// 入力があるときのみ
	// プレイヤーを移動方向に向ける処理
	if (input)
	{
		// 進行方向(方向ベクトル : vMove)に向ける処理
		float rotY = atan2f(XMVectorGetX(vMove), XMVectorGetZ(vMove));
		// 元の角度
		float srcY = pPlayer->getRotationY();
		// 先の角度
		float dstY = rotY;
		// オイラー角からマトリクスに変換
		XMMATRIX mSrc = XMMatrixRotationRollPitchYaw(0.0f, srcY, 0.0f);
		XMMATRIX mDst = XMMatrixRotationRollPitchYaw(0.0f, dstY, 0.0f);
		// マトリクスからクオータニオンに変換
		XMVECTOR qScr = XMQuaternionRotationMatrix(mSrc);
		XMVECTOR qDst = XMQuaternionRotationMatrix(mDst);
		// 球面線形補間で中間の姿勢を計算(第3引数でターゲットに向く強度を変更可能)
		XMVECTOR qAns = XMQuaternionSlerp(qScr, qDst, 0.1f);
		// マトリクスに変換
		XMMATRIX mAns = XMMatrixRotationQuaternion(qAns);
		// マトリクスからY軸回転値を計算(Y軸回転のみの場合)
		rotY = atan2f(-XMVectorGetZ(mAns.r[0]), XMVectorGetZ(mAns.r[2]));

		pPlayer->setRotationY(rotY);
	}

	// 移動ベクトルをプレイヤーの座標に加算
	pPlayer->addPosition(&vMove);

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
	if (sanKeyboard::trg(DIK_L))
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

	XMVECTOR CamPos = v;

	// カメラ座標にプレイヤーの座標を加算
	CamPos = XMVectorAdd(CamPos, *pPlayer->getPosition());

	// 少しだけカメラを上げる
	XMVECTOR CamBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos, CamBias);

	// カメラの座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);

	// カメラの注視点をプレイヤーの座標に設定
	XMVECTOR camTrg = *pPlayer->getPosition();
	camTrg = XMVectorAdd(camTrg, CamBias);
	sanCamera::setTarget(&camTrg);

	// モデルデータから三角形を取得
	int vnum = pGround->getVertexNum();
	int inum = pGround->getIndexNum(); 

	sanVertex3D* vtx = pGround->getVertex();   // 頂点配列
	unsigned short* idx = pGround->getIndex(); // インデックス配列
	XMMATRIX world = *pGround->getWorld();	   // ワールドマトリックス

	// 直線の情報を作る
	// 自分の少し上からLineを飛ばす
	linePos = *pPlayer->getPosition();
	linePos += XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f);
	lineDir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	for (int i = 0; i < inum; i += 3)
	{
		// 三角形の頂点座標
		XMVECTOR v1 = XMVectorSet
		(
			vtx[idx[i + 0]].x,
			vtx[idx[i + 0]].y,
			vtx[idx[i + 0]].z,
			0.0f
		);
		XMVECTOR v2 = XMVectorSet
		(
			vtx[idx[i + 1]].x,
			vtx[idx[i + 1]].y,
			vtx[idx[i + 1]].z,
			0.0f
		);
		XMVECTOR v3 = XMVectorSet
		(
			vtx[idx[i + 2]].x,
			vtx[idx[i + 2]].y,
			vtx[idx[i + 2]].z,
			0.0f
		);

		v1 = XMVector3TransformCoord(v1, world);
		v2 = XMVector3TransformCoord(v2, world);
		v3 = XMVector3TransformCoord(v3, world);

		// 平面情報を作る
		XMVECTOR vV1, vV2;
		// 方向ベクトルを作成
		vV1 = v2 - v1;
		vV2 = v3 - v1;
		planeNormal = XMVector3Cross(vV1, vV2);
		planeNormal = XMVector3Normalize(planeNormal);

		XMVECTOR vDot = XMVector3Dot(planeNormal, v1);
		planeDistance = -XMVectorGetX(vDot);


		// 直線と平面が平行か調べる(直線と平面の法線は垂直)
		float w = XMVectorGetX(XMVector3Dot(lineDir, planeNormal));
		if (w != 0.0f)
		{
			// 直線と平面の衝突座標を求める
			float d, s;
			XMVECTOR vS = XMVector3Dot(planeNormal, linePos);
			s = XMVectorGetX(vS);
			s += planeDistance;

			XMVECTOR vD = XMVector3Dot(planeNormal, linePos + lineDir);
			d = XMVectorGetX(vD);
			d += planeDistance;

			if ((s / (s - d)) < 0.0f) continue;

			// レイの長さを調べて地面が足元にあるなら処理する
			if (1.0f >= (s / (s - d)))
			{
				XMVECTOR hit = linePos + lineDir * (s / (s - d));
				vV1 = v2 - v1;
				vV2 = hit - v1;
				pointNormal = XMVector3Cross(vV1, vV2);
				vDot = XMVector3Dot(pointNormal, planeNormal);
				pointDots[0] = XMVectorGetX(vDot);

				vV1 = v3 - v2;
				vV2 = hit - v2;
				pointNormal = XMVector3Cross(vV1, vV2);
				vDot = XMVector3Dot(pointNormal, planeNormal);
				pointDots[1] = XMVectorGetX(vDot);

				vV1 = v1 - v3;
				vV2 = hit - v3;
				pointNormal = XMVector3Cross(vV1, vV2);
				vDot = XMVector3Dot(pointNormal, planeNormal);
				pointDots[2] = XMVectorGetX(vDot);

				if ((pointDots[0] >= 0 && pointDots[1] >= 0 && pointDots[2] >= 0) ||
					(pointDots[0] <= 0 && pointDots[1] <= 0 && pointDots[2] <= 0))
				{
					float hitY = XMVectorGetY(hit);
					if (isnan(hitY) == true)
					{
						assert(false);
						// エラー対策
						hitY = 0.0f;
					}

					pPlayer->setPositionY(XMVectorGetY(hit));
					// 三角形の辺をデバッグ描画
					sanDebugDraw::Line(&v1, &v2, 0xffff0000);
					sanDebugDraw::Line(&v2, &v3, 0xffff0000);
					sanDebugDraw::Line(&v3, &v1, 0xffff0000);
					// モデルから真下へのhit線表示
					sanDebugDraw::Line(&linePos, &hit, 0xff00ff00);
					velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 初期化
				}
				else
				{
					// 三角形の辺をデバッグ描画
					sanDebugDraw::Line(&v1, &v2, 0xffff0000);
					sanDebugDraw::Line(&v2, &v3, 0xffff0000);
					sanDebugDraw::Line(&v3, &v1, 0xffff0000);
				}
				sanScene::execute();
			}

		}// end of if(w w!=0)
	}

	velocity += gravity;
	pPlayer->addPosition(&velocity);

	// ある程度落ちたらリセット
	if (pPlayer->getPositionY() < -30.0f)
	{
		pPlayer->setPosition(0.0f, 0.0f, 0.0f);
		velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 初期化
	}

	// デバッグ情報
	sanFont::print(20.0f, 20.0f,
		L"Player : (%.3f, %.3f, %.3f)",
		pPlayer->getPositionX(),
		pPlayer->getPositionY(),
		pPlayer->getPositionZ());
	sanFont::print(20.0f, 40.0f, L"Player RotationY : %.3f", pPlayer->getRotationY()* radians);
	sanFont::print(20.0f, 60.0f, L"カメラのワールド座標");
	sanFont::print(20.0f, 80.0f, L"Radius : %.3f", radius);
	sanFont::print(20.0f, 100.0f, L"Theta : %.3f", theta* radians);
	sanFont::print(20.0f, 120.0f, L"Phi : %.3f", phi* radians);
	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridActive);
	sanDebugDraw::Axis(5.0f, axisActive);
}

// 描画関数
void SceneGroundTest::render()
{
	sanScene::render();
}
// ゲームテスト
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool SceneGameTest::initialize()
{
	pPlayer = new sanModel(L"data/model/bear/", L"bear.vnm");
	pNPC = new sanModel(L"data/model/bear/", L"bear.vnm");
	pNPC->setPositionX(2.0f);
	pDebugFan = new sanModel(L"data/model/", L"debug_fan.vnm");
	pDebugFan->setTransparent(true); // 半透明有無
	pDebugFan->setParent(pNPC); // 親子関係設定
	pDebugFan->setRenderEnable(false);
	pGround = new sanModel(L"data/model/", L"ground.vnm");
	pSky = new sanModel(L"data/model/", L"skydome.vnm");
	pSky->setLighting(false); // ライティング無効
	pCrown = new sanModel(L"data/model/", L"crown.vnm");
	pCrown->setPositionY(2.8f);
	pCrown->setScale(0.3f, 0.3f, 0.3f);
	pCrown->setParent(pPlayer);
	pBillboard = new sanBillboard(1.0f, 1.0f, L"data/image/icon_exc.png");
	pBillboard->setParent(pNPC);
	pBillboard->setPositionY(3.6f);
	// エフェクト
	sanEffect::stEffectDesc desc;
	pEffect = new sanEffect(&desc);
	pEffect->setParent(pPlayer);
	pEffectNPC = new sanEffect(&desc);
	pEffectNPC->setParent(pNPC);
	pHitEffect = new sanEffect(&desc);
	pHitEffect->setEmit(false);

	// シーンの基底クラスへ登録
	registerObject(pPlayer);
	registerObject(pNPC);
	registerObject(pGround);
	registerObject(pSky);
	registerObject(pCrown);
	registerObject(pBillboard);
	registerObject(pEffect);
	registerObject(pEffectNPC);
	registerObject(pHitEffect);


	for (int i = 0; i < FENCE_NUM; i++)
	{
		pFence[i] = new sanModel(L"data/model/", L"fence.vnm");
		registerObject(pFence[i]);

		// 角度
		float degree = (float)i * 360.0f / (float)FENCE_NUM;
		float radian = degree * 3.141592f / 180.0f;
		// 座標の計算(極座標⇒直交座標)
		float x = cosf(radian) * FENCE_RADIUS;
		float z = sinf(radian) * FENCE_RADIUS;
		// オブジェクトへの設定
		pFence[i]->setPosition(z, 0.0f, x);
		pFence[i]->setRotationY(radian);
	}

	registerObject(pDebugFan);

	radius = 10.0f; // 座標設定
	theta = 0.0f;// -90.0f / 180.0f * 3.14f;
	phi = 0.0f / 180.0f * 3.14f;
	value_r = 1.0f * 3.141592f / 180.0f;
	moveSpeed = 0.3f;
	radians = 180.0f / 3.141592f;
	scrollMove = 0.05f;
	shapeNumber = 0; // 形切り替えの数字

	gridAxisActive = true;
	isLightColorNormal = true;
	input = false;

	return true;
}

// 終了関数
void SceneGameTest::terminate()
{
	// 基底クラスの登録から削除
	for (int i = 0; i < FENCE_NUM; i++)
	{
		deleteObject(pFence[i]);
	}
	deleteObject(pHitEffect);
	deleteObject(pEffectNPC);
	deleteObject(pEffect);
	deleteObject(pBillboard);
	deleteObject(pCrown);
	deleteObject(pSky);
	deleteObject(pGround);
	deleteObject(pDebugFan);
	deleteObject(pNPC);
	deleteObject(pPlayer);
}

// 処理関数
void SceneGameTest::execute()
{
	input = false; // 入力していない場合はfalse

	// 移動ベクトル
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

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
	// エフェクト
	if (sanKeyboard::trg(DIK_SPACE))
	{
		pHitEffect->setPosition(pPlayer->getPosition());
		pHitEffect->addPositionY(3.5f);
		pHitEffect->setEmit(true);
		pHitEffect->setEmitCount(10.0f);
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
		// マトリクスからY軸回転値を計算(Y軸回転のみ場合)
		rotY =
			atan2f(-XMVectorGetZ(mAns.r[0]), XMVectorGetZ(mAns.r[2]));

		pPlayer->setRotationY(rotY);
		// 移動時にエフェクトを表示
		pEffect->setEmit(true);
	}
	else
	{
		// 停止時にエフェクトを非表示
		pEffect->setEmit(false);
	}

	// 移動ベクトルをプレイヤーの座標に加算
	pPlayer->addPosition(&vMove);

	// フェンスからはみ出さないように座標を制限する
	// プレイヤーの原点(円の中心)から距離を調べる
	XMVECTOR vDist = XMVector3Length(*pPlayer->getPosition());
	float dist = XMVectorGetX(vDist);

	// 距離が、フェンスの半径を超えているか調べる
	if (dist >= FENCE_RADIUS)
	{
		// 超えていた場合、プレイヤーの位置(座標)を半径内に収まるところに設定
		// 正規化(長さを1にする)
		XMVECTOR v = XMVector3Normalize(*pPlayer->getPosition());

		// 数値をかけて、設定したい長さのベクトルを作成
		v *= FIELD_RADIUS;
		// v = XMVectorScale(v, FIELD_RADIUS); // 関数の場合

		// 半径の内側に再計算された座標にプレイヤーを移動
		pPlayer->setPosition(&v);
	}

	// カメラの回転
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
	if (sanKeyboard::trg(DIK_L)) // グリッド/軸表示非表示
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


	// 仰角(Φ)の計算
	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi) * radius;
	float pz = sin(theta) * cosf(phi) * radius;

	// XMVECTORの変数はx,y,z,wの情報が入っている
	XMVECTOR CamPos = v;//XMVectorSet(px, py, pz, 0.0f);

	// カメラの座標にプレイヤーの座標を加算
	CamPos = XMVectorAdd(CamPos, *pPlayer->getPosition());

	// 少しだけ上げる
	XMVECTOR CamBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos, CamBias);

	// カメラの座標(ポインタな為アドレスを渡す)
	sanCamera::setPosition(&CamPos);

	// カメラの注視点をプレイヤーの座標に設定
	XMVECTOR camTrg = *pPlayer->getPosition();
	camTrg = XMVectorAdd(camTrg, CamBias);
	sanCamera::setTarget(&camTrg);

	// NPCの認識範囲
// 1条件、プレイヤーとの距離10以内
	XMVECTOR npcToPlayer = *pPlayer->getPosition() - *pNPC->getPosition();
	vDist = XMVector3Length(npcToPlayer);
	dist = XMVectorGetX(vDist);

	// 2条件、プレイヤーへの角度が前方±45°以内
	// 1.NPCの前方ベクトル
	XMMATRIX npcWorld = *pNPC->getWorld(); // ワールドマトリクスを取得
	XMVECTOR npcFront = npcWorld.r[2]; // ワールドマトリクスの3行目が前方のベクトル
	// 2.プレイヤーへ向かうベクトル
	// 正規化(内積がcasθになるように)
	npcFront = XMVector3Normalize(npcFront);
	npcToPlayer = XMVector3Normalize(npcToPlayer);

	// 内積
	XMVECTOR vDot = XMVector3Dot(npcFront, npcToPlayer);
	float dot = XMVectorGetX(vDot); // cosθ
	if (dot > 1.0f)
	{
		dot = 1.0f;
	}
	float radian = acosf(dot);
	float degree = XMConvertToDegrees(radian);

	// 認識範囲に入っているか
	if (dist < 10.0f && degree < 45.0f)
	{
		// 進行方向(プレイヤーの方向)に向ける処理
		float x = XMVectorGetX(npcToPlayer);
		float z = XMVectorGetZ(npcToPlayer);
		float rotY = atan2f(x, z);
		pNPC->setRotationY(rotY);

		// プレイヤーに向かって移動(プレイヤーの２分の1の速度)
		npcToPlayer *= 0.3 * 0.5f;
		pNPC->addPosition(&npcToPlayer);
		pBillboard->setRenderEnable(true);
		pEffectNPC->setEmit(true);
	}
	else
	{
		pBillboard->setRenderEnable(false);
		pEffectNPC->setEmit(false);
	}

	sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
	sanDebugDraw::Axis(5.0f, gridAxisActive);

	// デバッグ情報
	sanFont::print(20.0f, 20.0f,
		L"Player : (%.3f, %.3f, %.3f)",
		pPlayer->getPositionX(),
		pPlayer->getPositionY(),
		pPlayer->getPositionZ()
	);

	sanFont::print(20.0f, 40.0f, L"Player RY : %.3f", pPlayer->getRotationY() * radians);
	sanFont::print(20.0f, 60.0f, L"Radius : %.3f", radius);
	sanFont::print(20.0f, 80.0f, L"Theta : %.3f", theta * radians);
	sanFont::print(20.0f, 100.0f, L"Phi : %.3f", phi * radians);
	sanFont::print(20.0f, 120.0f, L"NPCとプレイヤーの距離 : %.3f", dist);
	sanFont::print(20.0f, 140.0f, L"NPCとプレイヤーの角度 : %.3f", degree);
	sanFont::print(20.0f, 160.0f, L"CrownLocalPos : %.3f, %.3f, %.3f",
		pCrown->getPositionX(),
		pCrown->getPositionY(),
		pCrown->getPositionZ());
	XMVECTOR vv;
	pCrown->getWorldPosition(&vv);
	sanFont::print(20.0f, 180.0f, L"CrownWorldPos : %.3f, %.3f, %.3f",
		pCrown->getWorldPositionX(),
		pCrown->getWorldPositionY(),
		pCrown->getWorldPositionZ());

	sanScene::execute();
}

// 描画関数
void SceneGameTest::render()
{
	sanScene::render();
}
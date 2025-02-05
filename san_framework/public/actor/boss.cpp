#include "../../framework.h"
#include "../../framework/san_environment.h"

boss::boss(const WCHAR* folder, const WCHAR* file) : sanModel(folder, file)
{
	pShadow = new sanModel(L"data/model/", L"shadow.vnm");
	pSe[0] = new sanSound(L"data/sound/hitatk.wav");
	pSe[1] = new sanSound(L"data/sound/nothitatk.wav");
	status.atkPower = 10.0f;
	status.health = 100.0f;
	status.isDefense = false;
	status.maxAtkPower = status.atkPower;
	status.maxHealth = status.health;
	handleAction = handleActionState::Defending;
	pi = 3.14f;
	atkProgress = 0.0f;
	atkTimeLimit = 12.0f; // (60fpsなので2秒)
	isDead = false;
	isDefense = false;
	isPlayerAtkRange = false;
	isPlayerJustStep = false;
	isTakeDamage = false;
	isTakeDamageDisPlay = false;
	pShadow->setTransparent(true); // 半透明有無

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

boss::~boss()
{
	delete pSe[0];
	delete pSe[1];
	delete pShadow;
}

void boss::DecideNextAction(player* rival)
{
	// 最終的には残りのHPやプレイヤーの状況によって行動を変える
	// プレイヤーが近い時は防御と攻撃
	// プレイヤーが範囲外の時は防御と移動
	if(playerCloseSearch(rival))
	{
		int actionIndex = rand() % 2; // 0か1
		handleAction = static_cast<handleActionState>(actionIndex);
	}
	else
	{
		int actionIndex = (rand() % 2) * 2; // 0か2
		handleAction = static_cast<handleActionState>(actionIndex);
	}
}

void boss::execute(player* rival)
{
	if (isDead) // 死んだとき
	{
		// 死んだアニメーションさせる
		return;
	}

	// ここでランダムにする
	// 各関数で処理の最後にDecideNextActionを呼び出す
	switch (handleAction)
	{
	case handleActionState::Defending:
		defense(rival);
		sanFont::print(20.0f, 320.0f, L"防御中");
		break;
	case handleActionState::Attacking:
		atk(rival);
		sanFont::print(20.0f, 320.0f, L"攻撃中");
		break;
	case handleActionState::Moveing:
		move(rival);
		sanFont::print(20.0f, 320.0f, L"移動中");
		break;
	}

	// ダメージを受けた時
	damageDisplay();
}

void boss::defense(player* rival)
{
	static float defenseProgress = 0.0f;      // 防御の進行度
	constexpr float defenseTimeLimit = 12.0f; // 防御の時間(60fpsなので2秒)

	isDefense = true; // 防御中にする

	defenseProgress += 0.1f; // 防御が進行する

	// 時間になったら
	if (defenseProgress >= defenseTimeLimit)
	{
		defenseProgress = 0.0f; // 値のリセット
		isDefense = false;
		DecideNextAction(rival); // 次の攻撃
	}

	// 敵とプレイヤーの位置を取得
	XMVECTOR playerPos = *(rival->getPosition());
	XMVECTOR enemyPos = *(getPosition());

	// プレイヤーから敵へのベクトルと距離を計算
	XMVECTOR vToRival = XMVectorSubtract(playerPos, enemyPos);
	float targetRotY = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival)); // 敵の方向

	// 現在の回転角度を取得
	float currentRotY = getRotationY();

	// ゆっくり回転させる (補間率 0.1f は回転速度を調整)
	float smoothFactor = 0.07f; // 0.0f〜1.0f (小さいほどゆっくり回る)
	float newRotY = currentRotY + (targetRotY - currentRotY) * smoothFactor;

	setRotationY(newRotY); // 更新
}

void boss::atk(player* rival)
{
	// プレイヤーがジャスト回避でなければ
	if (!isPlayerJustStep)
	{
		atkProgress += 0.1f;
	}
	else // プレイヤーがジャスト回避なら
	{
		atkProgress += 0.01f;
	}

	// 時間になったら
	if (atkProgress >= atkTimeLimit)
	{
		atkProgress = 0.0f;
		isPlayerJustStep = false;
		handleAction = handleActionState::Defending; // 防御にする
	}

	// 固定値なのでメモリを削減
	constexpr float atkDist = 3.5f;			 // 攻撃距離
	constexpr float atkDegree = 30.0f;		 // 攻撃範囲

	// NPCの認識範囲
	XMVECTOR bossToPlayer = *rival->getPosition() - *getPosition();
	XMVECTOR vDist = XMVector3Length(bossToPlayer);
	float dist = XMVectorGetX(vDist);

	XMMATRIX bossWorld = *getWorld();
	XMVECTOR bossrFront = bossWorld.r[2]; // 前方向
	bossrFront = XMVector3Normalize(bossrFront);
	bossToPlayer = XMVector3Normalize(bossToPlayer);

	// 内積
	XMVECTOR vDot = XMVector3Dot(bossrFront, bossToPlayer);
	float dot = XMVectorGetX(vDot); // cosθ
	float radian = acosf(dot);
	float degree = XMConvertToDegrees(radian);

	// 攻撃時のデバッグライン表示開始
	const int segments = 4; // 円弧の分割数
	float halfAngle = atkDegree / 2.0f;

	// 円弧の中心点
	XMVECTOR center = *getPosition();

	// 前方向を基準に左右の範囲を計算
	for (int i = 0; i < segments; ++i)
	{
		float theta1 = -halfAngle + (atkDegree / segments) * i;          // 現在の角度
		float theta2 = -halfAngle + (atkDegree / segments) * (i + 1);    // 次の角度

		// 現在の角度での方向を計算
		XMVECTOR dir1 = XMVector3Transform(bossrFront,
			XMMatrixRotationY(XMConvertToRadians(theta1)));
		XMVECTOR dir2 = XMVector3Transform(bossrFront,
			XMMatrixRotationY(XMConvertToRadians(theta2)));

		// 各方向に距離を掛けてポイントを計算
		XMVECTOR point1 = center + dir1 * atkDist;
		XMVECTOR point2 = center + dir2 * atkDist;

		// デバッグラインを描画
		sanDebugDraw::Line(&center, &point1, 0xffffff00);  // 中心から外へ
		sanDebugDraw::Line(&point1, &point2, 0xffffff00); // 円弧の外周を繋ぐ
		// 右端の点を更新
		XMVECTOR lastPoint = point2;
		// 右端の点から中心への線を描画
		sanDebugDraw::Line(&lastPoint, &center, 0xffffff00);
	}
	// 攻撃時のデバッグライン表示終了

	// 認識範囲に入っているか
	if (dist < atkDist && degree < atkDegree)
	{
		// プレイヤーが攻撃が当たる範囲にいるかどうか
		isPlayerAtkRange = true; 

		// 攻撃が半分以上の時
		if (atkProgress >= atkTimeLimit / 2)
		{
			// 攻撃処理
			XMVECTOR knockbackVector = bossrFront * 4;
			XMVECTOR newPosition = *rival->getPosition() + knockbackVector;
			rival->setPosition(&newPosition);
			rival->takeDamage(status.atkPower);
		}
	}
	else
	{
		isPlayerAtkRange = false;
	}

	// 敵に当たった時
	if (rival->getIsTakeHit())
	{
		// SE再生
		// もし再生中なら停止する
		//if (pSe[0]->isPlaying() == true)
		//{
		//	pSe[0]->stop();
		//}
		//if (pSe[0]->isPlaying() == false)
		//{
		//	pSe[0]->play();
		//}
	}
	else
	{
		// SE再生
		// もし再生中なら停止する
		//if (pSe[1]->isPlaying() == true)
		//{
		//	pSe[1]->stop();
		//}
		//if (pSe[1]->isPlaying() == false)
		//{
		//	pSe[1]->play();
		//}
	}
}

void boss::move(player* rival)
{
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 移動ベクトル

	// 固定値なのでメモリを削減
	constexpr float moveSpeed = 0.1f; // プレイヤーの移動速度
	constexpr float stopDistance = 3.0f; // プレイヤーの手前で止まる距離

	static float moveProgress = 0.0f;      // 防御の進行度
	constexpr float moveTimeLimit = 18.0f; // 防御の時間(60fpsなので3秒)

	moveProgress += 0.1f;

	// 時間になったら
	if (moveProgress >= moveTimeLimit)
	{
		moveProgress = false;
		DecideNextAction(rival); // 次の攻撃
	}


	// 敵と自身の位置を取得
	XMVECTOR rivalPos = *(rival->getPosition());
	XMVECTOR playerPos = *(getPosition());

	// プレイヤーから敵へのベクトルと距離を計算
	XMVECTOR vToRival = XMVectorSubtract(rivalPos, playerPos);
	float distToRival = XMVectorGetX(XMVector3Length(vToRival));

	// 敵に向かって前進(距離が一定以上の場合のみ)
	if (distToRival > stopDistance) 
	{
		// 敵に向かって前進
		vMove = XMVectorScale(vToRival, 1.0f / distToRival); // 正規化
		// 移動ベクトルにスピードを適用(長さを変える)
		vMove = XMVectorScale(vMove, moveSpeed);
	}

	// 敵を正面に向けるよう回転を設定
	float rotY = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));
	setRotationY(rotY);

	// プレイヤーを移動
	XMVECTOR newPos = XMVectorAdd(playerPos, vMove);
	setPosition(&newPos); // 新しい位置を設定

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

void boss::takeJustStep()
{
	isPlayerJustStep = true;
}

void boss::takeDamage(float damage)
{
	if (isDefense)
	{
		return;
	}
	else if (status.health <= 0) // 死んだとき
	{
		isDead = true;
	}
	isTakeDamage = true;
	status.health -= damage;
}

void boss::damageDisplay()
{
	static float takeDamageProgress; // 攻撃受け時の進行度

	if (!isTakeDamage) return;
	if (takeDamageProgress >= 20)
	{
		takeDamageProgress = 0.0f;
		isTakeDamageDisPlay = false;
		isTakeDamage = false;
	}
	else
	{
		takeDamageProgress++;
		if (takeDamageProgress == 5) isTakeDamageDisPlay = true;
		if (takeDamageProgress == 10) isTakeDamageDisPlay = false;
		if (takeDamageProgress == 15) isTakeDamageDisPlay = true;
	}

}

bool boss::playerCloseSearch(player* rival)
{
	// ボスとプレイヤーの距離
	XMVECTOR bossToPlayer = *rival->getPosition() - *getPosition();
	XMVECTOR vDist = XMVector3Length(bossToPlayer);
	float dist = XMVectorGetX(vDist);

	// 一定距離内のプレイヤーがいるかどうか
	if (dist <= 8)
	{
		return true;
	}
	else // 一定距離以上離れているかどうか
	{
		return false;
	}
}

bool boss::getIsDead()
{
	return isDead;
}

bool boss::getTakeDamageDisPlay()
{
	return isTakeDamageDisPlay;
}

float boss::getCurrentHp()
{
	return status.health;
}

bool boss::getIsTakeHit()
{
	return isTakeDamage;
}

bool boss::getIsDefense()
{
	return isDefense;
}

bool boss::getAtkProgress()
{
	return atkProgress;
}

bool boss::getPlayerAtkRange()
{
	return isPlayerAtkRange;
}

void boss::playerAllRender()
{
	sanFont::print(20.0f, 300.0f, L"体力 : %.3f", status.health);
	sanFont::print(20.0f, 340.0f, L"ボスの攻撃進捗度 : %.3f", atkProgress);
	pShadow->render();
	render();
}

handleActionState boss::getBossAction()
{
	return handleAction;
}
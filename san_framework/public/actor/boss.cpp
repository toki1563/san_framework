#include "../../framework.h"
#include "../../framework/san_environment.h"

boss::boss(const WCHAR* folder, const WCHAR* file) : sanModel(folder, file)
{
	pRightArm = new sanModel(L"data/model/Player_2/", L"RightArm.vnm");
	pRightArmAtkCoolTime = new sanModel(L"data/model/Player_2/", L"RightArmAtk.vnm");
	pShadow = new sanModel(L"data/model/", L"shadow.vnm");
	pSe[0] = new sanSound(L"data/sound/hitatk.wav");
	pSe[1] = new sanSound(L"data/sound/nothitatk.wav");
	status.atkPower = 10.0f;
	status.health = 1000.0f;
	status.isDefense = false;
	status.maxAtkPower = status.atkPower;
	status.maxHealth = status.health;
	handleAction = handleActionState::Defending;
	pi = 3.14f;
	isDefence = false;
	isTakeDamage = false;
	isAtkCoolTime = false;
	isTakeDamageDisPlay = false;
	pShadow->setTransparent(true); // 半透明有無

	// 敵の腕
	pRightArm->setRotationX(0.0f * pi);
	pRightArm->setRotationY(0.0f * pi);
	pRightArm->setRotationZ(0.0f * pi);
	pRightArmAtkCoolTime->setRotationX(0.0f * pi);
	pRightArmAtkCoolTime->setRotationY(0.0f * pi);
	pRightArmAtkCoolTime->setRotationZ(0.0f * pi);
	pRightArm->setPosition(getPositionX() - 0.7f, getPositionY() + 3.0f, getPositionZ());
	pRightArmAtkCoolTime->setPosition(getPositionX() - 0.7f, getPositionY() + 3.0f, getPositionZ());
	pRightArm->setRotationX(-0.1f * pi);
	pRightArm->setRotationZ(1.0f * pi);
	pRightArmAtkCoolTime->setRotationX(-0.1f * pi);
	pRightArmAtkCoolTime->setRotationZ(1.0f * pi);

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

boss::~boss()
{
	delete pSe[0];
	delete pSe[1];
	delete pRightArmAtkCoolTime;
	delete pRightArm;
	delete pShadow;
}

void boss::actionState(player* rival)
{
	// ここでランダムにする
	// 各関数で処理の最後にDecideNextActionを呼び出す
	switch (handleAction)
	{
	case handleActionState::Defending:
		defense(rival);
		break;
	case handleActionState::Attacking:
		atk(rival);
		break;
	case handleActionState::Move:
		move(rival);
		break;
	case handleActionState::MAX:
		break;
	}
	// ランダムで処理する(別で関数を作成する)
	//atk(rival);
	//damageDisplay();
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
	actionState(rival);
}

void boss::defense(player* rival)
{
	// 敵とプレイヤーの位置を取得
	XMVECTOR playerPos = *(rival->getPosition());
	XMVECTOR enemyPos = *(getPosition());

	// プレイヤーから敵へのベクトルと距離を計算
	XMVECTOR vToRival = XMVectorSubtract(playerPos, enemyPos);

	// 敵を正面に向けるよう回転を設定
	float rotY = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));
	setRotationY(rotY);
}

void boss::atk(player* rival)
{
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

	// 認識範囲に入っているか
	if (dist < 3.5f && degree < 45.0f)
	{
		// 攻撃処理
		XMVECTOR knockbackVector = bossrFront * 4;
		XMVECTOR newPosition = *rival->getPosition() + knockbackVector;
		rival->setPosition(&newPosition);
		rival->takeDamage(status.atkPower);
	}

	// 敵に当たった時
	if (rival->getIsTakeHit())
	{
		// SE再生
		// もし再生中なら停止する
		if (pSe[0]->isPlaying() == true)
		{
			pSe[0]->stop();
		}
		if (pSe[0]->isPlaying() == false)
		{
			pSe[0]->play();
		}
	}
	else
	{
		// SE再生
		// もし再生中なら停止する
		if (pSe[1]->isPlaying() == true)
		{
			pSe[1]->stop();
		}
		if (pSe[1]->isPlaying() == false)
		{
			pSe[1]->play();
		}
	}
}

void boss::move(player* rival)
{
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 移動ベクトル

	// 固定値なのでメモリを削減
	constexpr float moveSpeed = 0.1f; // プレイヤーの移動速度

	// 敵と自身の位置を取得
	XMVECTOR rivalPos = *(rival->getPosition());
	XMVECTOR playerPos = *(getPosition());

	// プレイヤーから敵へのベクトルと距離を計算
	XMVECTOR vToRival = XMVectorSubtract(rivalPos, playerPos);
	float distToRival = XMVectorGetX(XMVector3Length(vToRival));

	// 敵に向かって前進
	vMove = XMVectorScale(vToRival, 1.0f / distToRival); // 正規化

	// 移動ベクトルにスピードを適用(長さを変える)
	vMove = XMVectorScale(vMove, moveSpeed);

	// 腕をついてくるように設定
	pRightArm->setPosition(getPositionX() + 0.7f, getPositionY() + 3.0f, getPositionZ());
	pRightArmAtkCoolTime->setPosition(getPositionX() + 0.7f, getPositionY() + 3.0f, getPositionZ());

	// 敵を正面に向けるよう回転を設定
	float rotY = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));
	setRotationY(rotY);

	// プレイヤーを移動
	XMVECTOR newPos = XMVectorAdd(playerPos, vMove);
	setPosition(&newPos); // 新しい位置を設定

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

bool boss::playerCloseSearch(player* rival)
{
	// ボスとプレイヤーの距離
	XMVECTOR bossToPlayer = *rival->getPosition() - *getPosition();
	XMVECTOR vDist = XMVector3Length(bossToPlayer);
	float dist = XMVectorGetX(vDist);

	// 一定距離内のプレイヤーがいるかどうか
	if (dist <= 10)
	{
		return true;
	}
	else // 一定距離以上離れているかどうか
	{
		return false;
	}
}

void boss::takeDamage(float damage)
{
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

bool boss::getTakeDamageDisPlay()
{
	return isTakeDamageDisPlay;
}

bool boss::getIsTakeHit()
{
	return isTakeDamage;
}

bool boss::getIsDefense()
{
	return isDefense;
}

void boss::playerAllRender()
{
	sanFont::print(20.0f, 300.0f, L"体力 : %.3f", status.health);
	pShadow->render();
	// クールタイム中は切り替える
	if (isAtkCoolTime)
	{
		render();
		pRightArmAtkCoolTime->render();
	}
	else
	{
		render();
		pRightArm->render();
	}
}

handleActionState boss::getBossAction()
{
	return handleAction;
}
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
		move(rival);
		break;
	case handleActionState::Move:
		atk(rival);
		break;
	case handleActionState::Stunned:
		break;
	case handleActionState::MAX:
		break;
	}
	// ランダムで処理する(別で関数を作成する)
	atk(rival);
	damageDisplay();
}

void boss::DecideNextAction()
{
	// ここでランダムにする
	// 最終的には残りのHPやプレイヤーの状況によって行動を変える
	//handleAction = 
}

void boss::execute(player* rival)
{
	actionState(rival);
}

void boss::defense(player* rival)
{

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
		rival->takeDamage();
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
	constexpr float moveSpeed = 0.1f;

	if (sanKeyboard::on(DIK_U) || sanXInput::leftY(1) >= 0.5f)
	{
		vMove = XMVectorSetZ(vMove, 1);
	}
	if (sanKeyboard::on(DIK_J) || sanXInput::leftY(1) <= -0.5f)
	{
		vMove = XMVectorSetZ(vMove, -1);
	}
	if (sanKeyboard::on(DIK_H) || sanXInput::leftX(1) <= -0.5f)
	{
		vMove = XMVectorSetX(vMove, -1);
	}
	if (sanKeyboard::on(DIK_K) || sanXInput::leftX(1) >= 0.5f)
	{
		vMove = XMVectorSetX(vMove, 1);
	}

	// 移動ベクトルにスピードを適用(長さを変える)
	vMove = XMVectorScale(vMove, moveSpeed);

	// ここを常に敵に向くのか行動によって向くのかなど考える

	//if (input)	// 入力があるときのみ
	//{
	//	// 進行方向(方向ベクトル : vMove)に向ける処理
	//	float rotY = atan2f(XMVectorGetX(vMove), XMVectorGetZ(vMove));
	//	setRotationY(rotY);
	//}

	addPosition(&vMove); // 移動ベクトルをプレイヤーの座標に加算

	// プレイヤーの距離を測る計算
	XMVECTOR v = XMVector3Length(*getPosition());
	float dist = XMVectorGetX(v);

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

void boss::stun()
{

}

void boss::takeDamage()
{
	isTakeDamage = true;
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

void boss::playerAllRender()
{
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
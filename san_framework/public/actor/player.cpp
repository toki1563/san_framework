#include "../../framework.h"
#include "../../framework/san_environment.h"
#include "player.h"

player::player(const WCHAR* folder, const WCHAR* file) : sanModel(folder, file)
{
	pShadow = new sanModel(L"data/model/", L"shadow.vnm");
	pSe[0] = new sanSound(L"data/sound/hitatk.wav");
	pSe[1] = new sanSound(L"data/sound/nothitatk.wav");
	pSe[2] = new sanSound(L"data/sound/roll.wav");
	status.atkPower = 10.0f;
	status.health = 100.0f;
	status.stamina = 300.0f;
	status.maxAtkPower = status.atkPower;
	status.maxHealth = status.health;
	status.maxStamina = status.stamina;
	pi = 3.14f;
	isCanAtk = true;
	isTakeDamage = false;
	isAtkCoolTime = false;
	isTakeDamageDisPlay = false;
	pShadow->setTransparent(true); // 半透明有無

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

player::~player()
{
	for (int i = 0; i < 3; ++i)
	{
		delete pSe[i];
	}
	delete pShadow;
}

void player::execute(boss* rival)
{
	move(rival);
	step(rival);
	atk(rival);
	damageDisplay();
}

void player::move(boss* rival)
{
	if (!isCanAtk) return;
	static bool isInput = false; // 押されているかどうか

	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 移動ベクトル

	// 固定値なのでメモリを削減
	constexpr float moveSpeed = 0.1f; // プレイヤーの移動速度
	constexpr float stopDistance = 3.0f;     // 敵の手前で止まる距離

	// 敵と自身の位置を取得
	XMVECTOR rivalPos = *(rival->getPosition());
	XMVECTOR playerPos = *(getPosition());

	// プレイヤーから敵へのベクトルと距離を計算
	XMVECTOR vToRival = XMVectorSubtract(rivalPos, playerPos);
	float distToRival = XMVectorGetX(XMVector3Length(vToRival));

	// 敵を中心としたプレイヤーの角度を計算
	float angleToRival = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));

	if (sanKeyboard::on(DIK_W) || sanXInput::leftY(0) >= 0.5f)
	{
		// 敵に向かって前進(距離が一定以上の場合のみ)
		if (distToRival > stopDistance)
		{
			vMove = XMVectorScale(vToRival, 1.0f / distToRival); // 正規化
			isInput = true;
		}
	}
	if (sanKeyboard::on(DIK_S) || sanXInput::leftY(0) <= -0.5f)
	{
		// 敵から後退
		vMove = XMVectorScale(vToRival, -1.0f / distToRival); // 逆方向
		isInput = true;
	}
	if (sanKeyboard::on(DIK_A) || sanXInput::leftX(0) <= -0.5f)
	{
		// 敵を中心に左移動（時計回り）
		float leftAngle = angleToRival - XM_PIDIV2; // -90度
		vMove = XMVectorSet(sinf(leftAngle), 0.0f, cosf(leftAngle), 0.0f);
		isInput = true;
	}
	if (sanKeyboard::on(DIK_D) || sanXInput::leftX(0) >= 0.5f)
	{
		// 敵を中心に右移動（反時計回り）
		float rightAngle = angleToRival + XM_PIDIV2; // +90度
		vMove = XMVectorSet(sinf(rightAngle), 0.0f, cosf(rightAngle), 0.0f);
		isInput = true;
	}

	// 移動ベクトルにスピードを適用(長さを変える)
	vMove = XMVectorScale(vMove, moveSpeed);

	if (isInput)	// 入力があるときのみ
	{
		// 敵を正面に向けるよう回転を設定
		float rotY = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));
		setRotationY(rotY);

		// プレイヤーを移動
		XMVECTOR newPos = XMVectorAdd(playerPos, vMove);
		setPosition(&newPos); // 新しい位置を設定
	}

	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

void player::atk(boss* rival)
{
	static float atkProgress = 0.0f;          // 攻撃の進行度
	static float atkCoolTimeProgress = 0.0f;  // クールタイムの進行度

	// コンボ回数は決まっているのでuint8_t型を使用しメモリ削減
	static uint8_t atkCombo = 0; // 攻撃のコンボカウント

	// 固定値なのでメモリを削減
	constexpr float atkDist = 3.5f;			 // 攻撃距離
	constexpr float atkDegree = 30.0f;		 // 攻撃範囲
	constexpr float atkDuration = 10.0f;	 // 攻撃モーションのの持続時間
	constexpr float atkCoolTimeLimit = 1.2f; // クールタイムの長さ(60fpsなので0.2秒)

	// デバッグ用
	sanFont::print(20.0f, 180.0f, L"攻撃の進捗度 : %.3f", atkProgress);
	sanFont::print(20.0f, 200.0f, L"コンボカウント : %d", atkCombo);
	sanFont::print(20.0f, 220.0f, L"クールタイムの進捗度 : %.3f", atkCoolTimeProgress);

	// NPCの認識範囲
	XMVECTOR playerToEnemy = *rival->getPosition() - *getPosition();
	XMVECTOR vDist = XMVector3Length(playerToEnemy);
	float dist = XMVectorGetX(vDist);

	XMMATRIX playerWorld = *getWorld();
	XMVECTOR playerFront = playerWorld.r[2]; // プレイヤーの前方向
	playerFront = XMVector3Normalize(playerFront);
	playerToEnemy = XMVector3Normalize(playerToEnemy);

	// 内積
	XMVECTOR vDot = XMVector3Dot(playerFront, playerToEnemy);
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
		XMVECTOR dir1 = XMVector3Transform(playerFront,
			XMMatrixRotationY(XMConvertToRadians(theta1)));
		XMVECTOR dir2 = XMVector3Transform(playerFront,
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

	// 初の攻撃時
	if (isCanAtk)
	{
		// 攻撃入力時
		if ((sanKeyboard::trg(DIK_E) || sanXInput::trg(XINPUT_GAMEPAD_X, 0)))
		{
			isCanAtk = false; // 攻撃不可状態

			// 攻撃範囲なら攻撃する
			if (dist < atkDist && degree < atkDegree)
			{
				//ノックバック
				//XMVECTOR knockbackVector = playerFront * 4;
				//XMVECTOR newPosition = *rival->getPosition() + knockbackVector;
				//rival->setPosition(&newPosition);
				rival->takeDamage(status.atkPower);
			}

			// 敵に攻撃が当たった時
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
	}

	// 攻撃中の処理
	if (!isCanAtk)
	{
		// 攻撃の進捗が最後まで進行するまで＆クールタイムでない時
		if (atkProgress <= atkDuration && !isAtkCoolTime)
		{
			atkProgress += 0.1f;  // 攻撃が進行する
		}
		else // 攻撃が終わった時
		{
			// 攻撃が終わった後にクールタイムを開始
			isAtkCoolTime = true;
			atkProgress = 0.0f;	// 攻撃の進捗リセット
			atkCombo = 0;
		}

		// 攻撃の進捗が攻撃終了の半分以上経過したら
		// つまり攻撃を行ってから攻撃の終了までの後半からコンボ判定を認識する
		// 3コンボまで使用するとクールタイムに入る
		if (atkProgress >= (atkDuration / 2))
		{
			// 攻撃入力時
			if ((sanKeyboard::trg(DIK_E) || sanXInput::trg(XINPUT_GAMEPAD_X, 0)))
			{
				// 攻撃のコンボの上限が3回
				// 攻撃のコンボが3回以下なら
				if (atkCombo < 2)
				{
					atkCombo++;
					atkProgress = 0.0f; // 攻撃進捗リセット

					// 攻撃範囲なら攻撃する
					if (dist < 3.5f && degree < 45.0f)
					{
						//ノックバック
						//XMVECTOR knockbackVector = playerFront * 4;
						//XMVECTOR newPosition = *rival->getPosition() + knockbackVector;
						//rival->setPosition(&newPosition);

						// 攻撃が3連目の時
						if (atkCombo == 2)
						{
							rival->takeDamage(status.atkPower * 1.5);
						}
						else // 攻撃が2連目の時
						{
							rival->takeDamage(status.atkPower * 1.2);
						}
					}

					// 敵に攻撃が当たった時
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
			}
		}
	}

	// クールタイム処理
	if (isAtkCoolTime)
	{
		atkCoolTimeProgress += 0.1f; // クールタイム時間更新

		// クールタイムの進捗が制限を超えた時
		if (atkCoolTimeProgress >= atkCoolTimeLimit)
		{
			atkCoolTimeProgress = 0.0f; // 攻撃のクールタイムの進捗度リセット
			isAtkCoolTime = false;
			isCanAtk = true;
		}
	}
}

void player::step(boss* rival)
{
	static float lastLeftTime = -1.0f;
	static float lastRightTime = -1.0f;
	static float stepEndTime = -1.0f; // ステップ終了時間
	static bool isRightStep = false;  // 右にステップ回避
	static bool isLeftStep = false;	  // 左にステップ回避
	static bool isStep = false;       // ステップしているかどうか
	static float stepValue = status.maxStamina / 3; // ステップに必要なスタミナの量

	float stepMoveSpeed = 0.6f;			   // ステップの移動速度
	float currentTime = getCurrentTime();  // 現在の時間を取得
	float stepDuration = 0.1f;             // ステップの持続時間
	float doubleClickTime = 0.3f;          // ダブルクリック判定時間

	// スタミナ回復処理
	if (status.stamina < status.maxStamina)
	{
		status.stamina++;
	}

	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 移動ベクトル

	// 敵と自身の位置を取得
	XMVECTOR rivalPos = *(rival->getPosition());
	XMVECTOR playerPos = *(getPosition());

	// プレイヤーから敵へのベクトルと距離を計算
	XMVECTOR vToRival = XMVectorSubtract(rivalPos, playerPos);
	float distToRival = XMVectorGetX(XMVector3Length(vToRival));

	// 敵を中心としたプレイヤーの角度を計算
	float angleToRival = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));

	// ステップ中の処理
	if (currentTime <= stepEndTime)
	{
		if (isStep)
		{
			// スタミナを半分減らす
			status.stamina -= stepValue;
			isStep = false;

			// SE再生
			if (pSe[2]->isPlaying() == true)
			{
				pSe[2]->stop();
			}

			if (pSe[2]->isPlaying() == false)
			{
				pSe[2]->play();
			}
		}
		if (isRightStep)
		{
			// 敵を中心に右移動（反時計回り）
			float rightAngle = angleToRival + XM_PIDIV2; // +90度
			vMove = XMVectorSet(sinf(rightAngle), 0.0f, cosf(rightAngle), 0.0f);
		}
		else if (isLeftStep)
		{
			// 敵を中心に左移動（時計回り）
			float leftAngle = angleToRival - XM_PIDIV2; // -90度
			vMove = XMVectorSet(sinf(leftAngle), 0.0f, cosf(leftAngle), 0.0f);
		}

		// 移動ベクトルにスピードを適用(長さを変える)
		vMove = XMVectorScale(vMove, stepMoveSpeed);


		// 敵を正面に向けるよう回転を設定
		float rotY = atan2f(XMVectorGetX(vToRival), XMVectorGetZ(vToRival));
		setRotationY(rotY);

		// プレイヤーを移動
		XMVECTOR newPos = XMVectorAdd(playerPos, vMove);
		setPosition(&newPos); // 新しい位置を設定
		return; // ステップ中は他の処理を無効にする
	}
	else
	{
		isRightStep = false;
		isLeftStep = false;
	}

	// スタミナがある時
	if (status.stamina >= stepValue)
	{
		// 右ステップ
		if (sanKeyboard::trg(DIK_D))
		{
			if (currentTime - lastRightTime <= doubleClickTime)
			{
				// ステップの設定（時計回り）
				stepEndTime = currentTime + stepDuration;  // ステップ終了時間を設定
				isRightStep = true;
				isStep = true;
			}

			lastRightTime = currentTime; // 入力時間を記録
		}

		// 左ステップ
		if (sanKeyboard::trg(DIK_A))
		{
			if (currentTime - lastLeftTime <= doubleClickTime)
			{
				// ステップの設定（反時計回り）
				stepEndTime = currentTime + stepDuration;  // ステップ終了時間を設定
				isLeftStep = true;
				isStep = true;
			}

			lastLeftTime = currentTime; // 入力時間を記録
		}
	}
}

void player::takeDamage(float damage)
{
	isTakeDamage = true;
	status.health -= damage;
}

void player::damageDisplay()
{
	static float takeDamageProgress = 0.0f; // 攻撃受け時の進行度

	// ダメージを受けていないときは短縮
	if (!isTakeDamage) return;

	// 攻撃を受けた時ダメージ受けの可視化
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

bool player::getTakeDamageDisPlay()
{
	return isTakeDamageDisPlay;
}

float player::getCurrentTime()
{
	// 起動からの経過時間を秒単位で取得
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	return std::chrono::duration<float>(duration).count();
}

bool player::getIsTakeHit()
{
	return isTakeDamage;
}

void player::playerAllRender()
{
	sanFont::print(20.0f, 140.0f, L"体力 : %.3f", status.health);
	sanFont::print(20.0f, 160.0f, L"スタミナ : %.3f", status.stamina);
	
	pShadow->render();
	// クールタイム中は切り替える
	if (isAtkCoolTime)
	{
		render();
	}
	else
	{
		render();
	}
}
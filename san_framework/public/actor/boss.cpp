#include "../../framework.h"
#include "../../framework/san_environment.h"
#include "boss.h"


#define OUTPUT_MOTION_DATA (0)

#define FULL_PATH_MAX	(256)
#define FILE_PATH_MAX	(128)

WCHAR bossMotionFolder[] = L"data/model/BoxUnityChan/motion/";

WCHAR bossMotionFile[][FILE_PATH_MAX] =
{
	L"DAMAGED00.mot", //  0 : ちょいダメージ
	L"DAMAGED01.mot", //  1 : 吹っ飛び
	L"HANDUP00_R.mot",//  2 : 片手上げ
	L"JUMP00.mot",	  //  3 : 高ジャンプ
	L"JUMP00B.mot",	  //  4 : 低ジャンプ
	L"JUMP01.mot",	  //  5 : 優勝喜び1　
	L"JUMP01B.mot",	  //  6 : 優勝喜び2
	L"LOSE00.mot",	  //  7 : 頭抱え込む
	L"REFLESH00.mot", //  8 : 疲れた膝に手を置く
	L"RUN00_F.mot",	  //  9 : 走る
	L"RUN00_L.mot",	  // 10 : 走る左向く
	L"RUN00_R.mot",	  // 11 : 走る右向く
	L"SLIDE00.mot",	  // 12 : スライディング
	L"UMATOBI00.mot", // 13 : 跳び箱
	L"WAIT00.mot",	  // 14 : 通常立ち
	L"WAIT01.mot",	  // 15 : 手を前に伸ばしてのびのび
	L"WAIT02.mot",	  // 16 : 左右にルンルン
	L"WAIT03.mot",	  // 17 : にっこにっこにー
	L"WAIT04.mot",	  // 18 : まわし蹴り
	L"WALK00_B.mot",  // 19 : 後ろ見ながら歩く
	L"WALK00_F.mot",  // 20 : 正面見ながら歩く
	L"WALK00_L.mot",  // 21 : 左見ながら歩く
	L"WALK00_R.mot",  // 22 : 右見ながら歩く
	L"WIN00.mot",	  // 23 : 勝利ポーズ
};



boss::boss(const WCHAR* folder, const WCHAR* boneFile) : cCharacter(folder, boneFile)
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

	//モーションファイルの読み込み
	bossMotionNum = sizeof(bossMotionFile) / (sizeof(WCHAR) * FILE_PATH_MAX);
	bossMotion = new sanMotionData * [bossMotionNum];
	for (int i = 0; i < bossMotionNum; i++)
	{
		WCHAR path[FULL_PATH_MAX];
		wsprintf(path, L"%s%s", bossMotionFolder, &bossMotionFile[i][0]);

		bossMotion[i] = loadMotionFile(path);
		assert(bossMotion[i] != NULL);

		//回転の補正
		rotRoll(bossMotion[i]);
	}

	setMotion(bossMotion[16]);


	// 影の腕と足をプレイヤーに合わせる
	pShadow->setPosition(getPositionX(), getPositionY() + 0.01f, getPositionZ());
}

boss::~boss()
{
	for (int i = 0; i < 2; ++i)
	{
		delete pSe[i];
	}

	for (int i = 0; i < bossMotionNum; i++)
	{
		if (!bossMotion[i])continue;
		delete[] bossMotion[i];
		bossMotion[i] = NULL;
	}

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
		sanFont::setTextFormat(sanFont::create(L"Meiryo", 30));
		sanFont::print(20.0f, 350.0f, L"防御中");
		sanFont::setTextFormat(sanFont::create(L"Meiryo", 16));
		break;
	case handleActionState::Attacking:
		atk(rival);
		sanFont::setTextFormat(sanFont::create(L"Meiryo", 30));
		sanFont::print(20.0f, 350.0f, L"攻撃中");
		sanFont::setTextFormat(sanFont::create(L"Meiryo", 16));
		break;
	case handleActionState::Moveing:
		move(rival);
		sanFont::setTextFormat(sanFont::create(L"Meiryo", 30));
		sanFont::print(20.0f, 350.0f, L"移動中");
		sanFont::setTextFormat(sanFont::create(L"Meiryo", 16));
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
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 24));
	sanFont::print(20.0f, 320.0f, L"ボスの攻撃進捗度 : %.3f", atkProgress);
	sanFont::setTextFormat(sanFont::create(L"Meiryo", 16));
	pShadow->render();
	render();
}

handleActionState boss::getBossAction()
{
	return handleAction;
}

//モーションファイル読み込み関数
sanMotionData* boss::loadMotionFile(const WCHAR* motFile)
{
	if (!motFile)return NULL;

	//戻り値用変数
	sanMotionData* pMot = NULL;

	//モーションファイルの読み込み
	FILE* fp = NULL;
	if (_wfopen_s(&fp, motFile, L"rb") != 0)
	{
		//ファイルが開けなかった
		return NULL;
	}

	//ファイルサイズの取得
	long size = 0;
	fseek(fp, 0, SEEK_END);	//ファイルポインタを最後尾に移動
	size = ftell(fp);	//現在のファイルポインタ位置を取得(Byte)
	fseek(fp, 0, SEEK_SET);	//ファイルポインタを先頭に移動

	//ファイルサイズ分のメモリを確保
	pMot = (sanMotionData*)new BYTE[size];

	//ファイルの中身を全て取得
	fread(pMot, size, 1, fp);

	//ファイルを閉じる
	fclose(fp);

#if OUTPUT_MOTION_DATA
	//出力Windowへのログ表示
	sanFont::output(L"Length : %.3f\n", pMot->Length);
	sanFont::output(L"ChannelNum : %d\n", pMot->ChannelNum);
	sanFont::output(L"KeyframeNum : %d\n", pMot->KeyFrameNum);

	//チャンネルデータへのアクセス
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)pMot) + pMot->ChannelAccess);

	for (int i = 0; i < (int)pMot->ChannelNum; i++)
	{
		//char文字列をWCHAR文字列に変換
		WCHAR name[256];
		size_t ret;
		mbstowcs_s(&ret, name, 256, channel[i].Name, strlen(channel[i].Name));

		sanFont::output(L"Channel[%d].Name : %s\n", i, name);
		sanFont::output(L"Channel[%d].ChannelID : %d\n", i, channel[i].ChannelID);
		sanFont::output(L"Channel[%d].KeyFrameNum : %d\n", i, channel[i].KeyFrameNum);
		sanFont::output(L"Channel[%d].StartIndex : %d\n", i, channel[i].StartIndex);
	}

	//キーフレームデータへのアクセス
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)pMot) + pMot->KeyFrameAccess);

	for (int i = 0; i < (int)pMot->KeyFrameNum; i++)
	{
		sanFont::output(L"key[%d].Time : %f\n", i, key[i].Time);
		sanFont::output(L"key[%d].Value : %f\n", i, key[i].Value);
	}
#endif

	return pMot;
}

//回転補正
void boss::rotRoll(sanMotionData* p)
{
	if (!p)return;

	//チャンネルデータへのアクセス
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)p) + p->ChannelAccess);

	//キーフレームデータへのアクセス
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)p) + p->KeyFrameAccess);

	for (int i = 0; i < (int)p->ChannelNum; i++)
	{
		//Rotationのチャンネルのみを対象とする
		if (channel[i].ChannelID != eMotionChannel::RotX && channel[i].ChannelID != eMotionChannel::RotY && channel[i].ChannelID != eMotionChannel::RotZ)continue;

		//各チャンネルにおけるキーフレーム配列の最初
		sanMotionData_KeyFrame* k = key + channel[i].StartIndex;

		//チャンネル内での最大値と最小値を求める
		float min = 100.0f;
		float max = -100.0f;
		for (int j = 0; j < channel[i].KeyFrameNum; j++)
		{
			if (k[j].Value > max)max = k[j].Value;
			if (k[j].Value < min)min = k[j].Value;
		}

		//最大値と最小値の差が270度以上あれば補正する(中間値以上の値を360度マイナスする)
		if (max - min < XMConvertToRadians(270.0f))continue;
		float avr = (min + max) * 0.5f;
		for (int j = 0; j < channel[i].KeyFrameNum; j++)
		{
			if (k[j].Value > avr)k[j].Value -= XMConvertToRadians(360.0f);
		}
	}
}
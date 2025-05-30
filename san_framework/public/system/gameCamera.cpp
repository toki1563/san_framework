#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool gameCamera::initialize()
{
	// カメラ距離・補間設定
	cameraDistance = 3.5f; // カメラの距離
	rightOffset = 0.8f;    // 右にずらす値を調整
	lerpFactor = 0.3f;     // 補間の強さ(0~1で0に近いほうが弱い)

	// カメラの高さの設定
	minHeight = 2.5f; // カメラの高さ下限
	maxHeight = 6.0f; // カメラの高さ上限
	heightFactor = 0.1f; // どのくらい高さを上げるかの係数

	return true;
}

// 終了関数
void gameCamera::terminate()
{

}

// 処理関数
void gameCamera::execute(player* pPlayer, boss* pBoss)
{
	// ゲームクリア時の処理
	if (pBoss->getIsDead())
	{
		// カメラを上に向けてIsDeadと表示させる

	}
	// ゲームオーバー時の処理
	else if (pPlayer->getIsDead())
	{

	}
	else // 両方生きている時
	{
		// ゲーム内でのカメラの処理

		// カメラの現在位置(前フレーム)
		static XMVECTOR currentCameraPosition = XMVectorZero(); // 初期化は1回だけ

		// プレイヤーとボスの位置を取得
		XMVECTOR playerPosition = *pPlayer->getPosition();
		XMVECTOR bossPosition = *pBoss->getPosition();

		// プレイヤーのワールド行列
		XMMATRIX playerWorld = *pPlayer->getWorld();
		XMVECTOR playerForward = playerWorld.r[2]; // Z軸：前
		XMVECTOR playerRight = playerWorld.r[0]; // X軸：右


		// 敵との距離に応じて高さ調整
		float distance = XMVectorGetX(XMVector3Length(XMVectorSubtract(bossPosition, playerPosition))); // お互いの距離を計算
		float dynamicHeight = min(minHeight + distance * heightFactor, maxHeight); // カメラの高さ上限を超えないようにminを使用

		// プレイヤー背後＋右＋高さの理想カメラ位置を計算
		XMVECTOR targetCameraPosition = playerPosition;
		targetCameraPosition = XMVectorAdd(targetCameraPosition, XMVectorScale(playerForward, -cameraDistance)); // 背後の距離
		targetCameraPosition = XMVectorAdd(targetCameraPosition, XMVectorSet(0.0f, dynamicHeight, 0.0f, 0.0f));  // 距離に応じた高さ
		targetCameraPosition = XMVectorAdd(targetCameraPosition, XMVectorScale(playerRight, rightOffset));       // 右方向のずらし

		// 補間でスムーズにカメラ移動
		currentCameraPosition = XMVectorLerp(currentCameraPosition, targetCameraPosition, lerpFactor);
		sanCamera::setPosition(&currentCameraPosition);

		// ターゲット(中間点＋高さ)
		XMVECTOR midPoint = XMVectorScale(XMVectorAdd(playerPosition, bossPosition), 0.5f);
		midPoint = XMVectorAdd(midPoint, XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f)); // 少し上を注視
		sanCamera::setTarget(&midPoint);
	}
}

// 描画関数
void gameCamera::render()
{
}
#pragma once

class gameCamera
{
private:
	player* pPlayer; // プレイヤーのポインタ
	boss* pBoss;     // ボスのポインタ

	// カメラ距離・補間設定
	float cameraDistance;
	float rightOffset;
	float lerpFactor;

	float minHeight; // カメラの高さ下限
	float maxHeight; // カメラの高さ上限
	float heightFactor; // どのくらい高さを上げるかの係数

	bool isJustAvoidActive = false; // ジャスト回避

	XMVECTOR currentCameraPosition; // 前フレーム位置
	XMVECTOR currentCameraTarget;   // 前フレームターゲット

public:
	gameCamera();
	~gameCamera();
	void battleCamera();
	void justAvoidCamera();
	void setCharaPtr(player* pPlayer, boss* pBoss); // キャラのポインタ取得
};
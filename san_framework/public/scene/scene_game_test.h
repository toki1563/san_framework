#pragma once
#define FENCE_NUM (32)       // フェンスの数
#define FENCE_RADIUS (37.5f) // フェンスの半径
#define FIELD_RADIUS (36.5f) // フェンスの当たり判定を取る半径
// ゲームテスト

class SceneGameTest : public sanScene
{
	sanModel*	   pPlayer;
	sanModel*	   pNPC;
	sanModel*	   pDebugFan;
	sanModel*	   pGround;
	sanModel*	   pFence[FENCE_NUM];
	sanModel*	   pSky;
	sanModel*	   pCrown;
	sanBillboard*  pBillboard;
	sanEffect*	   pEffect;
	sanEffect*     pEffectNPC;
	sanEffect*     pHitEffect;

	// 極座標の情報
	float radius;  // 半径
	float theta;   // 平面角
	float phi;     // 仰角
	float value_r; // 回転
	float moveSpeed; // objectの移動スピード
	float radians;   // ラジアン
	float scrollMove; // 右クリック時のスクロール値
	int   shapeNumber;		// 形の番号

	bool  gridAxisActive;  // グリッド&軸表示非表示
	bool  isLightColorNormal; // ライトの色を通常と夕焼け変更可
	bool  input;			  // 入力があるかを示すフラグ

public:
	bool initialize();
	void terminate();
	void execute();
	void render();
};
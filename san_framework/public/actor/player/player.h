#pragma once
class boss; // 前方宣言

class playerStatus
{
public:
	float atkPower; // 攻撃力	
	float health;   // 体力
	float stamina;  // スタミナ

	// マックス値
	float maxAtkPower;
	float maxHealth;
	float maxStamina; 
};

class player : public cCharacter
{
private:
	sanModel* pShadow; // 影のモデル
	sanSound* pSe[3];  // SE
	boss* pBoss;       // ボスのポインタ

	//モーションの数
	int playerMotionNum;
	//読み込んだモーションデータ
	sanMotionData** playerMotion;

	//モーションファイル読み込み関数
	sanMotionData* loadMotionFile(const WCHAR* motFile);

	//回転補正
	void rotRoll(sanMotionData* p);

	float moveLimit; // 移動制限・移動とステップで使用
	float slowRate; // スローの際の処理倍率
	float oldSlowRate; // 変化する前のスロー処理倍率


	bool isDead; // 死んだとき
	bool isStep; // 回避したかどうか
	bool isCanAtk;	    // 攻撃可能どうか
	bool isTakeDamage;  // 攻撃を受けたかどうか
	bool isAtkCoolTime; // 攻撃のクールタイム
	bool isJustStepSuccess;   // ジャスト回避
	bool isTakeDamageDisPlay; // 攻撃受け時のプレイヤー表示

public:
	player(const WCHAR* folder, const WCHAR* boneFile);
	~player();
	

	void execute(); // 全体の処理
	void move();	   // プレイヤーの動き
	void atk();     // プレイヤーの攻撃
	void step();    // プレイヤーのステップ
	void JustStepAttack(); // ジャスト回避の特殊攻撃
	void takeDamage(float damage); // ダメージ受け時のプレイヤー表示
	void damageDisplay(); // 攻撃受け時の画面表示
	void playerAllRender(); // プレイヤーの影の表示
	void setBoss(boss* pBoss);
	bool getIsDead();    // 死んでいるかどうか
	bool getIsTakeHit(); // 攻撃を受けた時
	bool getTakeDamageDisPlay(); // 攻撃受けの画面表示取得
	float getCurrentTime(); // ゲーム開始から現在の時間を取得

	playerStatus status; // プレイヤーのステータス
};
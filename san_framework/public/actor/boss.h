#pragma once
class player; // 前方宣言

class bossStatus // ステータス
{
public:
	float atkPower; // 攻撃力	
	float health;   // 体力
	bool isDefense;  // 防御力

	// マックス値
	float maxAtkPower;
	float maxHealth;
};

enum class handleActionState
{
	Defending,  // 防御中
	Attacking,  // 攻撃中
	Moveing,	// 移動中
	MAX,		// 最大値
};

enum class attackKinds
{
	NormalAtk,     // 通常攻撃
	ContinuousAtk, // 連続攻撃
	RangeAtk,      // 範囲攻撃
	HeavyAtk,      // 重たい攻撃
	Max,			  // 最大値
};

class boss : public cCharacter
{
private:
	sanModel* pShadow; // 影のモデル
	sanSound* pSe[2]; // SE

	//モーションの数
	int bossMotionNum;
	//読み込んだモーションデータ
	sanMotionData** bossMotion;

	//モーションファイル読み込み関数
	sanMotionData* loadMotionFile(const WCHAR* motFile);

	//回転補正
	void rotRoll(sanMotionData* p);

	// アクション状態
	handleActionState handleAction;

	// 攻撃の種類
	attackKinds attackAction;
	attackKinds oldAttackAction; // 一つ前の攻撃情報の保持

	float atkProgress;  // 攻撃の進行度
	float slowRate; // スローの際の処理倍率
	float oldSlowRate; // 変化する前のスロー処理倍率

	bool isDead;        // 死んだとき
	bool isDefense;     // 防御しているかどうか
	bool hasHitFirst;   // 連続攻撃の初撃が当たったかどうか
	bool hasHitSecond;  // 連続攻撃の2連撃目が当たったかどうか
	bool isPlayerAtkRange; // プレイヤーに攻撃が当たるかどうか
	bool isPlayerJustStep; // プレイヤーにジャスト回避されたかどうか
	bool isPlayerJustStepPossible; // プレイヤーがジャスト回避可能かどうか
	bool isTakeDamage;     // 攻撃を受けたかどうか
	bool isTakeDamageDisPlay; // 攻撃受け時のプレイヤー表示

public:
	boss(const WCHAR* folder, const WCHAR* boneFile);
	~boss();

	void DecideNextAction(player* rival); // 次の行動
	void NextAttackAction(player* rival); // 次の攻撃
	void execute(player* rival); // 全体の処理
	void defense(player* rival); // 防御
	void normalAtk(player* rival);     // 通常攻撃
	void continuousAtk(player* rival); // 連続攻撃
	void rangeAtk(player* rival);      // 範囲攻撃
	void heavyAtk(player* rival);      // 重い攻撃
	void move(player* rival);    // プレイヤーに近づく
	void takeJustStep();         // ジャストで回避されたとき
	void takeDamage(float damage); // ダメージ受け時のプレイヤー表示
	void damageDisplay(); // 攻撃受け時の画面表示
	void playerAllRender(); // プレイヤーの影の表示
	bool playerCloseSearch(player* rival); // プレイヤーが近くにいるか判定
	bool getIsDead();    // 死んでいるかどうか
	bool getIsTakeHit(); // 攻撃を受けた時
	bool getIsDefense(); // 防御
	bool getPlayerJustStep(); // プレイヤーがジャスト回避時
	bool getPlayerJustStepPossible(); // プレイヤーがジャスト回避可能かどうか
	bool getPlayerAtkRange(); // 攻撃がプレイヤーに当たるかどうか
	bool getTakeDamageDisPlay(); // 攻撃受けの画面表示取得
	float getAtkProgress(); // 攻撃の進捗度
	float getCurrentHp(); // 現在のHP取得

	// UIで参照できるようにpublic
	bossStatus status;
};
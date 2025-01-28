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

class boss : public sanModel
{
public:
	boss(const WCHAR* folder, const WCHAR* file);
	~boss();

	void DecideNextAction(player* rival); // 次の行動
	void execute(player* rival); // 全体の処理
	void defense(player* rival); // 防御
	void atk(player* rival);     // 攻撃
	void move(player* rival);    // プレイヤーに近づく
	bool playerCloseSearch(player* rival); // プレイヤーが近くにいるか判定
	void takeDamage(float damage); // ダメージ受け時のプレイヤー表示
	void damageDisplay(); // 攻撃受け時の画面表示
	void playerAllRender(); // プレイヤーの影の表示
	bool getIsTakeHit(); // 攻撃を受けた時
	bool getIsDefense(); // 防御
	bool getTakeDamageDisPlay(); // 攻撃受けの画面表示取得
	handleActionState getBossAction(); // ボスの行動の取得

private:
	sanModel* pShadow; // 影のモデル
	sanModel* pRightArm;	// アニメーション用右腕
	sanModel* pRightArmAtkCoolTime; // 攻撃のクールタイム用
	sanSound* pSe[2]; // SE

	bossStatus status;
	handleActionState handleAction;

	float pi;           // 円周率
	bool isDefense;     // 防御しているかどうか
	bool isTakeDamage;  // 攻撃を受けたかどうか
	bool isTakeDamageDisPlay; // 攻撃受け時のプレイヤー表示
};
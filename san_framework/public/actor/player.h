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

class player : public sanModel
{
public:
	player(const WCHAR* folder, const WCHAR* filen);
	~player();

	void execute(boss* rival); // 全体の処理
	void move(boss* rival);	   // プレイヤーの動き
	void atk(boss* rival);     // プレイヤーの攻撃
	void step(boss* rival);    // プレイヤーのステップ
	void takeDamage(float damage); // ダメージ受け時のプレイヤー表示
	void damageDisplay(); // 攻撃受け時の画面表示
	void playerAllRender(); // プレイヤーの影の表示
	bool getIsTakeHit(); // 攻撃を受けた時
	bool getTakeDamageDisPlay(); // 攻撃受けの画面表示取得
	float getCurrentTime(); // ゲーム開始から現在の時間を取得

	// UIで参照できるようにpublic
	playerStatus status; // プレイヤーのステータス

private:
	sanModel* pShadow; // 影のモデル
	sanSound* pSe[3];  // SE

	float pi;     // 円周率
	bool isCanAtk;	  // 攻撃可能どうか
	bool isTakeDamage;   // 攻撃を受けたかどうか
	bool isAtkCoolTime; // 攻撃のクールタイム
	bool isTakeDamageDisPlay; // 攻撃受け時のプレイヤー表示
};
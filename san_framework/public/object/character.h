#pragma once

class cCharacter : public sanObject
{
private:

#pragma region Hierarchy	// 階層構造関連

	// 階層構造データ(boneファイル)
	// ※名前やポーズの初期値(バインドポーズ)等を参照するため保持し続ける
	sanModel_BoneData* pBoneData;

	// パーツの数
	int PartsNum;

	// パーツごとのオブジェクトポインタ
	// ※vnmファイル(ポリゴン)が存在しない階層もあり得るためvnObjectクラスのポインタで作成
	sanObject** pParts;

#pragma endregion


#pragma region Motion	// モーション関連

	float time;	// 経過時間
	float animSpeed; // アニメーション速度

	// 再生中のモーションデータ
	sanMotionData* pMotion;

#pragma endregion



public:
	cCharacter(const WCHAR* folder, const WCHAR* boneFile);
	~cCharacter();

	void execute();

	// バインドポーズ(キャラクター標準状態)に戻す
	void bindPose();

	// 再生するモーションを設定(NULLで未適用に戻す)
	void setMotion(sanMotionData* p);

	// 現在のモーションを取得
	sanMotionData* getMotion(void);

	// 再生位置(時間)を取得
	float getTime(void);

	// パーツ数を取得
	int getPartsNum(void);

	// パーツを番号で取得
	sanObject* getParts(int i);

	// パーツを名前で取得
	sanObject* getParts(char* name);

	void setAnimSpeed(int speed);

	// モーション終了フラグ
	bool isMotionEnded; 

};
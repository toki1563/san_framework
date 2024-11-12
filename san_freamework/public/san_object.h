// オブジェクト基底クラス
#include "../framework//san_modelData.h"

// ポリゴン頂点構造体
struct sanVertex3D
{
	float x, y, z;	  // 座標
	float nx, ny, nz; // 法線
	float r, g, b, a; // カラー
	float u, v;		  // UV
};

class sanObject
{
protected:
	// 移動値
	XMVECTOR	Position;

	// 回転値
	XMVECTOR	Rotation;

	// 拡大値
	XMVECTOR	Scale;

	// 変換行列
	XMVECTOR	Local;
	XMVECTOR	World;

	// 実行状態の管理
	bool executeEnable;

	// 描画状態の管理
	bool renderEnable;

	// ライティングの有効/無効
	bool lighting;

	// 半透明有効/無効
	bool transparent;

	// 階段構造の親
	sanObject* pParent;

	// マトリクスの計算(Local/World)
	void calculateLocalMatrix();
	void calculateWorldMatrix();


	
};
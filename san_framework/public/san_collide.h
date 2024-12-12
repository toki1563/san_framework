//--------------------------------------------------------------//
//	"san_collide.h"	    										//
//	     衝突座標インターフェース									//
//													2024/11/17	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

#define EPSION (1.0e-6) // 微小値(0.000001)
#define ISZERO(x) (fabsf(x) < EPSION) // マクロ(1行関数)

namespace sanCollide
{
	// 形状 : 直線
	struct stLine
	{
		XMVECTOR Pos;
		XMVECTOR Dir;

		// 2点から直線の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2);
	};

	// (s / (s - d))ここの値が0.1以上で当たり判定を作れる
	struct stSegment : public stLine
	{
		float Lenght;
	};

	//形状 : 平面
	struct stPlane
	{
		XMVECTOR Normal;
		float Distance;
		// 3点から平面の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	// 形状 : 三角形
	struct stTriangle
	{
		stPlane plane; // 内包
		XMVECTOR v[3];
		//  3点から平面の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	// 形状	: 球
	struct stSphere
	{
		XMVECTOR Pos;
		float Radius;
	};

	// 直線と点の最近棒距離を示す直線状の点を求める
	// [return] void
	// [Argument]
	// [out]NULL NG
	// [out]XMVECTOR* ret : 求める座標を格納するアドレス
	// [in] const stLine* line : 直線の情報
	// [in] const XMVECTOR* pos : 点の情報
	void closestPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos);

	// 直線と平面の交点を調べる
	// [return] bool true : 衝突している、 false : 衝突していない
	// [Argument]
	// [out]NULL NG
	// [out]XMVECTOR hit : 衝突座標を格納するアドレス
	// [in] const stLine* line : 直線の情報
	// [in] const stPlane* plane : 平面の情報
	bool isCollide(XMVECTOR* hit, const stLine* line, const stPlane* plane);

	// 直線と三角形の交点を調べる
	// [return] bool true : 衝突している、 false : 衝突していない
	// [Argument]
	// [out]NULL NG
	// [out]XMVECTOR *hit : 衝突座標を格納するアドレス
	// [in] const stLine* line : 直線の情報
	// [in] const stTriangle* tri : 三角形の座標
	bool isCollide(XMVECTOR* hit, const stLine* line, const stTriangle* tri);

	// 直線と三角形の交点を調べる
	// [return] int 交点の数
	// [Argument]
	// [out]NULL NG
	// [out]XMVECTOR *hit : 衝突座標を格納する配列の先頭アドレス
	// [in] const stLine* line : 直線の情報
	// [in] const stTriangle* tri : 三角形の座標
	int isCollide(XMVECTOR* hit, const stLine* line, const stSphere* sphere);

	// ベクトルがゼロベクトルかどうかの鑑定
	bool IsZeroVector(XMVECTOR v);
}
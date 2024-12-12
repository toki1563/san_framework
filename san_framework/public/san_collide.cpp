//--------------------------------------------------------------//
//	"san_collide.cpp"											//
//	     衝突座標インターフェース									//
//													2024/11/17	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/san_environment.h"
using namespace sanCollide;

// 2点から直線の情報の作成
void stLine::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2)
{
	Pos = *p1;
	Dir = *p2 - *p1;
	Dir = XMVector3Normalize(Dir);
}

// 3点から平面の情報を作成
void stPlane::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	XMVECTOR v, w;
	v = *p3 - *p1;
	w = *p2 - *p1;
	Normal = XMVector3Cross(v, w);
	Normal = XMVector3Normalize(Normal);
	Distance = -XMVectorGetX(XMVector3Dot(Normal, *p1));
}

// 3点から平面情報を作成
void stTriangle::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	plane.fromPoints(p1, p2, p3);
	v[0] = *p1;
	v[1] = *p2;
	v[2] = *p3;
}

// 直線と点の最近棒距離を示す直線状の点を求める
void sanCollide::closestPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos)
{
	if(ret == nullptr || line == nullptr || pos == nullptr) return;
	XMVECTOR v = *pos - line->Pos;
	// ゼロベクトルのチェック
	// float型はノイズが入るので絶対値で確認
	if (IsZeroVector(v))
	{
		// Line始点と点が一致
		*ret = line->Pos;
		return;
	}
	XMVECTOR vDot = XMVector3Dot(v, line->Dir);
	float dot = XMVectorGetX(vDot);

	*ret = line->Pos + line->Dir * dot;
}

// 直線と平面の交点を調べる
bool sanCollide::isCollide(XMVECTOR* hit, const stLine* line, const stPlane* plane)
{
	if(hit == nullptr || line == nullptr || plane == nullptr) return false;
	//直線と平面が平行か調べる(直線と平面の法線は垂直)
	float w = XMVectorGetX(XMVector3Dot(line->Dir, plane->Normal));
	if (ISZERO(w)) // 早期棄却
	{
		return false;
	}

	// 直線と平面の衝突座標を求める
	float d, s;
	XMVECTOR vS = XMVector3Dot(plane->Normal, line->Pos);
	s = XMVectorGetX(vS);
	s += plane->Distance;

	XMVECTOR vD = XMVector3Dot(plane->Normal, line->Pos + line->Dir);
	d = XMVectorGetX(vD);
	d += plane->Distance;

	if (ISZERO(s - d))
	{
		return false;
	}

	*hit = line->Pos + line->Dir * (s / (s - d));

	return true;
}

// 直線と三角形の交点を調べる
bool sanCollide::isCollide(XMVECTOR* hit, const stLine* line, const stTriangle* tri)
{
	if(hit == nullptr || line == nullptr || tri == nullptr) return false;

	if (isCollide(hit, line, &tri->plane) == false)
	{
		return false;
	}

	// 直線と平面の交点が三角形内にあるかチェック
	XMVECTOR e, h, c;
	float d;
	e = tri->v[1] - tri->v[0];
	h = *hit - tri->v[0];
	c = XMVector3Cross(e, h);
	d = XMVectorGetX(XMVector3Dot(c, tri->plane.Normal));
	if (d < 0.0f)	return false; // 三角形からはみ出ている
	e = tri->v[2] - tri->v[1];
	h = *hit - tri->v[1];
	c = XMVector3Cross(e, h);
	d = XMVectorGetX(XMVector3Dot(c, tri->plane.Normal));
	if (d < 0.0f)	return false; // 三角形からはみ出ている
	e = tri->v[0] - tri->v[2];
	h = *hit - tri->v[2];
	c = XMVector3Cross(e, h);
	d = XMVectorGetX(XMVector3Dot(c, tri->plane.Normal));
	if (d < 0.0f)	return false; // 三角形からはみ出ている

	// 3つの辺全てでははみ出しがなければ囲まれている状態
	return true;
}

// 直線と球の交点を調べる
int sanCollide::isCollide(XMVECTOR* hit, const stLine* line, const stSphere* sphere)
{
	if (hit == nullptr || line == nullptr || sphere == nullptr) return false;

	int n = 0; // 衝突座標の数
	XMVECTOR v = line->Pos - sphere->Pos;
	// ゼロベクトルのチェック
	// float型はノイズが入るので絶対値で確認
	if (IsZeroVector(v))
	{
		hit[n++] = line->Pos + line->Dir * sphere->Radius;
		hit[n++] = line->Pos - line->Dir * sphere->Radius;
		return n;
	}

	XMVECTOR nv = XMVector3Normalize(v);
	// カメラから球への方向ベクトルとの内積を計算
	XMVECTOR vdt = XMVector3Dot(nv, line->Dir);
	XMVECTOR vdl = XMVector3Length(v);
	// カメラから球中心までの距離
	float dt = XMVectorGetX(vdt);
	float dl = XMVectorGetX(vdl);
	if(dt >= 1.0f) dt = 1.0f;
	// 球中心と直線の距離
	float dr = dl * sqrt(1.0f - dt * dt);

	// 球に当たっているかつ前の情報より距離が近い場合は更新
	if (dr <= sphere->Radius)
	{
		// 球中心と直線の距離が半径以内なら衝突
		float dq = sqrtf(sphere->Radius * sphere->Radius - dr * dr);
		float ds = -dl * dt;
		float dp = ds + dq;
		float dm = ds - dq;

		hit[n++] = line->Pos + line->Dir * dm;
		if (ISZERO(dq) == false)
		{
			hit[n++] = line->Pos + line->Dir * dp;
		}
	}
	return 0;
}

bool sanCollide::IsZeroVector(XMVECTOR v)
{
	float vx = XMVectorGetX(v);
	float vy = XMVectorGetY(v);
	float vz = XMVectorGetZ(v);
	if (ISZERO(vx) && ISZERO(vy) && ISZERO(vz))
	{
		return true;
	}
	else
	{
		return false;
	}
}
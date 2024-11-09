//--------------------------------------------------------------//
//	"san_light.cpp"												//
//	     ライト													//
//													2024/11/09	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "san_environment.h"

XMVECTOR	sanLight::Direction;  // 並行光源の方向
XMVECTOR	sanLight::iDirection; // 平行光源の逆方向
XMVECTOR	sanLight::LightColor; // 平行光源の色 
XMVECTOR	sanLight::Ambient;    // アンビエント色

bool sanLight::initialize()
{
	Direction = XMVectorSet(-3.0f, -5.0f, -2.0f, 0.0f);

	iDirection = XMVector3Normalize(Direction); // 光源方向ベクトルの正規化
	iDirection = XMVectorNegate(iDirection);    // 光源方向ベクトルの反転

	LightColor = XMVectorSet(0.8f, 0.8f, 0.8f, 1.0f);
	Ambient = XMVectorSet(0.2f, 0.2f, 0.2f, 0.0f);

	return true;
}

void sanLight::terminate()
{
}

XMVECTOR *sanLight::getILightDir()
{
	iDirection = XMVector3Normalize(Direction);
	iDirection = XMVectorNegate(iDirection);
	return &iDirection;
}

XMVECTOR *sanLight::getLightColor()
{
	return &LightColor;
}

XMVECTOR *sanLight::getAmbient()
{
	return &Ambient;
}

// 取得

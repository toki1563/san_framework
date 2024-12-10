//--------------------------------------------------------------//
//	"san_light.cpp"												//
//	     ライト													//
//													2024/11/11	//
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
float sanLight::getLightDirX()
{
	return XMVectorGetX(Direction);
}

float sanLight::getLightDirY()
{
	return XMVectorGetY(Direction);
}

float sanLight::getLightDirZ()
{
	return XMVectorGetZ(Direction);
}

float sanLight::getLightColR()
{
	return XMVectorGetX(LightColor);
}

float sanLight::getLightColG()
{
	return XMVectorGetY(LightColor);
}

float sanLight::getLightColB()
{
	return XMVectorGetZ(LightColor);
}

float sanLight::getAmbientR()
{
	return XMVectorGetX(Ambient);
}

float sanLight::getAmbientG()
{
	return XMVectorGetY(Ambient);
}

float sanLight::getAmbientB()
{
	return XMVectorGetZ(Ambient);
}

// 設定
void sanLight::setLightDir(float x, float y, float z)
{
	Direction = XMVectorSet(x, y, z, 0.0f);
}

void sanLight::setLightDirX(float value)
{
	Direction = XMVectorSetX(Direction, value);
}

void sanLight::setLightDirY(float value)
{
	Direction = XMVectorSetY(Direction, value);
}

void sanLight::setLightDirZ(float value)
{
	Direction = XMVectorSetZ(Direction, value);
}

void sanLight::setLightCol(float r, float g, float b)
{
	LightColor = XMVectorSet(r, g, b, 1.0f);
}

void sanLight::setLightColR(float value)
{
	LightColor = XMVectorSetX(LightColor, value);
}

void sanLight::setLightColG(float value)
{
	LightColor = XMVectorSetY(LightColor, value);
}

void sanLight::setLightColB(float value)
{
	LightColor = XMVectorSetZ(LightColor, value);
}

void sanLight::setAmbient(float r, float g, float b)
{
	Ambient = XMVectorSet(r, g, b, 0.0f);
}

void sanLight::setAmbientR(float value)
{
	Ambient = XMVectorSetX(Ambient, value);
}

void sanLight::setAmbientG(float value)
{
	Ambient = XMVectorSetY(Ambient, value);
}

void sanLight::setAmbientB(float value)
{
	Ambient = XMVectorSetZ(Ambient, value);
}

// 加算
void sanLight::addLightDir(float x, float y, float z)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(x, y, z, 0.0f));
}

void sanLight::addLightDirX(float value)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanLight::addLightDirY(float value)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void sanLight::addLightDirZ(float value)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

void sanLight::addLightCol(float r, float g, float b)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(r, g, b, 0.0f));
}

void sanLight::addLightColR(float value)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanLight::addLightColG(float value)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void sanLight::addLightColB(float value)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

void sanLight::addAmbient(float r, float g, float b)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(r, g, b, 0.0f));
}

void sanLight::addAmbientR(float value)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanLight::addAmbientG(float value)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void sanLight::addAmbientB(float value)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}
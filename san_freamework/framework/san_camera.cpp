//--------------------------------------------------------------//
//	"san_camera.cpp"											//
//	     カメラ													//
//													2024/11/07	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "san_environment.h"

XMVECTOR		sanCamera::Position;
XMVECTOR		sanCamera::Target;
XMVECTOR		sanCamera::Up;

float			sanCamera::FovY;
float			sanCamera::Aspect;
float			sanCamera::zNear;
float			sanCamera::zFar;

XMMATRIX		sanCamera::View;
XMMATRIX		sanCamera::Proj;
XMMATRIX        sanCamera::Screen;

D3D11_VIEWPORT  sanCamera::Viewport;

bool sanCamera::initialize()
{
	Position = XMVectorSet(5.0f, 5.0f, -10.0f, 0.0f);
	Target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	FovY = XMConvertToRadians(45.0f); // 視野角
	Aspect = (float)sanMainFrame::screenWidth / (float)sanMainFrame::screenHeight;
	zNear = 1.0f;
	zFar = 1000.0f;

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = (float)sanMainFrame::screenWidth;
	Viewport.Height = (float)sanMainFrame::screenHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	return true;
}

void sanCamera::terminate()
{

}

XMMATRIX *sanCamera::getView()
{
	View = XMMatrixLookAtLH(Position, Target, Up);
	return &View;
}

XMMATRIX *sanCamera::getProj()
{
	Proj = XMMatrixPerspectiveFovLH(FovY, Aspect, zNear, zFar);
	return &Proj;
}

XMMATRIX *sanCamera::getScreen()
{
	Screen = XMMatrixMultiply(*getView(), *getProj());
	return &Screen;
}

D3D11_VIEWPORT *sanCamera::getViewport()
{
	return &Viewport;
}

// 取得
const XMVECTOR *sanCamera::getPosition()
{
	return &Position;
}

float sanCamera::getPositionX()
{
	return XMVectorGetX(Position);
}

float sanCamera::getPositionY()
{
	return XMVectorGetY(Position);
}

float sanCamera::getPositionZ()
{
	return XMVectorGetZ(Position);
}

const XMVECTOR *sanCamera::getTarget()
{
	return &Target;
}

float sanCamera::getTargetX()
{
	return XMVectorGetX(Target);
}

float sanCamera::getTargetY()
{
	return XMVectorGetY(Target);
}

float sanCamera::getTargetZ()
{
	return XMVectorGetZ(Target);
}

const XMVECTOR *sanCamera::getUp()
{
	return &Up;
}

float sanCamera::getUpX()
{
	return XMVectorGetX(Up);
}

float sanCamera::getUpY()
{
	return XMVectorGetY(Up);
}

float sanCamera::getUpZ()
{
	return XMVectorGetZ(Up);
}

float sanCamera::getFovY()
{
	return FovY;
}

float sanCamera::getAspect()
{
	return Aspect;
}

// 設定
void sanCamera::setPosition(float x, float y, float z)
{
	Position = XMVectorSet(x, y, z, 0.0f);
}

void sanCamera::setPosition(const XMVECTOR *v)
{
	Position = *v;
}

void sanCamera::setPositionX(float value)
{
	Position = XMVectorSetX(Position, value);
}

void sanCamera::setPositionY(float value)
{
	Position = XMVectorSetY(Position, value);
}

void sanCamera::setPositionZ(float value)
{
	Position = XMVectorSetZ(Position, value);
}

void sanCamera::setTarget(float x, float y, float z)
{
	Target = XMVectorSet(x, y, z, 0.0f);
}

void sanCamera::setTarget(const XMVECTOR *v)
{
	Target = *v;
}

void sanCamera::setTargetX(float value)
{
	Target = XMVectorSetX(Target, value);
}

void sanCamera::setTargetY(float value)
{
	Target = XMVectorSetY(Target, value);
}

void sanCamera::setTargetZ(float value)
{
	Target = XMVectorSetZ(Target, value);
}

void sanCamera::setUp(float x, float y, float z)
{
	Up = XMVectorSet(x, y, z, 0.0f);
}

void sanCamera::setUp(const XMVECTOR *v)
{
	Up = *v;
}

void sanCamera::setUpX(float value)
{
	Up = XMVectorSetX(Up, value);
}

void sanCamera::setUpY(float value)
{
	Up = XMVectorSetY(Up, value);
}

void sanCamera::setUpZ(float value)
{
	Up = XMVectorSetZ(Up, value);
}

void sanCamera::setFovY(float value)
{
	FovY = value;
}

void sanCamera::setAspect(float value)
{
	Aspect = value;
}

// 加算
void sanCamera::addPosition(float x, float y, float z)
{
	Position = XMVectorAdd(Position, XMVectorSet(x, y, z, 0.0f));
}

void sanCamera::addPosition(const XMVECTOR *v)
{
	Position += *v;
}

void sanCamera::addPositionX(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanCamera::addPositionY(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, value,  0.0f, 0.0f));
}

void sanCamera::addPositionZ(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, 0.0f, value,  0.0f));
}

void sanCamera::addTarget(float x, float y, float z)
{
	Target = XMVectorAdd(Target, XMVectorSet(x, y, z, 0.0f));
}

void sanCamera::addTarget(const XMVECTOR *v)
{
	Target += *v;
}

void sanCamera::addTargetX(float value)
{
	Target = XMVectorAdd(Target, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanCamera::addTargetY(float value)
{
	Target = XMVectorAdd(Target, XMVectorSet(0.0f, value,  0.0f, 0.0f));
}

void sanCamera::addTargetZ(float value)
{
	Target = XMVectorAdd(Target, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

void sanCamera::addFovY(float value)
{
	FovY += value;
}

void sanCamera::addAspect(float value)
{
	Aspect += value;
}
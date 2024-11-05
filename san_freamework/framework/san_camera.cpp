// カメラ
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

XMMATRIX* sanCamera::getView()
{
	View = XMMatrixLookAtLH(Position, Target, Up);
	return &View;
}

XMMATRIX *sanCamera::getProj()
{
	Proj = XMMatrixPerspectiveFovLH(FovY, Aspect, zNear, zFar);
	return &Proj;
}

XMMATRIX* sanCamera::getScreen()
{
	Screen = XMMatrixMultiply(*getView(), *getProj());
	return &Screen;
}

D3D11_VIEWPORT* sanCamera::getViewport()
{
	return &Viewport;
}


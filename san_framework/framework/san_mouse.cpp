//--------------------------------------------------------------//
//	"san_Mouse.cpp"												//
//	     マウス入力												//
//													2024/10/28	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "san_environment.h"

int	sanMouse::x = 0;
int	sanMouse::y = 0;
int	sanMouse::r = 0;
int	sanMouse::dx = 0;
int sanMouse::dy = 0;
int	sanMouse::On = 0;
int	sanMouse::Trg = 0;
int	sanMouse::Rel = 0;

int sanMouse::getX(void)  { return x; }
int sanMouse::getY(void)  { return y; }
int sanMouse::getR(void)  { return r; }
int sanMouse::getDX(void) { return dx; }
int sanMouse::getDY(void) { return dy; }
bool sanMouse::on(eButton btn) { return btn & On ? true : false; }
bool sanMouse::trg(eButton btn) { return btn & Trg ? true : false; }
bool sanMouse::rel(eButton btn) { return btn & Rel ? true : false; }

void sanMouse::setMouseStatus(void)
{
	x = sanDirectInput::getMouseX();
	y = sanDirectInput::getMouseY();
	r = sanDirectInput::getMouseState()->lZ;
	dx = sanDirectInput::getMouseState()->lX;
	dy = sanDirectInput::getMouseState()->lY;
	int Old = On;
	On = 0;
	if (sanDirectInput::getMouseState()->rgbButtons[0])	On |= LT;
	if (sanDirectInput::getMouseState()->rgbButtons[1])	On |= RT;
	if (sanDirectInput::getMouseState()->rgbButtons[2]) On |= MD;

	Trg = (On ^ Old) & On;
	Rel = (On ^ Old) & Old;
}
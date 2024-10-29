// フォント描画
#include "../framework.h"
#include "san_environment.h"

WCHAR sanFont::fontName[256][64];
int sanFont::fontNum = 0;

ID2D1SolidColorBrush	*sanFont::pBruch = NULL;
IDWriteTextFormat	    *sanFont::pTextFormat = NULL;
D2D1_RECT_F			    sanFont::rect;

float	sanFont::Size = 16.0f;
float   sanFont::X = 0.0f; 
float   sanFont::Y = 0.0f;
DWORD	sanFont::Color = 0xffffffff;

sanFont::stStringInfo	sanFont::strInfo[sanFontSTRING_MAX];
int		sanFont::registerNum = 0;
int		sanFont::registerCount = 0;
WCHAR	*sanFont::allocPtr = NULL;
WCHAR	*sanFont::currentPtr = NULL;

IDWriteTextFormat* sanFont::currentTextFormat = NULL;

bool sanFont::initialize()
{
	registerNum = 0;
	registerCount = 0;
	allocPtr = new WCHAR[sanFontCHARACTOR_MAX];
	currentPtr = allocPtr;

	HRESULT hr;

	// 使用できるフォント名の取得
	memset(fontName, 0, sizeof(fontName));
	IDWriteFontCollection* fonts = NULL;
	hr = sanDirect2D::getDWFactory()->GetSystemFontCollection(&fonts);
	if (hr == S_OK)
	{
		WCHAR locale[LOCALE_NAME_MAX_LENGTH];
		GetUserDefaultLocaleName(locale, _countof(locale));
		unsigned int const count = fonts->GetFontFamilyCount();
	}
}
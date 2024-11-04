//--------------------------------------------------------------//
//	"san_font.cpp"												//
//	     フォント描画												//
//													2024/11/01	//
//														sanda	//
//--------------------------------------------------------------//
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
	// 初期化
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
		// ロケール情報取得
		WCHAR locale[LOCALE_NAME_MAX_LENGTH];
		GetUserDefaultLocaleName(locale, _countof(locale));
		unsigned int const count = fonts->GetFontFamilyCount();

		// フォントファミリーの設定
		for (unsigned int familyIndex = 0; familyIndex != count && fontNum < sanFontNUM_MAX; familyIndex++)
		{
			IDWriteFontFamily* family = NULL;
			fonts->GetFontFamily(familyIndex, &family);
			IDWriteLocalizedStrings* names = NULL;
			hr = family->GetFamilyNames(&names);
			if (hr != S_OK)	continue;
			unsigned int nameIndex = 0;
			BOOL exists = false;
			hr = names->FindLocaleName(locale, &nameIndex, &exists);
			if (hr == S_OK && exists == TRUE)
			{
				hr = names->GetString(nameIndex, fontName[fontNum], _countof(fontName[fontNum]));
				fontNum++;
			}
			family->Release();
			names->Release();
		}
		fonts->Release();
	}

	// 白色ブラシの生成
	sanDirect2D::getDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBruch);
	if (hr != S_OK)
	{
		return false;
	}

	// フォントの作成
	pTextFormat = create(L"Meiryo", (int)Size);
	if (pTextFormat == NULL)
	{
		return false;
	}
	currentTextFormat = pTextFormat;

	rect.left = 0.0f;
	rect.top = 0.0f;
	rect.right = (float)sanMainFrame::screenWidth;
	rect.bottom = (float)sanMainFrame::screenHeight;

	return true;
}

void sanFont::terminate()
{
	delete[] allocPtr;
	SAFE_RELEASE(pTextFormat);
	SAFE_RELEASE(pBruch);
}

// 文字列をフォント用バッファに登録し、描画情報として管理
void sanFont::registerString(const WCHAR *string, UINT32 count)
{
	// 警告を一時的に無効化
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
	if (string == NULL) return;
	if (registerNum >= sanFontSTRING_MAX) return;
	if (registerCount + count >= sanFontCHARACTOR_MAX) return;
	// 情報設定
	strInfo[registerNum].x = X;
	strInfo[registerNum].y = Y;
	strInfo[registerNum].color = Color;
	strInfo[registerNum].ptr = currentPtr;
	strInfo[registerNum].count = count;
	strInfo[registerNum].textFormat = currentTextFormat;
	// 文字列をコピー
	wcsncpy_s((WCHAR*)currentPtr, sanFontCHARACTOR_MAX - (reinterpret_cast<int>(currentPtr) - reinterpret_cast<int>(allocPtr)), string, count);
	// ポインタとカウンタの更新
	currentPtr += count;
	registerNum++;
	registerCount += count;
	// 警告のリセット
#pragma warning(pop)
}

// registerStringで登録したテキスト情報をDirect2DのAPIを使用して画面に描画する処理
void sanFont::renderString()
{
	if (pTextFormat == NULL) return;

#if 1
	static float reciprocal = 1.0f / 255.0f;
	for (int i = 0; i < registerNum; i++)
	{
		// 描画の位置の設定
		rect.left = strInfo[i].x;
		rect.top = strInfo[i].y;

		// 描画色の設定
		if (!i || Color != strInfo[i].color)
		{
			Color = strInfo[i].color;

			D2D1_COLOR_F col;
			col.r = (float)((Color & 0x00ff0000) >> 16) * reciprocal;
			col.g = (float)((Color & 0x0000ff00) >>  8) * reciprocal;
			col.b = (float)((Color & 0x000000ff) >>  0) * reciprocal;
			col.a = (float)((Color & 0xff000000) >> 24) * reciprocal;
			pBruch->SetColor(&col);
		}

		// テキストの描画
		sanDirect2D::getDeviceContext()->DrawTextW(
			strInfo[i].ptr,
			strInfo[i].count,
			strInfo[i].textFormat,
			&rect,
			pBruch
		);
	}
#endif
	registerNum = 0;
	registerCount = 0;
	currentPtr = allocPtr;
	return;
}

// 使用できるフォントの数の取得
int sanFont::getFontNum(void)
{
	return fontNum;
}

// 使用できるフォント名の取得
WCHAR* sanFont::getFontName(int id)
{
	if (id < 0 || id >= fontNum) return NULL;
	return fontName[id];
}

// フォントの作成
IDWriteTextFormat* sanFont::create(const WCHAR* fontname, int size)
{
	HRESULT hr;
	IDWriteTextFormat* ret = NULL;

	// テキストフォーマットの生成
	hr = sanDirect2D::getDWFactory()->CreateTextFormat(
		fontname,  // フォントファミリの名前を含む文字配列
		NULL,      // フォントコレクションオブジェクトへのポインター。これがNULLの場合、システムフォントコレクションを示す。
		DWRITE_FONT_WEIGHT_NORMAL,   // オブジェクトのフォントの太さを示す値
		DWRITE_FONT_STYLE_NORMAL,	 // オブジェクトのフォントスタイルを示す値
		DWRITE_FONT_STRETCH_NORMAL,  // オブジェクトのフォント伸縮を示す値
		(float)size,  // DIP("デバイス非依存ピクセル")単位でのフォントの論理サイズ。「1DIPは1/96インチ」
		L"",		  // ロケール名を含む文字配列
		&ret		  // このメソッドが返されるときに、新しく作成されたテキスト形式オブジェクトへのポインターのアドレスが格納される。失敗した場合はNULLが格納される
	);
	return ret;
}

void sanFont::setPos(float x, float y)
{
	X = x;
	y = y;
}

void sanFont::setPosX(float x)
{
	X = x;
}

void sanFont::setPosY(float y)
{
	Y = y;
}

void sanFont::setColor(DWORD color)
{
	Color = color;
}

void sanFont::setTextFormat(IDWriteTextFormat* tf)
{
	currentTextFormat = (tf == NULL ? pTextFormat : tf);
}

float sanFont::getPosX()
{
	return X;
}

float sanFont::getPosY()
{
	return Y;
}

DWORD sanFont::getColor()
{
	return Color;
}

// 文字列の描画(ワイド文字)
void sanFont::print(float x, float y, DWORD color, const WCHAR *string, ...)
{
	if (string == NULL) return;

	X = x;
	Y = y;
	Color = color;

	va_list va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

void sanFont::print(float x, float y, const WCHAR *string, ...)
{
	if (string == NULL) return;

	X = x;
	Y = y;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

void sanFont::print(const WCHAR *string, ...)
{
	if (string == NULL) return;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

// 文字列の描画(ワイド文字/文字数指定付き)
void sanFont::printCount(float x, float y, DWORD color, int count, const WCHAR *string, ...)
{
	if (string == NULL) return;
	X = x;
	Y = y;
	Color = color;

	va_list va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)count);
}

void sanFont::printCount(float x, float y, int count, const WCHAR *string, ...)
{
	if (string == NULL)return;

	X = x;
	Y = y;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)count);
}

void sanFont::printCount(int count, const WCHAR *string, ...)
{
	if (string == NULL)return;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)count);
}

// 標準出力への文字列描画(ワイド文字)(ワイド文字)
void sanFont::output(const WCHAR *string, ...)
{
	if (string == NULL)return;

	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	OutputDebugStringW(buf);
}
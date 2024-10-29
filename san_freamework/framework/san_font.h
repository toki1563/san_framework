// フォント描画
#pragma once

#define sanFontSTRING_MAX	  (0x0400)	// 1フレームで実行できるrenderの数
#define sanFontCHARACTOR_MAX  (0X2000)	// 1フレーム描画できる文字の数

#define sanFontNUM_MAX		  (256)     // 確保しておける使用可能フォント名の最大数

class sanFont
{
private:
	static WCHAR  fontName[sanFontNUM_MAX][64];
	static int	  fontNum;

	static ID2D1SolidColorBrush  *pBruch;
	static IDWriteTextFormat     *pTextFormat;
	static D2D1_RECT_F			 rect;

	static float	Size;
	static float	X;
	static float	Y;
	static DWORD	Color;

	struct stStringInfo
	{
		float x;
		float y;
		DWORD color;
		WCHAR *ptr;
		UINT32 count;
		IDWriteTextFormat* textFormat;
	};
	static stStringInfo strInfo[sanFontSTRING_MAX];
	static int			registerNum;
	static int			registerCount;
	static WCHAR		*allocPtr;
	static WCHAR		*currentPtr;

	static IDWriteTextFormat  *currentTextFormat;

	static void registerString(const WCHAR *string, UINT32 count);

public:
	// システム管理
	static bool initialize();
	static void terminate();
	static void renderString();

	// 使用できるフォント数の取得
	static int getFontNum(void);

	// 使用できるフォント名の取得
	static WCHAR *getFontName(int id);

	// フォントの作成
	static IDWriteTextFormat *create(const WCHAR *fontName, int size);

	// 各種設定
	static void setPos(float x, float y);
	static void setPosX(float x);
	static void setPosY(float y);
	static void setColor(DWORD color);

	// 各種取得
	static float getPosX();
	static float getPosY();
	static DWORD getColor();
	static void setTextFormat(IDWriteTextFormat* tf = NULL);

	// 文字列の描画(ワイド文字)
	static void print(float x, float y, DWORD color, const WCHAR* string, ...);
	static void print(float x, float y, const WCHAR* string, ...);
	static void print(const WCHAR* string, ...);

	// 文字列の描画(ワイド文字/文字数指定付き)
	static void printCount(float x, float y, DWORD color, int count, const WCHAR* string, ...);
	static void printCount(float x, float y, int count, const WCHAR* string, ...);
	static void printCount(int count, const WCHAR* string, ...);

	// 標準出力への文字列描画(ワイド文字)
	static void output(const WCHAR *string, ...);
};

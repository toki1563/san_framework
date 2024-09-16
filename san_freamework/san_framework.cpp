#include <windows.h>
#include "framework.h"
#include "framework/san_environment.h"
#ifdef _DEBUG
#include <iostream>
#endif

using namespace std;

// @brief コンソール画面にフォーマット付き文字列を表示
// @param formatフォーマット(%dとか%fとかの)
// @parma 可変長引数
// @remarks この関数はデバッグ用です。デバッグ時にしか動作しません。

void DebugOutputFormatString(const char* format, ...)
{
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	printf(format, valist);
	va_end(valist);
#endif
}

// ウィンドウプロシージャ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ウィンドウが破棄されたら呼ばれる
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0); // OSに終わりと伝える
		return 0;
	}
	if (wParam == VK_ESCAPE)	//ESCキー
	{
		PostQuitMessage(0);		//アプリケーションを終了する
	}
	return DefWindowProc(hwnd, msg, wParam, lParam); // 既定の処理を行う
}


#ifdef _DEBUG
int main()
{
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif
	DebugOutputFormatString("Show window test.");
	getchar();

	// ウィンドクラスの生成&登録
	WNDCLASSEX w = {};

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure; // コールバック関数の指定
	w.lpszClassName = (L"san_framework"); // アプリケーション名
	w.hInstance = GetModuleHandle(nullptr); // ハンドルの取得
	RegisterClassEx(&w); // アプリケーションクラス(ウィンドウクラスの指定をOSに伝える)
	RECT wrc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // ウィンドウサイズ
	// 関数を使ってウィンドウサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクト生成
	HWND hwnd = CreateWindow(w.lpszClassName, // クラス名指定
		(L"san_framework"), // タイトルバー文字
		WS_OVERLAPPEDWINDOW, // タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT, // 表示ｘ座標はOSにお任せ
		CW_USEDEFAULT, // 表示ｘ座標はOSにお任せ
		wrc.right - wrc.left, // ウィンドウ幅
		wrc.bottom - wrc.top, // ウィンドウ高
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // 追加パラメーター

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	MSG msg = {};
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// アプリケーションが終わるときにmessageがWM_QUITになる
		if (msg.message == WM_QUIT)
		{
			break;
		}
	}

	// もう使わないので登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
// san_framework.cpp : アプリケーションのエントリ ポイントを定義します。
#include "framework.h"
#include "resource.h"
#include "framework/san_environment.h"

#define MAX_LOADSTRING 100

// グローバル変数
HINSTANCE hInst;                     // 現在のインターフェース
WCHAR szTitle[MAX_LOADSTRING];       // タイトルバーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING]; // メインウィンドウクラス名
HWND hWnd = NULL;					 // ウィンドウハンドル

// クライアント領域のサイズ
int screen_width = SCREEN_WIDTH;
int screen_height = SCREEN_HEIGHT;

// このコードモジュールに含まれる関数の宣言を転送します
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// エントリーポイント
int APIENTRY wWinMain(_In_ HINSTANCE hInsrance,
	_In_opt_ HINSTANCE hPrevInstance, // このアプリケーションが既に実行中かどうかを示す
	_In_ LPWSTR lpCmdLine,			  // コマンドライン引数
	_In_ int nCmdShow)				  // ウィンドウ表示状態(最大化・最小化など)
{
	//=============================================================================================
}

// ウィンドウクラスを登録(設定)
ATOM MyMegisterClass(HINSTANCE hInstance)
{
	//=============================================================================================
}

// ウィンドウの初期化
// インスタンスハンドルを保存して、メインウィンドウを作成
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//=============================================================================================
}

// ウィンドウプロシージャ
LRESULT WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
#if 0 // メッセージ確認用
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ISM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
#endif
	case WM_MOUSEMOVE: // マウスの座標の設定
		//========================================================================================
		break;
	case WM_KEYDOWN:   // キーが押された
		// ウィンドウが破棄されたら呼ばれる
		if (wParam == VK_ESCAPE)	//ESCキー
		{
			PostQuitMessage(0);		//アプリケーションを終了する
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0); // OSに終わりと伝える
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // 既定の処理を行う
	}
	return 0;
}

#if 0
// バージョン情報ボックスのメッセージ ハンドラー
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
#endif
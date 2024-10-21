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
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// アプリケーション起動前に必要な初期設定

	// グローバル文字列を初期化する
	LoadStringW(hInsrance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInsrance, IDC_SANFRAMEWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInsrance);

	// アプリケーション初期化の実行
	if (!InitInstance(hInsrance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInsrance, MAKEINTRESOURCE(IDC_SANFRAMEWORK));
	MSG msg;

	// ロケールの設定(日本)
	setlocale(LC_CTYPE, "JPN");

	// メインフレームの初期化
	sanMainFrame::initialize(hWnd, hInst, screen_width, screen_height);

	// メインメッセージループ
	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// アプリが終わるまで
			if (msg.message == WM_QUIT)
			{
				goto END;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// メインフレーム実行
		sanMainFrame::execute();
	}

	END:
		// メインフレームの終了
		sanMainFrame::terminate();
		return(int)msg.wParam;
}

// ウィンドウクラスを登録(設定)
ATOM MyMegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style		   = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WindowProcedure;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SANFRAMEWORK));
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = NULL; // MAKEINTRESOURCEW(IDC_SANFRAMEWORK)
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

// ウィンドウの初期化
// インスタンスハンドルを保存して、メインウィンドウを作成
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンスハンドルを格納

	// ウィンドウ生成
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU, // WS_OVERLAPPEDWINDOW,
		110, 50, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	
	// クライアント領域が希望のサイズになるようにウィンドウ全体のサイズを計算して再設定
	RECT rw, rc;
	::GetWindowRect(hWnd, &rw); // ウィンドウ全体のサイズ
	::GetClientRect(hWnd, &rc); // クライアント領域のサイズ

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + screen_width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + screen_height;

	::SetWindowPos(hWnd, nullptr, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);

	// ウィンドウの表示と更新
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// WM_PAINTを発生させないようにする
	ValidateRect(hWnd, 0);

	return TRUE;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
/****************************************************************************************************
//
//		main.cpp
//
//
//		@brief	WinMainの定義
//		@author	Yasushi Hirakawa
//
****************************************************************************************************/

#include "MainLoop.h"

//---------------------------------------------------------------------------------------------
//	const
//---------------------------------------------------------------------------------------------
const wchar_t*	kWindowName = L"Test";

//---------------------------------------------------------------------------------------------
//	Global Member
//---------------------------------------------------------------------------------------------
HWND		g_hWnd = NULL;			// ウィンドウハンドル
MainLoop	g_MainLoop;				// メインループ

//---------------------------------------------------------------------------------------------
//	プロトタイプ宣言
//---------------------------------------------------------------------------------------------
bool	Init(HINSTANCE hInst);		// 初期化
void	Destroy();					// 破棄
void	Loop();						// ループ	

//---------------------------------------------------------------------------------------------
//	Method
//---------------------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------------------
//	@brief	ウィンドウコールバック
//
//	@param	hWnd		ウィンドウハンドル
//	@param	msg			メッセージ
//	@param	wParam		メッセージに関する追加情報
//	@param	lParam		メッセージに関する追加情報
---------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, unsigned int msg, unsigned int wParam, LONG lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		// ゲーム及び各種ライブラリ破棄
		Destroy();
		// 終了メッセージ
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}

	default: break;
	}

	// 入力コールバックを渡す
	HirakawaLib::SetWindowInputCallback(hWnd, msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*---------------------------------------------------------------------------------------------
//	@brief	WinMain
//
//	@param	hInst		インスタンスハンドル
//	@param	hPrevInst	以前のインスタンスハンドル
//	@param	lpCmdLine	コマンドライン
//	@param	nCmdShow	表示状態
---------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG
	// メモリリーク検出用フラグ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	// 初期化
	if (Init(hInst)) {
		// ウインドウ表示
		ShowWindow(g_hWnd, SW_SHOWNORMAL);
		UpdateWindow(g_hWnd);

		// ループ
		Loop();

		// クラスレジスタの解放
		UnregisterClass(kWindowName, hInst);
	}

	return  true;
}

/*---------------------------------------------------------------------------------------------
//	@brief	初期化
//	@param	hInst	アプリケーションハンドル
//	@retval	true	：成功
//	@retval	false	：失敗
---------------------------------------------------------------------------------------------*/
bool Init(HINSTANCE hInst)
{
	bool	ret = true;

	do {

		WNDCLASSEX wc = {
			sizeof(WNDCLASSEX),	// 構造体のサイズ．sizeof(WNDCLASSEX)
			CS_CLASSDC,			// ウインドウスタイル
			WndProc,			// ウインドウのメッセージを処理するコールバック関数へのポインタ
			0,					// ウインドウクラス構造体の後ろに割り当てる補足バイト数 普通0
			0,					// ウインドウインスタンスの後ろに割り当てる補足バイト数 普通0
			hInst,				// ウィンドウプロシージャーのインスタンスハンドル
			NULL,				// アイコンのハンドル
			NULL,				// マウスカーソルのハンドル
			NULL,				// ウインドウ背景色
			NULL,				// デフォルトメニュー名
			kWindowName,		// このウインドウクラスにつける名前
			NULL };				// 16×16の小さいサイズのアイコン
		if (!RegisterClassEx(&wc)) {
			ret = false;
			break;
		}

		// ウィンドウ生成
		g_hWnd = CreateWindowEx(
			0,							// 拡張ウィンドウスタイル
			kWindowName,				// 登録されているクラス名
			kWindowName,				// ウィンドウ名
			WS_OVERLAPPED | WS_SYSMENU,	// ウィンドウスタイル
			CW_USEDEFAULT,				// ウィンドウの横方向の位置
			CW_USEDEFAULT,				// ウィンドウの縦方向の位置
			kWindowWidth,				// ウィンドウの幅
			kWindowHeight,				// ウィンドウの高さ
			NULL,						// 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,						// メニューハンドルまたは子識別子
			hInst,						// アプリケーションインスタンスのハンドル
			NULL);						// ウィンドウ作成データ
		if (g_hWnd == NULL) {
			ret = false;
			break;
		}

		// カーソルの設定
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		// ウィンドウサイズを再設定
		RECT rw, rc;
		::GetWindowRect(g_hWnd, &rw);
		::GetClientRect(g_hWnd, &rc);
		int new_width = (rw.right - rw.left) - (rc.right - rc.left) + kWindowWidth;
		int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + kWindowHeight;
		::SetWindowPos(g_hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);

		// KoudoLibの初期化
		if (HirakawaLib::Init(g_hWnd) == false) {
			ret = false;
			break;
		}

	} while (0);

	return ret;
}

/*---------------------------------------------------------------------------------------------
//	@brief	破棄関数
---------------------------------------------------------------------------------------------*/
void Destroy()
{
	g_MainLoop.Destroy();
	HirakawaLib::Destroy();
}

/*---------------------------------------------------------------------------------------------
//	@brief	メインループ
---------------------------------------------------------------------------------------------*/
void Loop()
{
	MSG			msg;			// メッセージ管理
	ZeroMemory(&msg, sizeof(msg));

	// メッセージ・ループ
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (HirakawaLib::Update()) {
				g_MainLoop.Update();
			}
		}
	}
}

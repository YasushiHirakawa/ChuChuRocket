/****************************************************************************************************
//
//		HLCommon.h
//
//		@brief	KoudoLib 共通定義
//		@author	Yasushi Hirakawa
//		@date	2016/08/23
//
****************************************************************************************************/

#ifndef __HLCOMMON_H__
#define __HLCOMMON_H__

//---------------------------------------------------------------------------------------------
//	Library
//---------------------------------------------------------------------------------------------

// DirectX
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

//---------------------------------------------------------------------------------------------
//	include
//---------------------------------------------------------------------------------------------
// メモリリーク検知定義
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#endif

// Ｃライブラリ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <direct.h>
#include <mmsystem.h>
#include <time.h>
#include <fstream>

// DirectX
#include <d3d11.h>

// DirectXTK
#include "CommonStates.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Audio.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------------------------------------------
//	const
//---------------------------------------------------------------------------------------------
const int	kStrMax = 128;				// 最大文字列数
const unsigned int	kWindowWidth	= 1280;		// ウィンドウの横幅
const unsigned int	kWindowHeight	= 720;		// ウィンドウの縦幅


//---------------------------------------------------------------------------------------------
//	define
//---------------------------------------------------------------------------------------------
#define SAFE_RELEASE(x) if(x)  { x->Release(); x = NULL; }
#define SAFE_DELETE(x) if(x)  { delete x; x = NULL; }


//---------------------------------------------------------------------------------------------
//	class
//---------------------------------------------------------------------------------------------
class HLCommon
{
public:
	static void MultibyteToWide(const char* mbStr, wchar_t* wStr);	// マルチバイト文字列をワイド文字列に変換
	static void SetWindowHandle(HWND hWnd);							// ウィンドウハンドルの設定
	
	static HWND GetWindowHandle() { return window_handle_; }		// ウィンドウハンドルの取得
	static wchar_t* GetWindowName() { return window_name_; }			// ウィンドウ名の取得

private:
	static wchar_t			window_name_[kStrMax];	// ウィンドウ名
	static HWND				window_handle_;			// ウィンドウハンドル
};

#endif //__HLCOMMON_H__

// フレームワーク共通環境ヘッダー
#pragma once

// ライブラリ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "winmm.lib")

// 共通ヘッダファイルのインクルード
#include <stdio.h>
#include <assert.h>
#include <mmsystem.h>
#include <locale.h>
#include <time.h>

// DirectXヘッダー
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>
#include <xaudio2.h>

#include <DirectXMath.h>

#include "../DirectXTex/DirectXTex.h"

// ネームスペース
using namespace DirectX;

// 画面の大きさ
#define SCREEN_WIDTH (1280) 
#define SCREEN_HEIGHT (720)

// リリース
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}

// フレームワークヘッダー
#include "directX/san_Direct2D.h"
#include "directX/san_Direct3D.h"
#include "directX/san_DirectInput.h"
#include "directX/san_shader.h"
#include "directX/san_XAudio2.h" 
#include "directX/san_XInput.h"

#include "san_camera.h"
#include "san_mouse.h"

#include "san_font.h"

#include "../public/scene/san_scene.h"
#include "../public/scene/scene_table.h"

#include "san_mainframe.h"
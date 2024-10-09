// フレームワーク共通環境ヘッダー
#pragma once

// ライブラリ
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// 共通ヘッダファイルのインクルード
#include <stdio.h>
#include <assert.h>
#include <mmsystem.h>
#include <locale.h>
#include <time.h>

// DirectXヘッダー
#include <d3d12.h>
#include <dxgi1_6.h>

// 画面の大きさ
#define SCREEN_WIDTH (1280) 
#define SCREEN_HEIGHT (720)

// フレームワークヘッダー
#include "directX/san_Direct3D.h"
#include "../public/scene/san_scene.h"

#include "../public/scene/san_scene.h"
#include "../public/scene/scene_table.h"

#include "san_mainframe.h"
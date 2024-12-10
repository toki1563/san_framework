// メインフレーム
#include "../framework.h"
#include "san_environment.h"

#include <iostream>
#include <string>

#define FIXED_FRAMERATE (1)

#if FIXED_FRAMERATE
// 固定フレームレートのためのパフォーマンスカウンター
bool performanceCounter;
LARGE_INTEGER freq, startCount, finishCount;
float count;
#endif

int freameCount = 0;
float framePerSrecond = 0.0f;
float countTime = 0.0f;

extern eSceneTable initialScene;

// 静的変数
int sanMainFrame::screenWidth;   // 画面の幅
int sanMainFrame::screenHeight;  // 画面の高さ
int sanMainFrame::screenCenterX; // 画面の中央(横)
int sanMainFrame::screenCenterY; // 画面の中央(縦)

float sanMainFrame::clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 画面のクリアカラー

sanScene* sanMainFrame::pScene = NULL;

// メインフレームの初期化
bool sanMainFrame::initialize(void* hWnd, void* hInst, int width, int height)
{
	// 画面サイズの設定
	screenWidth = width;
	screenHeight = height;
	screenCenterX = width / 2;
	screenCenterY = height / 2;

#if FIXED_FRAMERATE
	// パフォーマンスｶカウンタの初期化
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&finishCount, 0, sizeof(finishCount));
	count = 0.0f;

	performanceCounter = QueryPerformanceFrequency(&freq);
#endif

	sanDirect3D::initialize();

	sanDirect2D::initialize();

	sanDirectInput::initialize(hInst);

	sanXInput::initialize();

	sanXAudio2::initialize();

	sanShader::initialize();

	sanFont::initialize();

	sanCamera::initialize();

	sanLight::initialize();

	sanDebugDraw::initialize();

	sanObject::initializeCommon();

	sanSprite::initializeCommon();

	switchScene();

	return true;
}

// 終了
// 終了時は逆順で読んだ方がいい
void sanMainFrame::terminate()
{
	terminateScene();

	sanSprite::terminateCommon();

	sanObject::terminateCommon();

	sanDebugDraw::terminate();

	sanLight::terminate();

	sanCamera::terminate();

	sanFont::terminate();

	sanShader::terminate();

	sanXAudio2::terminate();

	sanXInput::terminate();

	sanDirectInput::terminate();

	sanDirect2D::terminate();
	
	sanDirect3D::terminate();
}

// 実行
void sanMainFrame::execute()
{
#if FIXED_FRAMERATE
	if (performanceCounter)
	{
		// フレームの開始時間を取得
		QueryPerformanceCounter(&startCount);
	}
#endif

	HRESULT hr = S_OK;

	// 入力管理
	sanDirectInput::KeyManager();
	sanXInput::KeyManager();

	// 通常処理開始
	//BACKボタンで起動時のステージに戻る。
	if (sanKeyboard::trg(DIK_BACK))
	{
		switchScene(initialScene);
	}

	pScene->execute();
	// 通常処理終了

	// [Direct3D描画
	sanDirect3D::beginRender();

	pScene->render();

	sanDebugDraw::render();

	sanDirect3D::finishRender();

	// Direct2D描画
	sanDirect2D::render();

#if FIXED_FRAMERATE
	// フレームレートの固定
	if (performanceCounter)
	{
		// フレーム終了時間を取得
		static float frame_mec = 1.0f / 60.0f;
		QueryPerformanceCounter(&finishCount);
		count = ((float)(finishCount.QuadPart - startCount.QuadPart) / (float)freq.QuadPart);

		if (count < frame_mec)
		{
			// 1/60秒が経つまで待つ
			DWORD wait = (DWORD)((frame_mec - count) * 1000.0f - 0.5f);
			timeBeginPeriod(1);
			Sleep(wait);
			timeEndPeriod(1);
		}
	}
#endif
	// プレゼンテーション
	sanDirect3D::present();
	
	// ステージ切り替え
	switchScene();
}

void sanMainFrame::initializeScene(sanScene* p)
{
	if (p == NULL)return;
	pScene = p;
	pScene->initialize();
}

void sanMainFrame::terminateScene()
{
	if (pScene != NULL)
	{
		pScene->terminate();
		delete pScene;
	}
	pScene = NULL;
}
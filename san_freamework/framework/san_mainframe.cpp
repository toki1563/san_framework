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

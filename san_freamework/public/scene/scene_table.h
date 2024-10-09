// シーンテーブル
#pragma once

// シーンの種類
enum eSceneTable
{
	Boot,
	SceneMax,
};

// シーンクラス定義ファイル
#include "san_scene.h"

// シーンの切り替え関数(予約)
void switchScene(eSceneTable scene);
// 現在のシーン取得(実処理)
void swichScene();
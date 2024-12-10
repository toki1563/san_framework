// シーンテーブル
#pragma once

// シーンの種類
// セレクト用テーブル
enum eSceneTable
{
	Boot,
	GameTest,
	BillBoardTest,
	GroundTest,
	ModelTest,
	CubeTest,
	PickTest,
	SphereCollisionTest,
	CollisionTest,
	SpriteTest,
	JoystickTest,
	SeTest,
	FontTest,
	SceneMax,
};

// シーンクラス定義ファイル
#include "san_scene.h"
#include "scene_boot.h"
#include "scene_billboard_test.h"
#include "scene_game_test.h"
#include "scene_ground_test.h"
#include "scene_model_test.h"
#include "scene_cube_test.h"
#include "scene_pick_test.h"
#include "scene_spherecollision_test.h"
#include "scene_collision_test.h"
#include "scene_sprite_test.h"
#include "scene_joystick_test.h"
#include "scene_se_test.h"
#include "scene_font_test.h"


// シーンの切り替え関数(予約)
void switchScene(eSceneTable scene);
// 現在のシーン取得(実処理)
void switchScene();
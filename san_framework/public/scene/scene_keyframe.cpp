#include "../../framework.h"
#include "../../framework/san_environment.h"

extern stMotion motion_idle;
extern stMotion motion_walk;

extern void createCharacterMotion();

//初期化関数
bool SceneKeyframetest::initialize()
{
    createCharacterMotion();

    // キャラの作成
    pCharacter = new cCharacter();

    // モーションのポインタを渡す
    pCharacter->setMotion(&motion_idle);

    // シーンへの登録
    registerObject(pCharacter);

    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        registerObject(pCharacter->getParts(i));
    }

    radius = 10.0f; // 座標設定
    theta = 10.0f;
    phi = -10.0f;
    radiusRoll = 0.5f;
    scrollMove = 0.05f;
    gridAxisActive = true;

    scrollMove = 0.05f;

    return true;
}

//終了関数
void SceneKeyframetest::terminate()
{
    deleteObject(pCharacter);
    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        deleteObject(pCharacter->getParts(i));
    }
}

//処理関数
void SceneKeyframetest::execute()
{
    // カメラの回転
    if (sanMouse::getR() >= 0.1) // ﾏｳｽホイール値取得
    {
        radius += radiusRoll;
    }
    else if (sanMouse::getR() <= -0.1 && radius >= radiusRoll * 2)
    {
        radius -= radiusRoll;
    }
    if (sanMouse::onR() && sanMouse::getDX() >= 10) // ｽｸﾛｰﾙ
    {
        theta -= scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDX() <= -10)
    {
        theta += scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDY() <= -10)
    {
        phi += scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDY() >= 10)
    {
        phi -= scrollMove;
    }
    if (sanKeyboard::trg(DIK_L))
    {
        gridAxisActive = !gridAxisActive;
    }
    if (sanKeyboard::trg(DIK_1))
    {
        pCharacter->setMotion(&motion_idle);
    }
    if (sanKeyboard::trg(DIK_2))
    {
        pCharacter->setMotion(&motion_walk);
    }



    // 仰角(Φ)の計算
    float px = cosf(theta) * cosf(phi) * radius;
    float py = sinf(phi) * radius;
    float pz = sin(theta) * cosf(phi) * radius;

    // カメラの座標
    sanCamera::setPosition(px, py, pz);

    sanFont::print(20.0f, 40.0f, L"LキーでGrid&Axis表示・非表示");
    sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
    sanDebugDraw::Axis(5.0f, gridAxisActive);

    sanScene::execute();
}

void SceneKeyframetest::render()
{
    sanScene::render();
}
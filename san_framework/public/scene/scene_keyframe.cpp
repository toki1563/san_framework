#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneKeyframetest::initialize()
{
    pCube = new sanModel(L"data/model/bear_part/", L"head.vnm");
    registerObject(pCube);

    //キーフレーム初期化
    keyPx[0].time = 0.0f;    keyPx[0].value = -5.0f;
    keyPz[0].time = 0.0f;    keyPz[0].value = 0.0f;

    keyPx[1].time = 60.0f;    keyPx[1].value = 5.0f;
    keyPz[1].time = 60.0f;    keyPz[1].value = 0.0f;

    keyPx[2].time = 120.0f;    keyPx[2].value = 0.0f;
    keyPz[2].time = 120.0f;    keyPz[2].value = 5.0f;

    keyPx[3].time = 180.0f;    keyPx[3].value = -5.0f;
    keyPz[3].time = 180.0f;    keyPz[3].value = 0.0f;


    time = 0.0f;

    radius = 10.0f; // 座標設定
    theta = 10.0f;
    phi = -10.0f;
    radiusRoll = 0.5f;
    scrollMove = 0.05f;
    gridAxisActive = true;

    Cursor = 0;
    scrollMove = 0.05f;

    return true;
}

//終了関数
void SceneKeyframetest::terminate()
{
    deleteObject(pCube);
}

//処理関数
void SceneKeyframetest::execute()
{
    // カメラの回転
    if (sanMouse::getR() >= 0.1) // ﾏｳｽホイール値取得
    {
        radius += radiusRoll;
    }
    else if (sanMouse::getR() <= -0.1)
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
        reverse = !reverse;
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


    //時間経過
    if (!reverse)
    {
        time += 1.0f;
    }
    else
    {
        time -= 1.0f;
    }

    //アニメーション時間
    float animTime = 180.0f;

    //アニメーションル－プ
    if (time >= animTime)
    {
        time = 0.0f;
    }
    else if (time <= 0.0f)
    {
        time = 179.0f;
    }

    // 線形補間
    //time   0    15   30    45   60
    //value -5  -2.5    0    2.5   +5


    // 現在の時間で値を線形補間(0~1)
    float rate = time / animTime;
    // キーフレームがどこにあるかの判定
    int startIndexPosX = -1;
    int startIndexPosZ = -1;

    for (int i = 0; i < 3; ++i) // 3はキーフレーム数 - 1
    {
        if (startIndexPosX == -1 && time >= keyPx[i].time && time < keyPx[i + 1].time)
        {
            startIndexPosX = i;
        }

        if (startIndexPosZ == -1 && time >= keyPz[i].time && time < keyPz[i + 1].time)
        {
            startIndexPosZ = i;
        }

        // 両方のインデックスが見つかったらループを抜ける
        if (startIndexPosX != -1 && startIndexPosZ != -1)
        {
            break;
        }
    }
    // 補間
    stKeyframe startPosX = keyPx[startIndexPosX];
    stKeyframe endPosX = keyPx[startIndexPosX + 1];

    stKeyframe startPosZ = keyPz[startIndexPosZ];
    stKeyframe endPosZ = keyPz[startIndexPosZ + 1];

    // X座標
    float t = (time - startPosX.time) / (endPosX.time - startPosX.time); // 正規化された時間
    float valuePx = startPosX.value + (endPosX.value - startPosX.value) * t;

    // Z座標
    float tPz = (time - startPosZ.time) / (endPosZ.time - startPosZ.time); // 正規化された時間
    float valuePz = startPosZ.value + (endPosZ.value - startPosZ.value) * tPz;

    //// 位置の更新
    //pCube->setPositionX(valuePx);
    //pCube->setPositionZ(valuePz);

    sanFont::print(20.0f, 20.0f, L"time : %.3f", time);
}

void SceneKeyframetest::render()
{
    sanScene::render();
}
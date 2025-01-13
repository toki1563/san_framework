#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneKeyframetest::initialize()
{
    pCube = new sanModel(L"data/model/primitive/", L"cube.vnm");
    registerObject(pCube);

    //キーフレーム初期化
    keyPx[0].time = 0.0f;    keyPx[0].value = -5.0f;
    keyPz[0].time = 0.0f;    keyPz[0].value = 0.0f;

    keyPx[1].time = 60.0f;    keyPx[1].value = +5.0f;
    keyPz[1].time = 60.0f;    keyPz[1].value = 0.0f;

    keyPx[2].time = 120.0f;    keyPx[2].value = 0.0f;
    keyPz[2].time = 120.0f;    keyPz[2].value = 5.0f;

    keyPx[3].time = 180.0f;    keyPx[3].value = -5.0f;
    keyPz[3].time = 180.0f;    keyPz[3].value = 0.0f;


    time = 0.0f;

    radius = 12.0f;
    theta = 0.0f;
    phi = 10.0f / 180.0f * 3.14f;

    Cursor = 0;
    scrollMove = 0.05f;

    debugLine = true;
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
    //時間経過
    time += 1.0f;

    //アニメーション時間
    float animtime = 180.0f;

    //アニメーションル－プ
    if (time >= animtime) time = 0.0f;

    //今の時間での値
    float value = 0.0f;

    // 線形補間
    //time   0    15   30    45   60
    //value -5  -2.5    0    2.5   +5

    // 現在の時間で値を線形補間
    float rate = time / animtime;
    int startIndex = 0;
    for (int i = 0; i < 3; ++i)
    { // 2は (キーフレーム数 - 1)
        if (time >= keyPx[i].time && time < keyPx[i + 1].time)
        {
            startIndex = i;
            break;
        }
    }

    for (int i = 0; i < 3; ++i)
    { // 2は (キーフレーム数 - 1)
        if (time >= keyPz[i].time && time < keyPz[i + 1].time)
        {
            startIndex = i;
            break;
        }
    }
    // 補間
    stKeyframe startPx = keyPx[startIndex];
    stKeyframe endPx = keyPx[startIndex + 1];

    stKeyframe startPz = keyPz[startIndex];
    stKeyframe endPZ = keyPz[startIndex + 1];

    float t = (time - startPx.time) / (endPx.time - startPx.time); // 正規化された時間
    value = startPx.value + (endPx.value - startPx.value) * t;

    float tPz = (time - startPz.time) / (endPZ.time - startPz.time); // 正規化された時間
    float valuePz = startPz.value + (endPZ.value - startPz.value) * tPz;

    pCube->setPositionX(value);
    pCube->setPositionZ(valuePz);

    sanFont::print(10.0f, 10.0f, L"time : %.3f", time);

    // カメラの回転
    if (sanMouse::onR() && sanMouse::getDX() >= 10) // ｽｸﾛｰﾙ
    {
        theta += scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDX() <= -10)
    {
        theta -= scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDY() <= -10)
    {
        phi -= scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDY() >= 10)
    {
        phi += scrollMove;
    }

    // カメラ座標の計算
    // 回転してない基準となる軸ベクトル
    XMVECTOR v = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
    // 回転マトリクスを作成
    XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

    // 基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する)
    v = XMVector3TransformNormal(v, rotate);

}

void SceneKeyframetest::render()
{
    sanScene::render();
}
#include "../../framework.h"
#include "../../framework/san_environment.h"

//初期化関数
bool SceneKeyframetest::initialize()
{
    pParts[eParts::Body] = new sanModel(L"data/model/bear_part/", L"body.vnm");

    pParts[eParts::Head] = new sanModel(L"data/model/bear_part/", L"head.vnm");
    pParts[eParts::Head]->setPositionY(0.8f);
    pParts[eParts::Head]->setParent(pParts[eParts::Body]);

    pParts[eParts::ArmL] = new sanModel(L"data/model/bear_part/", L"arm_L.vnm");
    pParts[eParts::ArmL]->setPosition(0.2f, 0.75f, 0.05f);
    pParts[eParts::ArmL]->setRotationZ(0.45f);
    pParts[eParts::ArmL]->setParent(pParts[eParts::Body]);

    pParts[eParts::ArmR] = new sanModel(L"data/model/bear_part/", L"arm_R.vnm");
    pParts[eParts::ArmR]->setPosition(-0.2f, 0.75f, 0.025f);
    pParts[eParts::ArmR]->setRotationZ(5.8f);
    pParts[eParts::ArmR]->setParent(pParts[eParts::Body]);

    pParts[eParts::LegL] = new sanModel(L"data/model/bear_part/", L"leg_L.vnm");
    pParts[eParts::LegL]->setPosition(0.2f, 0.0f, 0.1f);
    pParts[eParts::LegL]->setParent(pParts[eParts::Body]);

    pParts[eParts::LegR] = new sanModel(L"data/model/bear_part/", L"leg_R.vnm");
    pParts[eParts::LegR]->setPosition(-0.2f, 0.0f, 0.1f);
    pParts[eParts::LegR]->setParent(pParts[eParts::Body]);

    for (int i = 0; i < eParts::Max; i++)
    {
        registerObject(pParts[i]);
    }

    channel[0].partsID = eParts::Head;
    channel[0].channelID = eChannel::RotY;
    channel[0].keyframeNum = 2;
    channel[0].pkey = new stKeyFrame[channel[0].keyframeNum];
    channel[0].pkey[0].time = 0.0f; channel[0].pkey[0].value = 0.0f;
    channel[0].pkey[1].time = 60.0f; channel[0].pkey[1].value = 6.28f;

    channel[1].partsID = eParts::Head;
    channel[1].channelID = eChannel::PosY;
    channel[1].keyframeNum = 3;
    channel[1].pkey = new stKeyFrame[channel[1].keyframeNum];
    channel[1].pkey[0].time = 0.0f;  channel[1].pkey[0].value = 0.0f;
    channel[1].pkey[1].time = 30.0f; channel[1].pkey[1].value = 1.0f;
    channel[1].pkey[2].time = 60.0f; channel[1].pkey[2].value = 0.0f;

    time = 0.0f;

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
    for (int i = 0; i < eParts::Max; i++)
    {
        deleteObject(pParts[i]);
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
    if (sanKeyboard::trg(DIK_SPACE))
    {
        reverse = !reverse;
    }

    // 仰角(Φ)の計算
    float px = cosf(theta) * cosf(phi) * radius;
    float py = sinf(phi) * radius;
    float pz = sin(theta) * cosf(phi) * radius;

    // カメラの座標
    sanCamera::setPosition(px, py, pz);

    sanFont::print(20.0f, 40.0f, L"LキーでGrid&Axis表示・非表示");
    sanFont::print(20.0f, 60.0f, L"SPACEキーでアニメーション反転");
    sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
    sanDebugDraw::Axis(5.0f, gridAxisActive);


    float animSpeed = 1.0f; // アニメーションスピード

    //時間経過
    if (!reverse)
    {
        time += animSpeed;
    }
    else
    {
        time -= animSpeed;
    }

    //アニメーション時間
    float animTime = 60.0f;

    //アニメーションル－プ
    if (time >= animTime)
    {
        time = 0.0f;
    }
    else if (time <= 0.0f)
    {
        time = 60.0f - animSpeed;
    }

    // 線形補間
    //time   0    15   30    45   60
    //value -5  -2.5    0    2.5   +5

    float value = 0.0f; // 今の時間で値を計算

    for (int i = 0; i < 2; i++)	//channel
    {
        for (int j = 1; j < channel[i].keyframeNum; j++)	//keyframe
        {
            if (time > channel[i].pkey[j].time)
            {
                continue;
            }

            float rate = (time - channel[i].pkey[j - 1].time) / (channel[i].pkey[j].time - channel[i].pkey[j - 1].time);

            value = (channel[i].pkey[j].value - channel[i].pkey[j - 1].value) * rate + channel[i].pkey[j - 1].value;

            //値を適応
            switch (channel[i].channelID)
            {
            case eChannel::PosX:
                pParts[channel[i].partsID]->setPositionX(value);
                break;
            case eChannel::PosY:
                pParts[channel[i].partsID]->setPositionY(value);
                break;
            case eChannel::PosZ:
                pParts[channel[i].partsID]->setPositionZ(value);
                break;
            case eChannel::RotX:
                pParts[channel[i].partsID]->setRotationX(value);
                break;
            case eChannel::RotY:
                pParts[channel[i].partsID]->setRotationY(value);
                break;
            case eChannel::RotZ:
                pParts[channel[i].partsID]->setRotationZ(value);
                break;
            }
            break;
        }
    }
    sanFont::print(20.0f, 20.0f, L"time : %.3f", time);
}

void SceneKeyframetest::render()
{
    sanScene::render();
}
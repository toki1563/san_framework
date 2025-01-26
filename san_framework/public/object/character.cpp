#include "../../framework.h"
#include "../../framework/san_environment.h"

#include "character.h"

cCharacter::cCharacter()
{
    pParts[eParts::Body] = new sanModel(L"data/model/bear_part/", L"Body.vnm");
    pParts[eParts::Body]->setParent(this);

    pParts[eParts::Head] = new sanModel(L"data/model/bear_part/", L"Head.vnm");
    pParts[eParts::Head]->setParent(pParts[eParts::Body]);

    pParts[eParts::ArmL] = new sanModel(L"data/model/bear_part/", L"Arm_L.vnm");
    pParts[eParts::ArmL]->setParent(pParts[eParts::Body]);

    pParts[eParts::ArmR] = new sanModel(L"data/model/bear_part/", L"Arm_R.vnm");
    pParts[eParts::ArmR]->setParent(pParts[eParts::Body]);

    pParts[eParts::LegL] = new sanModel(L"data/model/bear_part/", L"Leg_L.vnm");
    pParts[eParts::LegL]->setParent(pParts[eParts::Body]);

    pParts[eParts::LegR] = new sanModel(L"data/model/bear_part/", L"Leg_R.vnm");
    pParts[eParts::LegR]->setParent(pParts[eParts::Body]);

    bindPose(); // 位置の設定
    pMotion = NULL;

    time = 0;
}

cCharacter::~cCharacter()
{

}

void cCharacter::execute()
{
#if 1 //0で無効　1で有効
    if (pMotion == NULL) return;

    float animTime = 1.0f;

    //時間経過
    time += animTime;

    //モーションのループ
    if (time >= pMotion->Length)
    {
        time = 0.0f;
    }
    else if (time < 0.0f)
    {
        time = pMotion->Length;
    }

    //今の時間での値を計算
    float value = 0.0f;
    for (int i = 0; i < pMotion->ChannelNum; i++)    //channel 動かしたい部分の数
    {
        for (int j = 1; j < pMotion->pChannel[i].keyframeNum; j++)    //keyframe
        {
            if (time > pMotion->pChannel[i].pKey[j].time)continue;

            float rate = (time - pMotion->pChannel[i].pKey[j - 1].time) / (pMotion->pChannel[i].pKey[j].time - pMotion->pChannel[i].pKey[j - 1].time);

            value = (pMotion->pChannel[i].pKey[j].value - pMotion->pChannel[i].pKey[j - 1].value) * rate + pMotion->pChannel[i].pKey[j - 1].value;

            //値を適応
            switch (pMotion->pChannel[i].channelID)
            {
            case stMotion::eChannel::PosX:
                pParts[pMotion->pChannel[i].partsID]->setPositionX(value);
                break;
            case stMotion::eChannel::PosY:
                pParts[pMotion->pChannel[i].partsID]->setPositionY(value);
                break;
            case stMotion::eChannel::PosZ:
                pParts[pMotion->pChannel[i].partsID]->setPositionZ(value);
                break;
            case stMotion::eChannel::RotX:
                pParts[pMotion->pChannel[i].partsID]->setRotationX(value);
                break;
            case stMotion::eChannel::RotY:
                pParts[pMotion->pChannel[i].partsID]->setRotationY(value);
                break;
            case stMotion::eChannel::RotZ:
                pParts[pMotion->pChannel[i].partsID]->setRotationZ(value);
                break;
            }
            break;
        }
    }

    sanFont::print(10.0f, 10.0f, L"time : %.3f", time);

#endif
}

void cCharacter::bindPose()
{
    // 位置と回転のバインド調整
    pParts[eParts::Body]->setPositionY(0.72f); 

    pParts[eParts::Head]->setParent(pParts[eParts::Body]);
    pParts[eParts::Head]->setPositionY(0.8f);
    pParts[eParts::Head]->setRotation(0, 0, 0);

    pParts[eParts::ArmL]->setParent(pParts[eParts::Body]);
    pParts[eParts::ArmL]->setPosition(0.2, 0.75f, 0.05);
    pParts[eParts::ArmL]->setRotation(0, 0, 0.4537856f);

    pParts[eParts::ArmR]->setParent(pParts[eParts::Body]);
    pParts[eParts::ArmR]->setPosition(-0.2, 0.75f, 0.025f);
    pParts[eParts::ArmR]->setRotation(0, 0, 5.8294f);

    pParts[eParts::LegL]->setParent(pParts[eParts::Body]);
    pParts[eParts::LegL]->setPosition(0.2, 4.37114E-09, 0.1);
    pParts[eParts::LegL]->setRotation(0, 0, 0);

    pParts[eParts::LegR]->setParent(pParts[eParts::Body]);
    pParts[eParts::LegR]->setPosition(-0.2, 4.37114E-09, 0.1);
    pParts[eParts::LegR]->setRotation(0, 0, 0);
}

void cCharacter::setMotion(stMotion* p)
{
    if (pMotion == p)
    {

    }
    if (p == NULL)
    {
        return;
    }
    pMotion = p;

    // アニメーション時間リセット
    time = 0.0f;
    // ポーズを初期化する
    bindPose();
}

sanModel* cCharacter::getParts(int i)
{
    if (i < 0 || i >= eParts::PartsMax)return NULL;
    return pParts[i];
}
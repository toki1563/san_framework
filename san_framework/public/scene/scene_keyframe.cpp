#include "../../framework.h"
#include "../../framework/san_environment.h"

//�������֐�
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
    channel[0].pkey[1].time = 60.0f; channel[0].pkey[1].value = 6.26f;

    channel[1].partsID = eParts::Head;
    channel[1].channelID = eChannel::PosY;
    channel[1].keyframeNum = 3;
    channel[1].pkey = new stKeyFrame[channel[0].keyframeNum];
    channel[1].pkey[0].time = 0.0f;  channel[0].pkey[0].value = 0.0f;
    channel[1].pkey[1].time = 30.0f; channel[0].pkey[1].value = 1.0f;
    channel[1].pkey[2].time = 50.0f; channel[0].pkey[2].value = 0.0f;


    time = 0.0f;

    radius = 10.0f; // ���W�ݒ�
    theta = 10.0f;
    phi = -10.0f;
    radiusRoll = 0.5f;
    scrollMove = 0.05f;
    gridAxisActive = true;

    scrollMove = 0.05f;

    return true;
}

//�I���֐�
void SceneKeyframetest::terminate()
{
    for (int i = 0; i < eParts::Max; i++)
    {
        deleteObject(pParts[i]);
    }
}

//�����֐�
void SceneKeyframetest::execute()
{
    // �J�����̉�]
    if (sanMouse::getR() >= 0.1) // ϳ��z�C�[���l�擾
    {
        radius += radiusRoll;
    }
    else if (sanMouse::getR() <= -0.1 && radius >= radiusRoll * 2)
    {
        radius -= radiusRoll;
    }
    if (sanMouse::onR() && sanMouse::getDX() >= 10) // ��۰�
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


    // �p(��)�̌v�Z
    float px = cosf(theta) * cosf(phi) * radius;
    float py = sinf(phi) * radius;
    float pz = sin(theta) * cosf(phi) * radius;

    // �J�����̍��W
    sanCamera::setPosition(px, py, pz);

    sanFont::print(20.0f, 40.0f, L"L�L�[��Grid&Axis�\���E��\��");
    sanFont::print(20.0f, 60.0f, L"SPACE�L�[�ŃA�j���[�V�������]");
    sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
    sanDebugDraw::Axis(5.0f, gridAxisActive);


    float animSpeed = 1.0f; // �A�j���[�V�����X�s�[�h

    //���Ԍo��
    if (!reverse)
    {
        time += animSpeed;
    }
    else
    {
        time -= animSpeed;
    }

    //�A�j���[�V��������
    float animTime = 180.0f;

    //�A�j���[�V�������|�v
    if (time >= animTime)
    {
        time = 0.0f;
    }
    else if (time <= 0.0f)
    {
        time = 180.0f - animSpeed;
    }

    // ���`���
    //time   0    15   30    45   60
    //value -5  -2.5    0    2.5   +5


    // ���݂̎��ԂŒl����`���(0~1)
    float rate = time / animTime;
    // �L�[�t���[�����ǂ��ɂ��邩�̔���
    int startIndexPosX = -1;
    int startIndexPosZ = -1;

    for (int i = 0; i < 1; i++) //channelNumber
    {
        for (int j = 0; j < 2; ++j) // keyframeNumber
        {
            if (startIndexPosX == -1 && time >= keyPosX[i].time && time < keyPosX[i + 1].time)
            {
                startIndexPosX = i;
            }

            if (startIndexPosZ == -1 && time >= keyPosZ[i].time && time < keyPosZ[i + 1].time)
            {
                startIndexPosZ = i;
            }
            if (time > channel[i].key[j].time)
            {
                continue;
            }

            float rate = (time - channel[i].key[j - 1].time) / channel;
            float value = channel[i].key[j].value - channel[i].key[j - 1].value;

            // �l��Ή�
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

            // �����̃C���f�b�N�X�����������烋�[�v�𔲂���
            if (startIndexPosX != -1 && startIndexPosZ != -1)
            {
                break;
            }
        }
    }
    // ���
    stKeyFrame startPosX = keyPosX[startIndexPosX];
    stKeyFrame endPosX = keyPosX[startIndexPosX + 1];

    stKeyFrame startPosZ = keyPosZ[startIndexPosZ];
    stKeyFrame endPosZ = keyPosZ[startIndexPosZ + 1];

    // X���W
    float t = (time - startPosX.time) / (endPosX.time - startPosX.time); // ���K�����ꂽ����
    float valuePosX = startPosX.value + (endPosX.value - startPosX.value) * t;

    // Z���W
    float tPz = (time - startPosZ.time) / (endPosZ.time - startPosZ.time); // ���K�����ꂽ����
    float valuePosZ = startPosZ.value + (endPosZ.value - startPosZ.value) * tPz;

    // �ʒu�̍X�V
    pParts[eParts::Body]->setPositionX(valuePosX);
    pParts[eParts::Body]->setPositionZ(valuePosZ);


    sanFont::print(20.0f, 20.0f, L"time : %.3f", time);
}

void SceneKeyframetest::render()
{
    sanScene::render();
}
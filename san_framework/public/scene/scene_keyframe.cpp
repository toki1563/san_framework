#include "../../framework.h"
#include "../../framework/san_environment.h"

//�������֐�
bool SceneKeyframetest::initialize()
{
    pCube = new sanModel(L"data/model/bear_part/", L"head.vnm");
    registerObject(pCube);

    //�L�[�t���[��������
    keyPx[0].time = 0.0f;    keyPx[0].value = -5.0f;
    keyPz[0].time = 0.0f;    keyPz[0].value = 0.0f;

    keyPx[1].time = 60.0f;    keyPx[1].value = 5.0f;
    keyPz[1].time = 60.0f;    keyPz[1].value = 0.0f;

    keyPx[2].time = 120.0f;    keyPx[2].value = 0.0f;
    keyPz[2].time = 120.0f;    keyPz[2].value = 5.0f;

    keyPx[3].time = 180.0f;    keyPx[3].value = -5.0f;
    keyPz[3].time = 180.0f;    keyPz[3].value = 0.0f;


    time = 0.0f;

    radius = 10.0f; // ���W�ݒ�
    theta = 10.0f;
    phi = -10.0f;
    radiusRoll = 0.5f;
    scrollMove = 0.05f;
    gridAxisActive = true;

    Cursor = 0;
    scrollMove = 0.05f;

    return true;
}

//�I���֐�
void SceneKeyframetest::terminate()
{
    deleteObject(pCube);
}

//�����֐�
void SceneKeyframetest::execute()
{
    // �J�����̉�]
    if (sanMouse::getR() >= 0.1) // ϳ��z�C�[���l�擾
    {
        radius += radiusRoll;
    }
    else if (sanMouse::getR() <= -0.1)
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
        reverse = !reverse;
    }




    // �p(��)�̌v�Z
    float px = cosf(theta) * cosf(phi) * radius;
    float py = sinf(phi) * radius;
    float pz = sin(theta) * cosf(phi) * radius;

    // �J�����̍��W
    sanCamera::setPosition(px, py, pz);

    sanFont::print(20.0f, 40.0f, L"L�L�[��Grid&Axis�\���E��\��");
    sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
    sanDebugDraw::Axis(5.0f, gridAxisActive);


    //���Ԍo��
    if (!reverse)
    {
        time += 1.0f;
    }
    else
    {
        time -= 1.0f;
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
        time = 179.0f;
    }

    // ���`���
    //time   0    15   30    45   60
    //value -5  -2.5    0    2.5   +5


    // ���݂̎��ԂŒl����`���(0~1)
    float rate = time / animTime;
    // �L�[�t���[�����ǂ��ɂ��邩�̔���
    int startIndexPosX = -1;
    int startIndexPosZ = -1;

    for (int i = 0; i < 3; ++i) // 3�̓L�[�t���[���� - 1
    {
        if (startIndexPosX == -1 && time >= keyPx[i].time && time < keyPx[i + 1].time)
        {
            startIndexPosX = i;
        }

        if (startIndexPosZ == -1 && time >= keyPz[i].time && time < keyPz[i + 1].time)
        {
            startIndexPosZ = i;
        }

        // �����̃C���f�b�N�X�����������烋�[�v�𔲂���
        if (startIndexPosX != -1 && startIndexPosZ != -1)
        {
            break;
        }
    }
    // ���
    stKeyframe startPosX = keyPx[startIndexPosX];
    stKeyframe endPosX = keyPx[startIndexPosX + 1];

    stKeyframe startPosZ = keyPz[startIndexPosZ];
    stKeyframe endPosZ = keyPz[startIndexPosZ + 1];

    // X���W
    float t = (time - startPosX.time) / (endPosX.time - startPosX.time); // ���K�����ꂽ����
    float valuePx = startPosX.value + (endPosX.value - startPosX.value) * t;

    // Z���W
    float tPz = (time - startPosZ.time) / (endPosZ.time - startPosZ.time); // ���K�����ꂽ����
    float valuePz = startPosZ.value + (endPosZ.value - startPosZ.value) * tPz;

    //// �ʒu�̍X�V
    //pCube->setPositionX(valuePx);
    //pCube->setPositionZ(valuePz);

    sanFont::print(20.0f, 20.0f, L"time : %.3f", time);
}

void SceneKeyframetest::render()
{
    sanScene::render();
}
#include "../../framework.h"
#include "../../framework/san_environment.h"

extern stMotion motion_idle;
extern stMotion motion_walk;

extern void createCharacterMotion();

//�������֐�
bool SceneKeyframetest::initialize()
{
    createCharacterMotion();

    // �L�����̍쐬
    pCharacter = new cCharacter();

    // ���[�V�����̃|�C���^��n��
    pCharacter->setMotion(&motion_idle);

    // �V�[���ւ̓o�^
    registerObject(pCharacter);

    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        registerObject(pCharacter->getParts(i));
    }

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
    deleteObject(pCharacter);
    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        deleteObject(pCharacter->getParts(i));
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
    if (sanKeyboard::trg(DIK_1))
    {
        pCharacter->setMotion(&motion_idle);
    }
    if (sanKeyboard::trg(DIK_2))
    {
        pCharacter->setMotion(&motion_walk);
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

    sanScene::execute();
}

void SceneKeyframetest::render()
{
    sanScene::render();
}
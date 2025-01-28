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


    // �V�[���ւ̓o�^
    registerObject(pCharacter);

    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        registerObject(pCharacter->getParts(i));
    }

    // ���[�V�����t�@�C���̓ǂݍ���(�J��)
    FILE* fp = NULL;
    if(_wfopen_s(&fp, L"data/motion/motion_idle.mot", L"rb") != 0) // �ǂݍ��݂̓o�C�i���^
    {
        // �t�@�C�����J���Ȃ�����
        assert(false); // �����I��
    }

    // �t�@�C���T�C�Y�̎擾
    long size = 0;
    fseek(fp, 0, SEEK_END); // �t�@�C���|�C���^���Ō���Ɉړ�
    size = ftell(fp); // ���݂̃t�@�C���|�C���^�̈ʒu���擾(Byte)
    fseek(fp, 0, SEEK_SET); // �t�@�C���|�C���^��擪�Ɉړ�

    // �t�@�C���T�C�Y���̃��������m��
    mot_idle = (sanMotionData*)new BYTE[size];

    // �t�@�C���̒��g�����ׂĎ擾
    fread(mot_idle, size, 1, fp);

    // �t�@�C�������
    fclose(fp);

    // �f�[�^�̒��g
    sanFont::output(L"Lenght : %f\n", mot_idle->Length);
    sanFont::output(L"ChannelNum : %d\n", mot_idle->ChannelNum);
    sanFont::output(L"KeyFrameNum : %d\n", mot_idle->KeyFrameNum);

    sanMotionData_Channel *channel = 
        (sanMotionData_Channel*)((BYTE*)mot_idle + mot_idle->ChannelAccess);
    for (int i = 0; i < mot_idle->ChannelNum; i++)
    {
        // char�������WCHAR������ɕϊ�
        WCHAR name[256];
        size_t ret;
        mbstowcs_s(&ret, name, 256, channel[i].Name, strlen(channel[i].Name));

        sanFont::output(L"Channel[%d].Name : %s\n", i, name);
        sanFont::output(L"Channel[%d].channelID : %d\n", i, channel->ChannelID);
        sanFont::output(L"Channel[%d].KeyFrameNum : %d\n", i, channel->KeyFrameNum);
    }

    // �L�[�t���[��
    sanMotionData_KeyFrame* key =
        (sanMotionData_KeyFrame*)((BYTE*)mot_idle + mot_idle->KeyFrameAccess);
    for (int i = 0; i < mot_idle->KeyFrameNum; i++)
    {
        sanFont::output(L"Key[%d].Time : %f\n", i, key[i].Time);
        sanFont::output(L"Key[%d].Value : %f\n", i, key[i].Value);
    }

    // ���[�V�����̃|�C���^��n��
    pCharacter->setMotion(mot_idle);


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
    delete[] mot_idle;
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
        pCharacter->setMotion(mot_idle);
    }
    //if (sanKeyboard::trg(DIK_2))
    //{
    //    pCharacter->setMotion(mot_walk);
    //}



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
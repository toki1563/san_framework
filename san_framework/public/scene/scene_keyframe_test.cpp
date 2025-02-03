#include "../../framework.h"
#include "../../framework/san_environment.h"

#define OUTPUT_MOTION_DATA (1)

//������
bool SceneKeyframeTest::initialize()
{
	sanLight::initialize();
	sanLight::setLightDir(-3.0f, -7.0f, -4.0f);

	sanCamera::initialize();
	sanCamera::setPosition(-3.0f, 5.0f, 10.0f);
	sanCamera::setTarget(0.0f, 1.5f, 0.0f);



	//���[�V�����t�@�C���̓ǂݍ���
	mot_idle = loadMotionFile(L"data/motion/motion_idle.mot");
	assert(mot_idle != NULL);

	mot_walk = loadMotionFile(L"data/motion/motion_walk.mot");
	assert(mot_walk != NULL);



	//�L�����N�^�[�̍쐬
	pCharacter = new cCharacter(L"data/model/bear_part/", L"bear_part.bone");

	//�L�����N�^�[�ƃp�[�c�̓o�^
	registerObject(pCharacter);
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		registerObject(pCharacter->getParts(i));
	}

	//���[�V�����̍Đ�
	pCharacter->setMotion(mot_idle);

	return true;
}

//�I���֐�
void SceneKeyframeTest::terminate()
{
	deleteObject(pCharacter);
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		deleteObject(pCharacter->getParts(i));
	}

	//���[�V�����f�[�^�̍폜
	if (!mot_idle)
	{
		delete[] mot_idle;
		mot_idle = NULL;
	}
	if (!mot_walk)
	{
		delete[] mot_walk;
		mot_walk = NULL;
	}
}

//�����֐�
void SceneKeyframeTest::execute()
{
	//���[�V�����̐؂�ւ�
	if (sanKeyboard::trg(DIK_1))
	{	//�ҋ@
		pCharacter->setMotion(mot_idle);
	}
	else if (sanKeyboard::trg(DIK_2))
	{	//����
		pCharacter->setMotion(mot_walk);
	}
	else if (sanKeyboard::trg(DIK_3))
	{	//���[�V�����Ȃ�
		pCharacter->setMotion(NULL);
	}

	sanFont::print(100.0f, 100.0f, pCharacter->getMotion() == mot_idle ? 0xffffff00 : 0xffffffff, L"1. idle");
	sanFont::print(100.0f, 120.0f, pCharacter->getMotion() == mot_walk ? 0xffffff00 : 0xffffffff, L"2. walk");
	sanFont::print(100.0f, 140.0f, pCharacter->getMotion() == NULL ? 0xffffff00 : 0xffffffff, L"3. null");

	sanFont::print(100.0f, 180.0f, 0xffffffff, L"time : %.1f / %.1f", pCharacter->getTime(), pCharacter->getMotion()!=NULL ? pCharacter->getMotion()->Length : 0.0f);

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	sanScene::execute();
}

//���[�V�����t�@�C���ǂݍ��݊֐�
sanMotionData *SceneKeyframeTest::loadMotionFile(const WCHAR* motFile)
{
	if (!motFile)return NULL;

	//�߂�l�p�ϐ�
	sanMotionData* pMot = NULL;
	
	//���[�V�����t�@�C���̓ǂݍ���
	FILE* fp = NULL;
	if (_wfopen_s(&fp, motFile, L"rb") != 0)
	{
		//�t�@�C�����J���Ȃ�����
		return NULL;
	}

	//�t�@�C���T�C�Y�̎擾
	long size = 0;
	fseek(fp, 0, SEEK_END);	//�t�@�C���|�C���^���Ō���Ɉړ�
	size = ftell(fp);	//���݂̃t�@�C���|�C���^�ʒu���擾(Byte)
	fseek(fp, 0, SEEK_SET);	//�t�@�C���|�C���^��擪�Ɉړ�

	//�t�@�C���T�C�Y���̃��������m��
	pMot = (sanMotionData*)new BYTE[size];

	//�t�@�C���̒��g��S�Ď擾
	fread(pMot, size, 1, fp);

	//�t�@�C�������
	fclose(fp);

#if OUTPUT_MOTION_DATA
	//�o��Window�ւ̃��O�\��
	sanFont::output(L"Length : %.3f\n", pMot->Length);
	sanFont::output(L"ChannelNum : %d\n", pMot->ChannelNum);
	sanFont::output(L"KeyframeNum : %d\n", pMot->KeyFrameNum);

	//�`�����l���f�[�^�ւ̃A�N�Z�X
	sanMotionData_Channel* channel =	(sanMotionData_Channel*)(((BYTE*)pMot) + pMot->ChannelAccess);

	for (int i = 0; i < (int)pMot->ChannelNum; i++)
	{
		//char�������WCHAR������ɕϊ�
		WCHAR name[256];
		size_t ret;
		mbstowcs_s(&ret, name, 256, channel[i].Name, strlen(channel[i].Name));

		sanFont::output(L"Channel[%d].Name : %s\n", i, name);
		sanFont::output(L"Channel[%d].ChannelID : %d\n", i, channel[i].ChannelID);
		sanFont::output(L"Channel[%d].KeyFrameNum : %d\n", i, channel[i].KeyFrameNum);
		sanFont::output(L"Channel[%d].StartIndex : %d\n", i, channel[i].StartIndex);
	}

	//�L�[�t���[���f�[�^�ւ̃A�N�Z�X
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)pMot) + pMot->KeyFrameAccess);

	for (int i = 0; i < (int)pMot->KeyFrameNum; i++)
	{
		sanFont::output(L"key[%d].Time : %f\n", i, key[i].Time);
		sanFont::output(L"key[%d].Value : %f\n", i, key[i].Value);
	}
#endif

	return pMot;
}


//�`��֐�
void SceneKeyframeTest::render()
{
	sanScene::render();
}

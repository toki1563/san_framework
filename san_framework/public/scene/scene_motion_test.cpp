#include "../../framework.h"
#include "../../framework/san_environment.h"

#define OUTPUT_MOTION_DATA (0)

#define FULL_PATH_MAX	(256)
#define FILE_PATH_MAX	(128)

WCHAR motionFolder[] = L"data/model/BoxUnityChan/motion/";



WCHAR motionFile[][FILE_PATH_MAX] =
{
	L"DAMAGED00.mot",
	L"DAMAGED01.mot",
	L"HANDUP00_R.mot",
	L"JUMP00.mot",
	L"JUMP00B.mot",
	L"JUMP01.mot",
	L"JUMP01B.mot",
	L"LOSE00.mot",
	L"REFLESH00.mot",
	L"RUN00_F.mot",
	L"RUN00_L.mot",
	L"RUN00_R.mot",
	L"SLIDE00.mot",
	L"UMATOBI00.mot",
	L"WAIT00.mot",
	L"WAIT01.mot",
	L"WAIT02.mot",
	L"WAIT03.mot",
	L"WAIT04.mot",
	L"WALK00_B.mot",
	L"WALK00_F.mot",
	L"WALK00_L.mot",
	L"WALK00_R.mot",
	L"WIN00.mot",
};


//������
bool SceneMotionTest::initialize()
{
	sanLight::initialize();
	sanLight::setLightDir(-3.0f, -7.0f, -4.0f);
	sanLight::setLightCol(0.5f, 0.5f, 0.5f);
	sanLight::setAmbient(0.5f, 0.5f, 0.5f);

	sanCamera::initialize();
	sanCamera::setPosition(0.0f, 2.0f, 3.5f);
	sanCamera::setTarget(0.0f, 1.0f, 0.0f);

	//���[�V�����t�@�C���̓ǂݍ���
	motionNum = sizeof(motionFile) / (sizeof(WCHAR) * FILE_PATH_MAX);

	motion = new sanMotionData*[motionNum];
	for (int i = 0; i < motionNum; i++)
	{
		WCHAR path[FULL_PATH_MAX];
		wsprintf(path, L"%s%s", motionFolder, &motionFile[i][0]);

		motion[i] = loadMotionFile(path);
		assert(motion[i] != NULL);

		//��]�̕␳
		rotRoll(motion[i]);
	}

	//�L�����N�^�[�̍쐬
	pCharacter = new cCharacter(L"data/model/BoxUnityChan/", L"BoxUnityChan.bone");

	//�L�����N�^�[�ƃp�[�c�̓o�^
	registerObject(pCharacter);
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		registerObject(pCharacter->getParts(i));
	}

	// pCharacter->setMotion(motion[0]);

	Cursor = 0;

	return true;
}

//�I���֐�
void SceneMotionTest::terminate()
{
	for (int i = 0; i < motionNum; i++)
	{
		if (!motion[i])continue;
		delete[] motion[i];
		motion[i] = NULL;
	}
	deleteObject(pCharacter);
}

//�����֐�
void SceneMotionTest::execute()
{
	if (sanKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = motionNum - 1;
	}
	else if (sanKeyboard::trg(DIK_DOWN) && ++Cursor >= motionNum)
	{
		Cursor = 0;
	}
	if (sanKeyboard::on(DIK_LEFT))
	{
		pCharacter->addRotationY(XMConvertToRadians(3.0f));
	}
	else if (sanKeyboard::on(DIK_RIGHT))
	{
		pCharacter->addRotationY(-XMConvertToRadians(3.0f));
	}
	else if (sanKeyboard::trg(DIK_SPACE))
	{
		pCharacter->setMotion(motion[Cursor]);
	}

	//������@�̕\��
	sanFont::print(30.0f, 10.0f, 0xffffffff, L"����  : �J�[�\���ړ�");
	sanFont::print(30.0f, 30.0f, 0xffffffff, L"SPACE : ���[�V�����Đ�");
	sanFont::print(30.0f, 50.0f, 0xffffffff, L"����  : ���f����] (RotY = %.3f)", XMConvertToDegrees(pCharacter->getRotationY()));
	sanFont::print(350.0f, 50.0f, 0xffffffff, L"���f����RotV : %d", pCharacter->getRotationY());

	//�J�[�\���̕\��
	sanFont::print(10.0f, 130.0f + 20.0f * (float)Cursor, 0xffffff00, L"��");

	//���o���̕\��
	sanFont::print(30.0f, 90.0f, 0xffffffff, L"[MotionFile]");
	sanFont::print(200.0f, 90.0f, 0xffffffff, L"[ChannelNum]");
	sanFont::print(350.0f, 90.0f, 0xffffffff, L"[KeyFrameNum]");
	//���[�V�������̕\��
	for (int i = 0; i < motionNum; i++)
	{   
		DWORD col = pCharacter->getMotion() == motion[i] ? 0xffffff00 : 0xffffffff;
		sanFont::print(30.0f, 130.0f + 20.0f * (float)i, col, &motionFile[i][0]);
		sanFont::print(200.0f, 130.0f + 20.0f * (float)i, col, L"%d", motion[i]->ChannelNum);
		sanFont::print(350.0f, 130.0f + 20.0f * (float)i, col, L"%d", motion[i]->KeyFrameNum);
	}

	// �L�����N�^�[�̃p�[�c���擾����sanObject* 
	for (int i = 0; i < pCharacter->getPartsNum(); i++)
	{
		sanObject* pChild = pCharacter->getParts(i);
		if (pChild == NULL) continue;
		sanObject* pParent = pChild->getParent();
		if (pParent == NULL) continue;

		XMVECTOR v, m;
		pChild->getWorldPosition(&v);
		pParent->getWorldPosition(&m);
		sanDebugDraw::Line(&v, &m);
	}

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	sanScene::execute();
}

//�`��֐�
void SceneMotionTest::render()
{
	sanScene::render();
}

//���[�V�����t�@�C���ǂݍ��݊֐�
sanMotionData* SceneMotionTest::loadMotionFile(const WCHAR* motFile)
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
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)pMot) + pMot->ChannelAccess);

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


//��]�␳
void SceneMotionTest::rotRoll(sanMotionData* p)
{
	if (!p)return;

	//�`�����l���f�[�^�ւ̃A�N�Z�X
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)p) + p->ChannelAccess);

	//�L�[�t���[���f�[�^�ւ̃A�N�Z�X
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)p) + p->KeyFrameAccess);

	for (int i = 0; i < (int)p->ChannelNum; i++)
	{
		//Rotation�̃`�����l���݂̂�ΏۂƂ���
		if (channel[i].ChannelID != eMotionChannel::RotX && channel[i].ChannelID != eMotionChannel::RotY && channel[i].ChannelID != eMotionChannel::RotZ)continue;

		//�e�`�����l���ɂ�����L�[�t���[���z��̍ŏ�
		sanMotionData_KeyFrame* k = key + channel[i].StartIndex;

		//�`�����l�����ł̍ő�l�ƍŏ��l�����߂�
		float min = 100.0f;
		float max = -100.0f;
		for (int j = 0; j < channel[i].KeyFrameNum; j++)
		{
			if (k[j].Value > max)max = k[j].Value;
			if (k[j].Value < min)min = k[j].Value;
		}

		//�ő�l�ƍŏ��l�̍���270�x�ȏ゠��Ε␳����(���Ԓl�ȏ�̒l��360�x�}�C�i�X����)
		if (max - min < XMConvertToRadians(270.0f))continue;
		float avr = (min + max) * 0.5f;
		for (int j = 0; j < channel[i].KeyFrameNum; j++)
		{
			if (k[j].Value > avr)k[j].Value -= XMConvertToRadians(360.0f);
		}
	}
}

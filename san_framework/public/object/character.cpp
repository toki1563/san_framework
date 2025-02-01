#include "../../framework.h"
#include "../../framework/san_environment.h"

#include "character.h"

#define OUTPUT_BONE_DATA (1)

cCharacter::cCharacter(const WCHAR* folder, const WCHAR* boneFile)
{
	//�K�w�\���t�@�C���̃p�X(�t�H���_���ƃt�@�C����������)
	WCHAR path[256];
	swprintf_s(path, L"%s%s", folder, boneFile);

	//�K�w�\���t�@�C���̓ǂݍ���
	FILE* fp = NULL;
	if (_wfopen_s(&fp, path, L"rb") != 0)
	{
		//�t�@�C�����J���Ȃ�����
		assert(false);
	}

	//�t�@�C���T�C�Y�̎擾
	long size = 0;
	fseek(fp, 0, SEEK_END);	//�t�@�C���|�C���^���Ō���Ɉړ�
	size = ftell(fp);		//���݂̃t�@�C���|�C���^�ʒu���擾(Byte)
	fseek(fp, 0, SEEK_SET);	//�t�@�C���|�C���^��擪�Ɉړ�

	//�t�@�C���T�C�Y���̃��������m��
	pBoneData = (sanModel_BoneData*)new BYTE[size];

	//�t�@�C���̒��g��S�Ď擾
	fread(pBoneData, size, 1, fp);

	//�t�@�C�������
	fclose(fp);



	//�t�@�C�����Ɋi�[���ꂽ�p�[�c�����v�Z
	PartsNum = size / sizeof(sanModel_BoneData);

	//�p�[�c�̐������I�u�W�F�N�g���쐬���邽�߂̃I�u�W�F�N�g�|�C���^�z����쐬
	pParts = new sanObject * [PartsNum];

	//vnModel_BoneData�\���̔z���ǂݎ��Ȃ���p�[�c�̍쐬���K�w�̍\�z
	for (int i = 0; i < PartsNum; i++)
	{
		WCHAR partsname[256], partsfile[256];
		size_t ret;
		mbstowcs_s(&ret, partsname, 256, pBoneData[i].Name, strlen(pBoneData[i].Name));	//�p�[�c����WCHAR�ɕϊ�
		swprintf_s(partsfile, L"%s.vnm", partsname);	//".vnm"��t���ă��f���t�@�C�����ɂ���
		swprintf_s(path, L"%s%s", folder, partsfile);	//�t�@�C���̑��݂��m�F���邽�߃p�X���쐬

#if OUTPUT_BONE_DATA
		sanFont::output(L"Parts[%d].Name : %s\n", i, partsname);
#endif

		//�p�[�c����vnm�t�@�C�������݂��邩���ׂ�(fopen()�����s����΃t�@�C�����Ȃ�)
		if ((_wfopen_s(&fp, path, L"rb")) != 0)
		{	//vnm�t�@�C�������݂��Ȃ��p�[�c��vnObject�N���X�ō쐬
			pParts[i] = new sanObject();
		}
		else
		{	//vnm�t�@�C�������݂��Ȃ��p�[�c��vnModel�N���X�ō쐬
			sanModel* pModel = new sanModel(folder, partsfile);
			pParts[i] = pModel;
		}

		//�e�q�֌W�̐ݒ�
		if (pBoneData[i].ParentID == -1)
		{	//��parentID == -1 ��cCharacter�I�u�W�F�N�g���g��e�ɐݒ�
			pParts[i]->setParent(this);
		}
		else
		{
			pParts[i]->setParent(pParts[pBoneData[i].ParentID]);
		}

		//pos, rot, scl�̐ݒ�͌��bindPose()�֐��ōs���̂ł����ł͏ȗ�����
	}

	//�o�C���h�|�[�Y�ɂ���(�p�[�c�̎p���̏�����)
	bindPose();

	pMotion = NULL;
}

cCharacter::~cCharacter()
{
	/*�p�[�c�I�u�W�F�N�g��delete�͍쐬�����V�[�����őΉ�*/

	//�{�[���f�[�^�̍폜
	if (pBoneData != NULL)
	{
		delete[] pBoneData;
		pBoneData = NULL;
	}

}

void cCharacter::execute()
{
	if (pMotion == NULL)return;

	//���Ԍo��
	time += 1.0f;

	//�A�j���[�V�����̃��[�v
	if (time >= pMotion->Length)
	{
		time = 0.0f;
	}
	else if (time < 0.0f)
	{
		time = pMotion->Length;
	}

	//���̎��Ԃł̒l���v�Z
	float value = 0.0f;

	//�`�����l���\���̔z��̐擪�A�h���X
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)pMotion) + pMotion->ChannelAccess);

	//�L�[�t���[���\���̔z��̐擪�A�h���X
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)pMotion) + pMotion->KeyFrameAccess);

	for (int i = 0; i < (int)pMotion->ChannelNum; i++)	//vnMotionData_Channel
	{
		//�e�`�����l���ɂ�����L�[�t���[���z��̍ŏ�
		sanMotionData_KeyFrame* k = key + channel[i].StartIndex;

		//���[�V�����Ώۂ̃p�[�c
		sanObject* pObj = getParts(channel[i].Name);

		//�G���[�`�F�b�N
		if (!k || !pObj)continue;

		for (int j = 1; j < channel[i].KeyFrameNum; j++)	//vnMotionData_KeyFrame
		{
			if (time > k[j].Time)continue;

			float rate = (time - k[j - 1].Time) / (k[j].Time - k[j - 1].Time);

			value = (k[j].Value - k[j - 1].Value) * rate + k[j - 1].Value;

			switch (channel[i].ChannelID)
			{
			case eMotionChannel::PosX:
				pObj->setPositionX(value);
				break;
			case eMotionChannel::PosY:
				pObj->setPositionY(value);
				break;
			case eMotionChannel::PosZ:
				pObj->setPositionZ(value);
				break;
			case eMotionChannel::RotX:
				pObj->setRotationX(value);
				break;
			case eMotionChannel::RotY:
				pObj->setRotationY(value);
				break;
			case eMotionChannel::RotZ:
				pObj->setRotationZ(value);
				break;
			case eMotionChannel::SclX:
				pObj->setScaleX(value);
				break;
			case eMotionChannel::SclY:
				pObj->setScaleY(value);
				break;
			case eMotionChannel::SclZ:
				pObj->setScaleZ(value);
				break;
			}

			break;
		}
	}
}

//�o�C���h�|�[�Y(�L�����N�^�[�W�����)�ɖ߂�
void cCharacter::bindPose()
{
	for (int i = 0; i < PartsNum; i++)
	{
		pParts[i]->setPosition(pBoneData[i].pos[0], pBoneData[i].pos[1], pBoneData[i].pos[2]);
		pParts[i]->setRotation(pBoneData[i].rot[0], pBoneData[i].rot[1], pBoneData[i].rot[2]);
		pParts[i]->setScale(pBoneData[i].scl[0], pBoneData[i].scl[1], pBoneData[i].scl[2]);
	}
}

//�Đ����郂�[�V������ݒ�(NULL�Ŗ��K�p�ɖ߂�)
void cCharacter::setMotion(sanMotionData* p)
{
	//�������[�V�����������疳������
	if (pMotion == p)return;

	//���[�V�����f�[�^�̃|�C���^����
	pMotion = p;

	//�^�C�����[���ɖ߂�
	time = 0.0f;

	//�|�[�Y��W����Ԃɖ߂�
	bindPose();
}

//���݂̃��[�V�������擾
sanMotionData* cCharacter::getMotion(void)
{
	return pMotion;
}

//�Đ��ʒu(����)���擾
float cCharacter::getTime(void)
{
	return time;
}


//�p�[�c�����擾
int cCharacter::getPartsNum(void)
{
	return PartsNum;
}

//�p�[�c��ԍ��Ŏ擾
sanObject* cCharacter::getParts(int i)
{
	if (i < 0 || i >= PartsNum)return NULL;
	return pParts[i];
}

//�p�[�c�𖼑O�Ŏ擾
sanObject* cCharacter::getParts(char* name)
{
	for (int i = 0; i < PartsNum; i++)
	{
		if (strcmp(name, pBoneData[i].Name) == 0)
		{
			return pParts[i];
		}
	}
	return NULL;
}

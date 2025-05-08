#pragma once

class cCharacter : public sanObject
{
private:

#pragma region Hierarchy	// �K�w�\���֘A

	// �K�w�\���f�[�^(bone�t�@�C��)
	// �����O��|�[�Y�̏����l(�o�C���h�|�[�Y)�����Q�Ƃ��邽�ߕێ���������
	sanModel_BoneData* pBoneData;

	// �p�[�c�̐�
	int PartsNum;

	// �p�[�c���Ƃ̃I�u�W�F�N�g�|�C���^
	// ��vnm�t�@�C��(�|���S��)�����݂��Ȃ��K�w�����蓾�邽��vnObject�N���X�̃|�C���^�ō쐬
	sanObject** pParts;

#pragma endregion


#pragma region Motion	// ���[�V�����֘A

	float time;	// �o�ߎ���
	float animSpeed; // �A�j���[�V�������x

	// �Đ����̃��[�V�����f�[�^
	sanMotionData* pMotion;

#pragma endregion



public:
	cCharacter(const WCHAR* folder, const WCHAR* boneFile);
	~cCharacter();

	void execute();

	// �o�C���h�|�[�Y(�L�����N�^�[�W�����)�ɖ߂�
	void bindPose();

	// �Đ����郂�[�V������ݒ�(NULL�Ŗ��K�p�ɖ߂�)
	void setMotion(sanMotionData* p);

	// ���݂̃��[�V�������擾
	sanMotionData* getMotion(void);

	// �Đ��ʒu(����)���擾
	float getTime(void);

	// �p�[�c�����擾
	int getPartsNum(void);

	// �p�[�c��ԍ��Ŏ擾
	sanObject* getParts(int i);

	// �p�[�c�𖼑O�Ŏ擾
	sanObject* getParts(char* name);

	void setAnimSpeed(int speed);

	// ���[�V�����I���t���O
	bool isMotionEnded; 

};
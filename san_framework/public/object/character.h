#pragma once

// �L�[�t���[���\����
struct stKeyframe
{
	float time;  // ����
	float value; // �l
};

// �A�j���[�V�����p�[�c�̃`�����l��
struct stMotionChannel
{
	int partsID;     // �p�[�c�̎��
	int channelID;	 // �`�����l���̔ԍ�
	int keyframeNum; // �L�[�t���[���̑���
	stKeyframe* pKey; // �ς�����
};

// ����m�F�Ƃ��Ďg�p
struct stMotion
{
	// �ǂ��𓮂������̍\����
	enum eChannel
	{
		PosX,
		PosY,
		PosZ,
		RotX,
		RotY,
		RotZ,
		ChannelMax,
	};

	int Length;
	int ChannelNum;
	stMotionChannel* pChannel;
};


// ���ʂ��Ƃɍ쐬
//stMotionChannel walk[2];
//stMotionChannel run[5];
//stMotionChannel jump[8];
//stMotionChannel atk[4];


class cCharacter : public sanObject
{
public:
	enum eParts
	{
		Body,
		Head,
		ArmL,
		ArmR,
		LegL,
		LegR,
		PartsMax,
	};

private:
	sanModel* pParts[eParts::PartsMax];

	float time; // ���Ԍo��

	// �Đ����̃��[�V�����f�[�^
	// stMotion* pMotion;
	sanMotionData* pMotion;

public:
	cCharacter();
	~cCharacter();

	void execute();

	// �o�C���h�|�[�Y(�L�����̕W�����)�ɖ߂�
	void bindPose();

	// void setMotion(stMotion* p);
	void setMotion(sanMotionData* p);

	sanModel* getParts(int i);
	sanModel* getParts(char* name);
};
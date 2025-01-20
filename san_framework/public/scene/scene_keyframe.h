#pragma once

class SceneKeyframetest : public sanScene
{
	// �L�[�t���[���\����
	struct stKeyFrame
	{
		float time;  // ����
		float value; // �l
	};

	// �A�j���[�V�����p�[�c�̃`�����l��
	struct stMotionChannel
	{
		int partsID;     // �p�[�c�̎��
		int channelID;	 // �`�����l���̔ԍ�
		int keyframeNum;
		stKeyFrame *pkey; // �ς�����
	};

	stMotionChannel channel[2];

	// �ǂ��𓮂������̍\����
	enum eChannel
	{
		PosX,
		PosY,
		PosZ,
		RotX,
		RotY,
		RotZ,
	};

	enum eParts
	{
		Body,
		Head,
		ArmL,
		ArmR,
		LegL,
		LegR,
		Max,
	};
	sanModel* pParts[6];

	float time; // ���Ԍo��
	float radius;  // ���a
	float theta;   // ���ʊp
	float phi;     // �p
	float value_r; // ��]
	float radians;   // ���W�A��
	float radiusRoll; // �~�h����]���̒l
	float scrollMove; // �E�N���b�N���̃X�N���[���l
	bool reverse;     // ���]�t���O
	bool gridAxisActive;  // �O���b�h&���\����\��

public:
	//������
	bool initialize();
	//�I��
	void terminate();
	//����
	void execute();
	//�`��
	void render();
};
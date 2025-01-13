#pragma once

class SceneKeyframetest : public sanScene
{
	bool debugLine;

	struct stKeyframe
	{
		float time;  // ����
		float value; // �l
	};

	stKeyframe keyPx[4]; // �L�[�t���[���z�� positionX�p
	stKeyframe keyPz[4]; // �L�[�t���[���z�� positionZ�p

	void InitializeKeyFramesPx()
	{
		keyPx[0] = { 0.0f, -5.0f };
		keyPx[1] = { 60.0f,  5.0f };
		keyPx[2] = { 120.0f,  0.0f };
		keyPx[3] = { 180.0f, -5.0f };
	}

	void InitializeKeyFramesPz()
	{
		keyPx[0] = { 0.0f, 0.0f };
		keyPx[1] = { 60.0f, 0.0f };
		keyPx[2] = { 120.0f, 5.0f };
		keyPx[3] = { 180.0f, 0.0f };
	}

	float time; // ���Ԍo��

	sanModel* pCube;
	sanModel* pCube2;

	float radius; // ���a
	float theta;
	float phi;

	int Cursor;
	float scrollMove; // �E�N���b�N���̃X�N���[���l


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
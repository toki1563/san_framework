#pragma once
#include "../object/character.h"

class SceneKeyframetest : public sanScene
{
	// �L�����N�^�[�I�u�W�F�N�g
	cCharacter* pCharacter;

	// ���[�V�����t�@�C������ǂݍ��񂾃f�[�^
	sanMotionData *mot_idle;

	
	float radius;  // ���a
	float theta;   // ���ʊp
	float phi;     // �p
	float value_r; // ��]
	float radians;   // ���W�A��
	float radiusRoll; // �~�h����]���̒l
	float scrollMove; // �E�N���b�N���̃X�N���[���l
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
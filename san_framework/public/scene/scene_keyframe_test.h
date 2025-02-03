#pragma once

#include "../object/character.h"

class SceneKeyframeTest : public sanScene
{
private:
	//�L�����N�^�[�I�u�W�F�N�g
	cCharacter* pCharacter;

	//�ǂݍ��񂾃��[�V�����f�[�^
	sanMotionData* mot_idle;
	sanMotionData* mot_walk;

	//���[�V�����t�@�C���ǂݍ��݊֐�
	sanMotionData *loadMotionFile(const WCHAR* motFile);
	
public:	
	bool initialize();

	void terminate();

	void execute();
	void render();
};

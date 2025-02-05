#pragma once

class SceneMotionTest : public sanScene
{
private:

	int Cursor;

	//�L�����N�^�[�I�u�W�F�N�g
	cCharacter* pCharacter;

	//���[�V�����̐�
	int motionNum;
	//�ǂݍ��񂾃��[�V�����f�[�^
	sanMotionData** motion;

	//���[�V�����t�@�C���ǂݍ��݊֐�
	sanMotionData* loadMotionFile(const WCHAR* motFile);

	//��]�␳
	void rotRoll(sanMotionData* p);
	
public:	
	bool initialize();

	void terminate();

	void execute();
	void render();
};

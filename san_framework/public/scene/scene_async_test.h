#pragma once

class SceneAsyncTest : public sanScene
{
public:
	//�X���b�h�ɑ�������̍\����
	struct ThreadArg
	{
		int num;
		WCHAR *path;
		sanModel** ppModel;
	};

private:
	//�X���b�h�ɑ�������̍\����
	ThreadArg arg;

	//�N���e�B�J���Z�N�V����
	static CRITICAL_SECTION cs;

	//���f���f�[�^�̃p�X
	WCHAR path[256][256];

	//�ǂݍ��݃��f����
	int num;

	//�ǂݍ��݊������f����
	static int count;

	//�񓯊��ǂݍ��݊֐�
	static unsigned __stdcall loadAsync(void* lpx);

	//�X���b�h�̃n���h��
	HANDLE hTh;

	//�ǂݍ��݊������o�p
	int pastCount;

	//���f���z��
	sanModel** pModel;

	//�t���[���J�E���^
	int time;

public:
	//������
	bool initialize();

	//�I��
	void terminate();

	//����
	virtual void execute();

	//�`��
	virtual void render();
};

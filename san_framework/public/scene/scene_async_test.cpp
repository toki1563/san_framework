#include "../../framework.h"
#include "../../framework/san_environment.h"

#include <process.h>

//--------------------------------
//���񓯊��ǂݍ��ݒ��ɃV�[�����o��ƁA�V�[�����؂�ւ���Ă��A�ǂݍ��݃X���b�h���c�����܂܎~�܂炸�ɃG���[���N���܂��B
//--------------------------------

#define MODEL_NUM_X (5)
#define MODEL_NUM_Y (5)
#define MODEL_NUM (MODEL_NUM_X*MODEL_NUM_Y)

#define MODEL_INTERVAL (2.5f)

CRITICAL_SECTION SceneAsyncTest::cs;

int SceneAsyncTest::count = 0;

unsigned __stdcall SceneAsyncTest::loadAsync(void *lpx)
{
	ThreadArg* arg = (ThreadArg*)lpx;

	for (int i = 0; i < arg->num; i++)
	{
		Sleep(200);	//�킩��₷�����邽�߁A�킴�Ə����҂�

		arg->ppModel[i] = new sanModel(L"data/model/bear/", &arg->path[256 * i]);

		//�N���e�B�J���Z�N�V�����ɂ��f�[�^�̕ی�(���X���b�h����̋�����Ԃ�h��)
		EnterCriticalSection(&cs);
		count++;
		LeaveCriticalSection(&cs);

	}
	return 0;
}

//������
bool SceneAsyncTest::initialize()
{
	//�J�����̏����z�u
	sanCamera::setPosition(0.0f, 5.0f, 10.0f);
	sanCamera::setTarget(0.0f, 0.0f, 0.0f);

	//�񓯊��ǂݍ��݂̏���
	count = 0;
	num = 0;
	ZeroMemory(path, sizeof(path));

	//�ǂݍ��ރ��f���̐��ƃt�@�C�����̗p��
	for (int i = 0; i < MODEL_NUM; i++)
	{
		wsprintf(&path[num++][0], L"bear.vnm");
	}

	//���f���f�[�^���쐬����|�C���^�̗p��
	pModel = new sanModel * [num];
	ZeroMemory(pModel, sizeof(sanModel*) * num);

	//�N���e�B�J���Z�N�V�����̏�����
	InitializeCriticalSection(&cs);

	//�X���b�h�ɑ�����̍쐬
	unsigned int thID;
	arg.num = num;
	arg.path = (WCHAR*)path;
	arg.ppModel = pModel;

	//�񓯊��ǂݍ��ݗp�X���b�h�̍쐬�ƊJ�n
	hTh = (HANDLE)_beginthreadex(
		NULL,			//SECURITY_ATTRIBUTES �\���̂ւ̃|�C���^�[
		0,				//�V�����X���b�h�̃X�^�b�N �T�C�Y�܂��� 0
		loadAsync,		//�V�����X���b�h�̎��s���J�n���郋�[�`���̊J�n�A�h���X
		&arg,			//�V�����X���b�h�ɓn�����������X�g�܂��� NULL
		0,				//�V�����X���b�h�̏�����Ԃ𐧌䂷��t���O
		&thID);			//�X���b�h���ʎq���󂯎�� 32 �r�b�g�ϐ��ւ̃|�C���^�[
	if (hTh == 0) { sanFont::output(L"fail to create thread %d.\n"); }

	pastCount = 0;
	time = 0;

	return true;
}

//�I��
void SceneAsyncTest::terminate()
{
	for (int i = 0; i < num; i++)
	{
		if (pModel[i] == NULL)continue;
		sanScene::deleteObject(pModel[i]);
		pModel[i] = NULL;
	}
	delete pModel;
}

//�����֐�
void SceneAsyncTest::execute()
{
	if (hTh)	//�ǂݍ��ݒ�
	{
		//�쐬�������������f���̏����ݒ�ƃV�[�����N���X�ւ̓o�^
		while (pastCount < count)
		{
			float sx = -((float)(MODEL_NUM_X - 1) * MODEL_INTERVAL) / 2.0f;
			float sz = -((float)(MODEL_NUM_Y - 1) * MODEL_INTERVAL) / 2.0f;

			float x = (float)(pastCount % MODEL_NUM_X);
			float z = (float)(pastCount / MODEL_NUM_Y);

			pModel[pastCount]->setPosition(sx + x * (float)MODEL_INTERVAL, 0.0f, sz + z * (float)MODEL_INTERVAL);

			sanScene::registerObject(pModel[pastCount]);

			pastCount++;
		}

		//�X���b�h�̏�Ԃ𒲂ׂ�
		DWORD exitCode;
		BOOL error_code = GetExitCodeThread(
			hTh,      // �X���b�h�n���h��
			&exitCode   // �I���R�[�h
		);
		if (exitCode != STILL_ACTIVE)
		{	//�X���b�h�̏���������
			CloseHandle(hTh);	//�n���h�����N���[�Y
			hTh = 0;

			DeleteCriticalSection(&cs);	//�N���e�B�J���Z�N�V�������폜
		}

		//���b�Z�[�W�̕\��
		int msg = (++time / 30);
		if (msg >= 4)
		{
			msg = 0;
			time = 0;
		}
		switch (msg)
		{
		case 0:	sanFont::print(10, 10, L"Now Loading ");    break;
		case 1:	sanFont::print(10, 10, L"Now Loading .");	break;
		case 2:	sanFont::print(10, 10, L"Now Loading ..");	break;
		case 3:	sanFont::print(10, 10, L"Now Loading ...");	break;
		}

	}
	else
	{	//�ǂݍ��݊�����̒ʏ�execute
		sanFont::print(10, 10, L"Load Finish !");
	}

	//�i���󋵕\��
	sanFont::print(200, 10, L"progress : %d / %d", count, num);

	//�ǂݍ��ݏI��������f�����珈��
	for (int i = 0; i < num; i++)
	{
		if (pModel[i] == NULL)continue;
		pModel[i]->addRotationY(XMConvertToRadians(3.0f));
	}

	sanDebugDraw::Grid();
	sanDebugDraw::Axis();

	sanScene::execute();
}

//�`��֐�
void SceneAsyncTest::render()
{
	sanScene::render();
}

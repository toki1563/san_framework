#pragma once

#include "../object/skinned_cube.h"

class SceneSkinTest : public sanScene
{
private:
	cSkinnedCube* pCube;

	//����̎��
	enum eOperation
	{
		PositionX,
		PositionY,
		PositionZ,
		RotateX,
		RotateY,
		RotateZ,
		ScaleX,
		ScaleY,
		ScaleZ,
		OperationMax,
	};

	int	Cursor;
	int Select;

	// �ɍ��W�̏��
	float radius;
	float theta;
	float phi;
	float radiusRoll; // �~�h����]���̒l
	float scrollMove; // �E�N���b�N���̃X�N���[���l

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
	void operateCamera(); // �J��������
};

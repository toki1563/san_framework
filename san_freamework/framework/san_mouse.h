// �}�E�Xnyuuryoku
#pragma once

class sanMouse
{
public:
	// �}�E�X�{�^��
	enum eButton
	{
		LT = 0x01, // ���{�^��
		MD = 0x02, // ���{�^��
		RT = 0x04, // �E�{�^��
	};

	static int getX(void);   // ���WX
	static int getY(void);	 // ���WY
	static int getR(void);	 // ���[��
	static int getDX(void);  // �ڑ���X
	static int getDY(void);  // �ړ���Y
	static bool on(eButton btn = LT);   // �I�����(�����Ă����)�擾
	static bool trg(eButton btn = LT);  // �g���K�[(�������u��)�̎擾
	static bool rel(eButton btn = LT);  // �����[�X(�������u��)�̎擾

	// ���݂̏�Ԃ��擾
	static bool onL() { return on(LT); }
	static bool onM() { return on(MD); }
	static bool onR() { return on(RT); }

	static bool trgL() { return trg(LT); }
	static bool trgM() { return trg(MD); }
	static bool trgR() { return trg(RT); }

	static bool relL() { return rel(LT); }
	static bool relM() { return rel(MD); }
	static bool relR() { return rel(RT); }

private:
	static int x;
	static int y;
	static int r;
	static int dx;
	static int dy;
	static int On;
	static int Trg;
	static int Rel;

public:
	// �}�E�X�̏�Ԃ�ݒ�
	static void setMouseStatus(void);
};
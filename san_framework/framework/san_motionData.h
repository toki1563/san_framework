//--------------------------------------------------------------//
//	"san_motionData.h"											//
//		���[�V�����f�[�^��`									    //
//													2025/01/23	//
// 														sanda	//
//--------------------------------------------------------------//
#pragma once

struct sanMotionData
{
	UINT Magic;
	UINT Date;
	UINT Version;
	UINT HeaderSize;

	float Length;
	UINT ChannelNum;
	UINT KeyFrameNum;
	UINT Reserve;

	UINT ChannelAccess;
	UINT KeyFrameAccess;
	UINT Reserve1;
	UINT Reserve2;

	UINT Align[4];
};

enum eMotionChannel
{
	PosX,
	PosY,
	PosZ,
	RotX,
	RotY,
	RotZ,
	SclX,
	SclY,
	SclZ,
	QutX,
	QutY,
	QutZ,
	QutW,
	ChannelMax,
};

struct sanMotionData_Channel
{
	char Name[32];   // ���ʂ̖��O
	int ChannelID;   // eMotionChannel�̃p�[�c�ԍ�
	int KeyFrameNum; // �L�[�t���[��
	int StartIndex;  // �n�܂�̔ԍ�
	int Reserve;
};

struct sanMotionData_KeyFrame
{
	float	Time;
	float	Value;
};

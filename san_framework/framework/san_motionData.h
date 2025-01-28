//--------------------------------------------------------------//
//	"san_motionData.h"											//
//		モーションデータ定義									    //
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
	char Name[32];   // 部位の名前
	int ChannelID;   // eMotionChannelのパーツ番号
	int KeyFrameNum; // キーフレーム
	int StartIndex;  // 始まりの番号
	int Reserve;
};

struct sanMotionData_KeyFrame
{
	float	Time;
	float	Value;
};

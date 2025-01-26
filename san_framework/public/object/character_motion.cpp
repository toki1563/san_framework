#include "../../framework.h"
#include "../../framework/san_environment.h"

#include "character.h"

stMotion motion_idle;
stMotion motion_walk;

bool initialize_motion_data = false;

void createCharacterMotion()
{
	if (initialize_motion_data == true)return;

	int i = 0;

	motion_idle.Length = 120;
	motion_idle.ChannelNum = cCharacter::PartsMax * stMotion::ChannelMax;
	motion_idle.pChannel = new stMotionChannel[motion_idle.ChannelNum];

	motion_idle.pChannel[i].partsID = cCharacter::Body;	motion_idle.pChannel[i].channelID = stMotion::PosX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Body;	motion_idle.pChannel[i].channelID = stMotion::PosY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Body;	motion_idle.pChannel[i].channelID = stMotion::PosZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Body;	motion_idle.pChannel[i].channelID = stMotion::RotX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Body;	motion_idle.pChannel[i].channelID = stMotion::RotY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Body;	motion_idle.pChannel[i].channelID = stMotion::RotZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Head;	motion_idle.pChannel[i].channelID = stMotion::PosX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Head;	motion_idle.pChannel[i].channelID = stMotion::PosY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Head;	motion_idle.pChannel[i].channelID = stMotion::PosZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Head;	motion_idle.pChannel[i].channelID = stMotion::RotX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Head;	motion_idle.pChannel[i].channelID = stMotion::RotY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::Head;	motion_idle.pChannel[i].channelID = stMotion::RotZ;
	motion_idle.pChannel[i].keyframeNum = 5;
	motion_idle.pChannel[i].pKey = new stKeyframe[motion_idle.pChannel[i].keyframeNum];
	motion_idle.pChannel[i].pKey[0].time = 0;	motion_idle.pChannel[i].pKey[0].value = 0.0f;
	motion_idle.pChannel[i].pKey[1].time = 30;	motion_idle.pChannel[i].pKey[1].value = 0.0872f;
	motion_idle.pChannel[i].pKey[2].time = 60;	motion_idle.pChannel[i].pKey[2].value = 0.0f;
	motion_idle.pChannel[i].pKey[3].time = 90;	motion_idle.pChannel[i].pKey[3].value = -0.0872f;
	motion_idle.pChannel[i].pKey[4].time = 120;motion_idle.pChannel[i].pKey[4].value = 0.0f;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmL;	motion_idle.pChannel[i].channelID = stMotion::PosX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmL;	motion_idle.pChannel[i].channelID = stMotion::PosY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmL;	motion_idle.pChannel[i].channelID = stMotion::PosZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmL;	motion_idle.pChannel[i].channelID = stMotion::RotX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmL;	motion_idle.pChannel[i].channelID = stMotion::RotY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmL;	motion_idle.pChannel[i].channelID = stMotion::RotZ;
	motion_idle.pChannel[i].keyframeNum = 5;
	motion_idle.pChannel[i].pKey = new stKeyframe[motion_idle.pChannel[i].keyframeNum];
	motion_idle.pChannel[i].pKey[0].time = 0;	motion_idle.pChannel[i].pKey[0].value = 0.4606f;
	motion_idle.pChannel[i].pKey[1].time = 30;	motion_idle.pChannel[i].pKey[1].value = 0.5632f;
	motion_idle.pChannel[i].pKey[2].time = 60;	motion_idle.pChannel[i].pKey[2].value = 0.4606f;
	motion_idle.pChannel[i].pKey[3].time = 90;	motion_idle.pChannel[i].pKey[3].value = 0.5632f;
	motion_idle.pChannel[i].pKey[4].time = 120; motion_idle.pChannel[i].pKey[4].value = 0.4606f;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmR;	motion_idle.pChannel[i].channelID = stMotion::PosX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmR;	motion_idle.pChannel[i].channelID = stMotion::PosY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmR;	motion_idle.pChannel[i].channelID = stMotion::PosZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmR;	motion_idle.pChannel[i].channelID = stMotion::RotX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmR;	motion_idle.pChannel[i].channelID = stMotion::RotY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::ArmR;	motion_idle.pChannel[i].channelID = stMotion::RotZ;
	motion_idle.pChannel[i].keyframeNum = 5;
	motion_idle.pChannel[i].pKey = new stKeyframe[motion_idle.pChannel[i].keyframeNum];
	motion_idle.pChannel[i].pKey[0].time = 0;	motion_idle.pChannel[i].pKey[0].value = -0.4606f;
	motion_idle.pChannel[i].pKey[1].time = 30;	motion_idle.pChannel[i].pKey[1].value = -0.5632f;
	motion_idle.pChannel[i].pKey[2].time = 60;	motion_idle.pChannel[i].pKey[2].value = -0.4606f;
	motion_idle.pChannel[i].pKey[3].time = 90;	motion_idle.pChannel[i].pKey[3].value = -0.5632f;
	motion_idle.pChannel[i].pKey[4].time = 120; motion_idle.pChannel[i].pKey[4].value = -0.4606f;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegL;	motion_idle.pChannel[i].channelID = stMotion::PosX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegL;	motion_idle.pChannel[i].channelID = stMotion::PosY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegL;	motion_idle.pChannel[i].channelID = stMotion::PosZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegL;	motion_idle.pChannel[i].channelID = stMotion::RotX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegL;	motion_idle.pChannel[i].channelID = stMotion::RotY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegL;	motion_idle.pChannel[i].channelID = stMotion::RotZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegR;	motion_idle.pChannel[i].channelID = stMotion::PosX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegR;	motion_idle.pChannel[i].channelID = stMotion::PosY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegR;	motion_idle.pChannel[i].channelID = stMotion::PosZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegR;	motion_idle.pChannel[i].channelID = stMotion::RotX;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegR;	motion_idle.pChannel[i].channelID = stMotion::RotY;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;

	motion_idle.pChannel[i].partsID = cCharacter::LegR;	motion_idle.pChannel[i].channelID = stMotion::RotZ;
	motion_idle.pChannel[i].keyframeNum = 0;
	motion_idle.pChannel[i].pKey = NULL;
	i++;


	//Walk
	i = 0;
	motion_walk.Length = 60;
	motion_walk.ChannelNum = cCharacter::PartsMax * stMotion::ChannelMax;
	motion_walk.pChannel = new stMotionChannel[motion_walk.ChannelNum];

	motion_walk.pChannel[i].partsID = cCharacter::Body;	motion_walk.pChannel[i].channelID = stMotion::PosX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Body;	motion_walk.pChannel[i].channelID = stMotion::PosY;
	motion_walk.pChannel[i].keyframeNum = 5;
	motion_walk.pChannel[i].pKey = new stKeyframe[motion_walk.pChannel[i].keyframeNum];
	motion_walk.pChannel[i].pKey[0].time = 0;	motion_walk.pChannel[i].pKey[0].value = 0.73f;
	motion_walk.pChannel[i].pKey[1].time = 15;	motion_walk.pChannel[i].pKey[1].value = 0.70f;
	motion_walk.pChannel[i].pKey[2].time = 30;	motion_walk.pChannel[i].pKey[2].value = 0.73f;
	motion_walk.pChannel[i].pKey[3].time = 45;	motion_walk.pChannel[i].pKey[3].value = 0.70f;
	motion_walk.pChannel[i].pKey[4].time = 60;	motion_walk.pChannel[i].pKey[4].value = 0.73f;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Body;	motion_walk.pChannel[i].channelID = stMotion::PosZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Body;	motion_walk.pChannel[i].channelID = stMotion::RotX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Body;	motion_walk.pChannel[i].channelID = stMotion::RotY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Body;	motion_walk.pChannel[i].channelID = stMotion::RotZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Head;	motion_walk.pChannel[i].channelID = stMotion::PosX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Head;	motion_walk.pChannel[i].channelID = stMotion::PosY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Head;	motion_walk.pChannel[i].channelID = stMotion::PosZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Head;	motion_walk.pChannel[i].channelID = stMotion::RotX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Head;	motion_walk.pChannel[i].channelID = stMotion::RotY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::Head;	motion_walk.pChannel[i].channelID = stMotion::RotZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmL;	motion_walk.pChannel[i].channelID = stMotion::PosX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmL;	motion_walk.pChannel[i].channelID = stMotion::PosY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmL;	motion_walk.pChannel[i].channelID = stMotion::PosZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmL;	motion_walk.pChannel[i].channelID = stMotion::RotX;
	motion_walk.pChannel[i].keyframeNum = 5;
	motion_walk.pChannel[i].pKey = new stKeyframe[motion_walk.pChannel[i].keyframeNum];
	motion_walk.pChannel[i].pKey[0].time = 0;	motion_walk.pChannel[i].pKey[0].value = 0.0f;
	motion_walk.pChannel[i].pKey[1].time = 15;	motion_walk.pChannel[i].pKey[1].value = -0.8727f;
	motion_walk.pChannel[i].pKey[2].time = 30;	motion_walk.pChannel[i].pKey[2].value = 0.0f;
	motion_walk.pChannel[i].pKey[3].time = 45;	motion_walk.pChannel[i].pKey[3].value = 0.8727f;
	motion_walk.pChannel[i].pKey[4].time = 60;	motion_walk.pChannel[i].pKey[4].value = 0.0f;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmL;	motion_walk.pChannel[i].channelID = stMotion::RotY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmL;	motion_walk.pChannel[i].channelID = stMotion::RotZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmR;	motion_walk.pChannel[i].channelID = stMotion::PosX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmR;	motion_walk.pChannel[i].channelID = stMotion::PosY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmR;	motion_walk.pChannel[i].channelID = stMotion::PosZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmR;	motion_walk.pChannel[i].channelID = stMotion::RotX;
	motion_walk.pChannel[i].keyframeNum = 5;
	motion_walk.pChannel[i].pKey = new stKeyframe[motion_walk.pChannel[i].keyframeNum];
	motion_walk.pChannel[i].pKey[0].time = 0;	motion_walk.pChannel[i].pKey[0].value = 0.0f;
	motion_walk.pChannel[i].pKey[1].time = 15;	motion_walk.pChannel[i].pKey[1].value = 0.8727f;
	motion_walk.pChannel[i].pKey[2].time = 30;	motion_walk.pChannel[i].pKey[2].value = 0.0f;
	motion_walk.pChannel[i].pKey[3].time = 45;	motion_walk.pChannel[i].pKey[3].value = -0.8727f;
	motion_walk.pChannel[i].pKey[4].time = 60;	motion_walk.pChannel[i].pKey[4].value = 0.0f;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmR;	motion_walk.pChannel[i].channelID = stMotion::RotY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::ArmR;	motion_walk.pChannel[i].channelID = stMotion::RotZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegL;	motion_walk.pChannel[i].channelID = stMotion::PosX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegL;	motion_walk.pChannel[i].channelID = stMotion::PosY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegL;	motion_walk.pChannel[i].channelID = stMotion::PosZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegL;	motion_walk.pChannel[i].channelID = stMotion::RotX;
	motion_walk.pChannel[i].keyframeNum = 5;
	motion_walk.pChannel[i].pKey = new stKeyframe[motion_walk.pChannel[i].keyframeNum];
	motion_walk.pChannel[i].pKey[0].time =  0;	motion_walk.pChannel[i].pKey[0].value = 0.0f;
	motion_walk.pChannel[i].pKey[1].time = 15;	motion_walk.pChannel[i].pKey[1].value = 0.8727f;
	motion_walk.pChannel[i].pKey[2].time = 30;	motion_walk.pChannel[i].pKey[2].value = 0.0f;
	motion_walk.pChannel[i].pKey[3].time = 45;	motion_walk.pChannel[i].pKey[3].value = -0.8727f;
	motion_walk.pChannel[i].pKey[4].time = 60;	motion_walk.pChannel[i].pKey[4].value = 0.0f;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegL;	motion_walk.pChannel[i].channelID = stMotion::RotY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegL;	motion_walk.pChannel[i].channelID = stMotion::RotZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegR;	motion_walk.pChannel[i].channelID = stMotion::PosX;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegR;	motion_walk.pChannel[i].channelID = stMotion::PosY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegR;	motion_walk.pChannel[i].channelID = stMotion::PosZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegR;	motion_walk.pChannel[i].channelID = stMotion::RotX;
	motion_walk.pChannel[i].keyframeNum = 5;
	motion_walk.pChannel[i].pKey = new stKeyframe[motion_walk.pChannel[i].keyframeNum];
	motion_walk.pChannel[i].pKey[0].time = 0;	motion_walk.pChannel[i].pKey[0].value = 0.0f;
	motion_walk.pChannel[i].pKey[1].time = 15;	motion_walk.pChannel[i].pKey[1].value = -0.8727f;
	motion_walk.pChannel[i].pKey[2].time = 30;	motion_walk.pChannel[i].pKey[2].value = 0.0f;
	motion_walk.pChannel[i].pKey[3].time = 45;	motion_walk.pChannel[i].pKey[3].value = 0.8727f;
	motion_walk.pChannel[i].pKey[4].time = 60;	motion_walk.pChannel[i].pKey[4].value = 0.0f;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegR;	motion_walk.pChannel[i].channelID = stMotion::RotY;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	motion_walk.pChannel[i].partsID = cCharacter::LegR;	motion_walk.pChannel[i].channelID = stMotion::RotZ;
	motion_walk.pChannel[i].keyframeNum = 0;
	motion_walk.pChannel[i].pKey = NULL;
	i++;

	initialize_motion_data = true;
}

void deleteCharacterMotion()
{
	if (initialize_motion_data == false)return;

	initialize_motion_data = false;

	/*ÉÅÉÇÉäÇÃçÌèú*/

}
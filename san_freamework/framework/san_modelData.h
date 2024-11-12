//--------------------------------------------------------------//
//	"san_modelData.h"											//
//	     ���f���f�[�^��`(*.vnm / �o�C�i���t�@�C��)				//
//													2024/11/12	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

// �o�[�W��������
#define sanMODEL_DATA_LATEST_VER (0x20240101)

struct sanModelData
{
	UINT Magic;
	UINT Data;
	UINT Version;
	UINT HeaderSize;

	UINT MaterialNum;
	UINT MeshNum;
	UINT VertexNum;
	UINT IndexNum;

	UINT MaterialAccess;
	UINT MeshAccess;
	UINT VertexAccess;
	UINT IndexAccess;

	UINT Align[4];
};

struct sanModel_MaterialData
{
	UINT StructSize;
	UINT TextureNum;
	UINT FloatNum;
	UINT Flag;
	char Texture[32];
	float Diffuse[4];
	float Ambient[4];
	float Specular[4];
};

struct sanModel_MeshData
{
	UINT MaterialID;
	UINT IndexNum;
	UINT StartIndex;
	UINT StartVertex;
};

// .bone�t�@�C���̒��g
struct sanModel_BoneData
{
	char Name[32];	// �{�[���̖��O
	int ParentID;	// �e�̔ԍ�(-�͐e���Ȃ�)
	int Generation; // ����
	int StructSize; // �q�̍\���̂̃T�C�Y(Byte)
	float pos[3];	// �W����Ԃł�Position(x, y, z)
	float rot[3];	// �W����Ԃł�Rotation(x, y, z)
	float scl[3];	// �W����Ԃł�Scale(x, y, z)
};
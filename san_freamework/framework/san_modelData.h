//--------------------------------------------------------------//
//	"san_modelData.h"											//
//	     モデルデータ定義(*.vnm / バイナリファイル)				//
//													2024/11/12	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

// バージョン履歴
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

// .boneファイルの中身
struct sanModel_BoneData
{
	char Name[32];	// ボーンの名前
	int ParentID;	// 親の番号(-は親がない)
	int Generation; // 世代
	int StructSize; // 子の構造体のサイズ(Byte)
	float pos[3];	// 標準状態でのPosition(x, y, z)
	float rot[3];	// 標準状態でのRotation(x, y, z)
	float scl[3];	// 標準状態でのScale(x, y, z)
};
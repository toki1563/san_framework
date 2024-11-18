//--------------------------------------------------------------//
//	"san_modelData.h"											//
//	     モデルデータ構造(*.vnm/バイナリファイル)					//
//													2024/11/12	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

// バージョン定義
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

	// 各データへのアクセス
	UINT MaterialAccess;
	UINT MeshAccess;
	UINT VertexAccess;
	UINT IndexAccess;

	UINT Align[4]; // アライメント(4つのUINTでバディング)
};

struct sanModel_MaterialData
{
	UINT StructSize;   // 構造体サイズ
	UINT TextureNum;   // テクスチャの数
	UINT FloatNum;	   // 浮動店小数点の数
	UINT Flag;         // フラグ
	char Texture[32];  // テクスチャ名
	float Diffuse[4];  // ディフューズカラー(RGBA)
	float Ambient[4];  // アンビエントカラー(RGBA)
	float Specular[4]; // スペキュラーカラー(RGBA)
};

struct sanModel_MeshData
{
	UINT MaterialID;  // 使用するマテリアルのID
	UINT IndexNum;    // インデックス番号
	UINT StartIndex;  // 開始インデックス
	UINT StartVertex; // 開始頂点
};

// .boneファイルの構造
struct sanModel_BoneData
{
	char Name[32];	// ボーン名
	int ParentID;	// 親の番号(-1は親なし)
	int Generation; // 世代
	int StructSize; // 構造体サイズ(Byte)
	float pos[3];	// 位置(Position : x, y, z)
	float rot[3];	// 回転(Rotation : x, y, z)
	float scl[3];	// スケール(Scale : x, y, z)
};
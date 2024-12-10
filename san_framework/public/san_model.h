//--------------------------------------------------------------//
//	"san_model.h"												//
//	     モデルデータ描画クラス									//
//													2024/11/16	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

#include "san_object.h"

class sanModel : public sanObject
{
protected:
	struct stMaterial
	{
		// カラー
		XMVECTOR Diffuse;  // 拡散色		
		XMVECTOR Ambient;  // 環境色
		XMVECTOR Specular; // 鏡面反射色
		// テクスチャ
		ID3D12Resource* texbuff;
		// 定数バッファ
		ID3D12Resource* constbuff;
		ID3D12DescriptorHeap* basicDescHeap;
		stConstantBuffer* pConstBuffer;
	};

	// モデルデータ
	sanModelData *pModelData;

	// マテリアル
	stMaterial   *pMaterials;

	// 頂点バッファ
	ID3D12Resource* pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// インデックスバッファ
	ID3D12Resource* pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

public:
	// コンストラクタ
	sanModel(const WCHAR *folder, const WCHAR *file);
	// デストラクタ
	~sanModel();

	// システム関数
	virtual void execute();
	virtual void render();

	// モデルデータ(vnmファイルの情報)
	int getVertexNum();
	int getIndexNum();
	int getFaceNum();
	int getMaterialNum();
	int getMeshNum();
	const char *getTextureName(int id);

	sanModel_MeshData *getMesh(); // メッシュデータ(先頭アドレス)の取得
	sanVertex3D *getVertex();	  // 頂点データ(先頭アドレス)の取得
	LPWORD getIndex();			  // インデックスデータ(先頭アドレス)の取得

	// 拡散色の設定
	void setDiffuse(float r, float g, float b, float a, int id = -1);
	void setDiffuse(const XMVECTOR *v, int id = -1);
	void setDiffuseR(float value, int id = -1);
	void setDiffuseG(float value, int id = -1);
	void setDiffuseB(float value, int id = -1);
	void setDiffuseA(float value, int id = -1);
	void addDiffuseR(float value, int id = -1);
	void addDiffuseG(float value, int id = -1);
	void addDiffuseB(float value, int id = -1);
	void addDiffuseA(float value, int id = -1);

	// 拡散色の取得
	XMVECTOR *getDiffuse(int id = 0);
	float getDiffuseR(int id = 0);
	float getDiffuseG(int id = 0);
	float getDiffuseB(int id = 0);
	float getDiffuseA(int id = 0);

	// 環境色の設定
	void setAmbient(float r, float g, float b, float a, int id = -1);
	void setAmbient(const XMVECTOR *v, int id = -1);
	void setAmbientR(float value, int id = -1);
	void setAmbientG(float value, int id = -1);
	void setAmbientB(float value, int id = -1);
	void setAmbientA(float value, int id = -1);
	void addAmbientR(float value, int id = -1);
	void addAmbientG(float value, int id = -1);
	void addAmbientB(float value, int id = -1);
	void addAmbientA(float value, int id = -1);

	// 環境色の取得
	XMVECTOR *getAmbient(int id = 0);
	float getAmbientR(int id = 0);
	float getAmbientG(int id = 0);
	float getAmbientB(int id = 0);
	float getAmbientA(int id = 0);

	// 鏡面反射色の設定
	void setSpecular(float r, float g, float b, float p, int id = -1);
	void setSpecular(const XMVECTOR *v, int id = -1);
	void setSpecularR(float value, int id = -1);
	void setSpecularG(float value, int id = -1);
	void setSpecularB(float value, int id = -1);
	void setSpecularPower(float value, int id = -1);
	void addSpecularR(float value, int id = -1);
	void addSpecularG(float value, int id = -1);
	void addSpecularB(float value, int id = -1);
	void addSpecularPower(float value, int id = -1);

	// 鏡面反射色の取得
	const XMVECTOR *getSpecular(int id = 0);
	float getSpecularR(int id = 0);
	float getSpecularG(int id = 0);
	float getSpecularB(int id = 0);
	float getSpecularPower(int id = 0);
};
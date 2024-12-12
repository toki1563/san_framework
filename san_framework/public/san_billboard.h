//--------------------------------------------------------------//
//	"san_billboard.h"											//
//	     ビルボードポリゴン										//
//													2024/11/18	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class sanBillboard : public sanObject
{
private:
	// 基本座標
	XMVECTOR BasePos[4];

	// 頂点データ
	sanVertex3D *vtx;

	// 頂点バッファ
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// テクスチャ
	ID3D12Resource* texBuff;

	// 定数バッファ
	ID3D12Resource* constBuff;
	ID3D12DescriptorHeap* basicDescHeap;
	stConstantBuffer* pConstBuffer;

	// マテリアル関連
	XMVECTOR Diffuse;  // 拡散色
	XMVECTOR Ambient;  // 環境色
	XMVECTOR Specular; // 鏡面反射色

	// 頂点データへの各種情報の設定
	virtual void setVertexPosition();

public:
	// コンストラクタ
	sanBillboard() {} // デフォルトを使用
	sanBillboard(float width, float height, const WCHAR* textuer_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	// デストラクタ
	virtual ~sanBillboard();

	// 処理
	virtual void execute();

	// 描画
	virtual void render();

	// 拡散色の設定
	void setDiffuse(float r, float g, float b, float a);
};
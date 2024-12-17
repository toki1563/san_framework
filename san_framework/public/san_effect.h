//--------------------------------------------------------------//
//	"san_effect.h"									            //
//	     エフェクト(パーティクル)クラス							//
//													2024/12/12	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

// パーティクルクラス
class sanParticle
{
public:
	float Life;		 // 寿命
	float StartLife; // 初期値
	float Size;      // 大きさ

	XMVECTOR Pos; // 座標
	XMVECTOR Vel; // 速度
	XMVECTOR Col; // 色
};

class sanEffect : public sanObject
{
private:
	// パーティクル配列
	sanParticle* pParticle;

	// 描画されるインデックスの数
	int IndexNum;

	// 描画されるパーティクルの数
	int renderParticleNum;

	// 頂点データ
	sanVertex3D* vtx;

	// インデックスデータ
	WORD* idx;

	// 頂点バッファ
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// インデックスバッファ
	ID3D12Resource* pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	// テクスチャ
	ID3D12Resource* texbuff;
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
	sanEffect();
	virtual ~sanEffect();

	virtual void execute();

	virtual void render();
};
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
public:
	struct stEffectDesc
	{
		// エミットの設定(パーティクルのパラメータ)
		float LifeMin;
		float LifeMax;
		XMVECTOR VelocityMin;
		XMVECTOR VelocityMax;
		float SizeMin;
		float SizeMax;
		stEffectDesc()
		{
			LifeMin = 60.0f;
			LifeMax = 12.0f;
			VelocityMin = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
			VelocityMax = XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
			SizeMin = 0.0f;
			SizeMax = 1.0;
		}
	};
	
private:
	// パーティクル配列
	sanParticle* pParticle;

	bool emit;       // 放出するかどうか
	float emitCount; // 放出持続時間

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
	sanEffect(stEffectDesc * pDese);
	virtual ~sanEffect();

	virtual void execute();

	virtual void render();

	// 放出設定
	void setEmit(bool flag);
	bool isEmit();

	// 放出持続時間
	void setEmitCount(float value);
	float getEmitCount();
};
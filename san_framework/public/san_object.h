﻿//--------------------------------------------------------------//
//	"san_object.h"												//
//	     オブジェクト基底クラス									//
//													2024/11/13	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once
#include "../framework/san_modelData.h"
#include "../framework/san_motionData.h"

// ポリゴン頂点構造体
struct sanVertex3D
{
	float x, y, z;	  // 座標
	float nx, ny, nz; // 法線
	float r, g, b, a; // カラー
	float u, v;		  // UV
};

class sanObject
{
protected:
	// 移動値
	XMVECTOR	Position;

	// 回転値
	XMVECTOR	Rotation;

	// 拡大値
	XMVECTOR	Scale;

	// 変換行列
	XMMATRIX	Local;
	XMMATRIX	World;

	// 実行状態の管理
	bool executeEnable;

	// 描画状態の管理
	bool renderEnable;

	// ライティングの有効/無効
	bool lighting;

	// 半透明有効/無効
	bool transparent;

	// 深度書き込みの有効/無効
	bool zWrite;

	// 階段構造の親
	sanObject* pParent;

	// マトリクスの計算(Local/World)
	void calculateLocalMatrix();
	void calculateWorldMatrix();

	// 静的共有データ
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[]; // 頂点要素
	static ID3D12PipelineState *pPipelineState;          // パイプラインステート
	static ID3D12PipelineState *pPipelineState_NL;		 // ライティングなし
	static ID3D12PipelineState *pPipelineState_Alpha;	 // 半透明
	static ID3D12PipelineState *pPipelineState_Alpha_NL; // 半透明＆ライティングなし
	// 深度書き込みなし
	static ID3D12PipelineState *pPipelineState_ZOff;          // パイプラインステート
	static ID3D12PipelineState *pPipelineState_NL_ZOff;		  // ライティングなし
	static ID3D12PipelineState *pPipelineState_Alpha_ZOff;	  // 半透明
	static ID3D12PipelineState *pPipelineState_Alpha_NL_ZOff; // 半透明＆ライティングなし

	// コンスタントバッファ構造体
	struct stConstantBuffer
	{
		XMFLOAT4X4 WVP;		  // World * View * Proj
		XMFLOAT4X4 World;	  // Workd
		XMFLOAT4   LightDir;  // 平行光源の(逆)方向
		XMFLOAT4   LightCol;  // 並行光源の色
		XMFLOAT4   LightAmb;  // 環境光の色
		XMFLOAT4   CameraPos; // カメラのワールド座標
		XMFLOAT4   Diffuse;	  // マテリアルの拡散色
		XMFLOAT4   Ambient;	  // マテリアルの環境色
		XMFLOAT4   Specular;  // マテリアルの鏡面反射光
	};

public:
	// 静的共有データ初期化
	static bool initializeCommon();

	// 静的共通データ削除
	static void terminateCommon();

	// コンストラクタ
	sanObject();

	// デストラクタ
	virtual ~sanObject();

	// システム関数
	virtual void execute();
	virtual void render();

	// 移動値の設定
	void setPosition(float x, float y, float z);
	void setPosition(const XMVECTOR *v);
	void setPositionX(float value);
	void setPositionY(float value);
	void setPositionZ(float value);
	void addPosition(float x, float y, float z);
	void addPosition(const XMVECTOR *v);
	void addPositionX(float value);
	void addPositionY(float value);
	void addPositionZ(float value);

	// 移動量の取得
	XMVECTOR *getPosition();
	float getPositionX(void);
	float getPositionY(void);
	float getPositionZ(void);

	// 回転値の設定
	void setRotation(float x, float y, float z);
	void setRotation(const XMVECTOR *v);
	void setRotationX(float radian);
	void setRotationY(float radian);
	void setRotationZ(float radian);
	void addRotation(float x, float y, float z);
	void addRotation(const XMVECTOR *v);
	void addRotationX(float radian);
	void addRotationY(float radian);
	void addRotationZ(float radian);

	// 回転値の取得
	XMVECTOR *getRotation();
	float getRotationX(void);
	float getRotationY(void);
	float getRotationZ(void);

	// 拡大値の設定
	void setScale(float x, float y, float z);
	void setScale(const XMVECTOR *v);
	void setScaleX(float value);
	void setScaleY(float value);
	void setScaleZ(float value);
	void addScale(float x, float y, float z);
	void addScale(const XMVECTOR *v);
	void addScaleX(float value);
	void addScaleY(float value);
	void addScaleZ(float value);

	// 拡大値の取得
	XMVECTOR *getScale();
	float getScaleX(void);
	float getScaleY(void);
	float getScaleZ(void);

	// ワールド行列の取得
	XMMATRIX *getWorld();

	// ワールド座標の取得
	void getWorldPosition(XMVECTOR *v);
	float getWorldPositionX();
	float getWorldPositionY();
	float getWorldPositionZ();

	// ワールド行列の設定
	void setWorld(XMMATRIX *m);

	// 実行状態の設定
	void setExecuteEnble(bool flag);

	// 描画状態の設定
	void setRenderEnable(bool flag);

	// 実行状態の取得
	bool isExcuteEnable();

	// 描画状態の取得
	bool isRenderEnable();

	// ライティングの有効/無効の設定
	void setLighting(bool flag);

	// 半透明の有効/向こうの設定
	void setTransparent(bool flag);

	// 深度書き込みの有効/無効の設定
	void setZWrite(bool flag);

	// 深度書き込みの有効/無効の取得
	bool getZWrite();

	// ライティングの有効/向こうの取得
	bool getLighting();

	// 半透明の有効/無効の取得
	bool getTransparent();

	// 階層構造の親の設定
	void setParent(sanObject* p);

	// 階層構造の親の取得
	sanObject* getParent(void);
};
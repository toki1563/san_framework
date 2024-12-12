//--------------------------------------------------------------//
//	"san_Direct2D.h"											//
//	     Direct2D管理											//
//													2024/11/01	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class sanDirect2D
{
private:
	static ID3D11On12Device* pDevice11On12; // D3D11on12デバイス
	static ID3D11DeviceContext* pDeviceContext11; // D3D11デバイスコンテキスト

	static ID2D1Factory3* pD2dFactory; // Direct2Dファクトリー
	static ID2D1Device2* pD2dDevice;   // Direct2Dデバイス
	static ID2D1DeviceContext2* pD2dDeviceContext; // Direct2Dデバイスコンテキスト
	static IDWriteFactory* pDWFactory; // DirectWriteファクトリー

	// バックバッファーとして使用するD3D11リソース
	static ID3D11Resource* pWrappedBackBuffers[sanDirect3D::frameCount];
	static ID2D1Bitmap1* pD2dRenderTargets[sanDirect3D::frameCount]; // D2Dレンダーターゲット

public:
	//フレームワーク管理
	static int initialize(void);
	static void terminate(void);

	static void render();

	// インターフェース関連の取得
	static ID2D1DeviceContext2* getDeviceContext();
	static IDWriteFactory* getDWFactory();
};
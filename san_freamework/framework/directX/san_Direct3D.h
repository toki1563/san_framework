#pragma once

class sanDirect3D
{

private:
	static IDXGIFactory4* pFactory; // DXGIファクトリー
	static IDXGIAdapter3* pAdapter; // アダプター
	static ID3D12Device* pDevice; // D3D12デバイス
	static IDXGISwapChain4* pSwapChain; // スワップチェイン
	static ID3D12CommandQueue* pCmdQueue; // コマンドキュー


	static ID3D12CommandAllocator* pCmdAllocator; // コマンドアロケーター
	static ID3D12GraphicsCommandList* pCmdList; // コマンドリスト

	// 初期化関数
	static void createFactory();
	static void createDevice();
	static void createCommandQueue();

	static void createCommandList();

public:
	// フレームワーク管理
	static int initialize(void);
};
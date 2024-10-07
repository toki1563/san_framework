// Direct3D管理
#pragma once

class sanDirect3D
{
private:
	static IDXGIFactory4* pFactory; // DXGIファクトリー
	static IDXGIAdapter3* pAdapter; // アダプター
	static ID3D12Device* pDevice; // D3D12デバイス
	static IDXGISwapChain4* pSwapChain; // スワップチェイン
	static ID3D12CommandQueue* pCmdQueue; // コマンドキュー
	static HANDLE              hFenceEvent; // フェンスイベント

	static ID3D12Fence* pQueueFence; // コマンドキュー用フェンス
	static ID3D12CommandAllocator* pCmdAllocator; // コマンドアロケーター
	static ID3D12GraphicsCommandList* pCmdList; // コマンドリスト

	static UINT fenceValue;

	// 初期化関数
	static void createFactory();
	static void createDevice();
	static void createCommandQueue();
	static void createSwapChain();


	static void createCommandList();

public:
	// フレームワーク管理
	static int initialize(void);
};
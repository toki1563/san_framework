//--------------------------------------------------------------//
//	"san_Direct3D.h"											//
//	     Direct3D管理											//
//													2024/10/17	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class sanDirect3D
{
public:
	static constexpr UINT frameCount = 2;
	
private:
	static UINT rtvIndex;

	static IDXGIFactory4* pFactory; // DXGIファクトリー
	static IDXGIAdapter3* pAdapter; // アダプター
	static ID3D12Device* pDevice; // D3D12デバイス
	static IDXGISwapChain4* pSwapChain; // スワップチェイン
	static ID3D12CommandQueue* pCmdQueue; // コマンドキュー
	static HANDLE              hFenceEvent; // フェンスイベント
	static ID3D12Fence*		pQueueFence;  // コマンドキュー用フェンス
	static ID3D12DescriptorHeap* pDH_RTV; // ディスクリプタヒープ(RenderTargetView)
	static ID3D12DescriptorHeap* pDH_DSV; // ディスクリプタヒープ(DepthStencilView)
	static D3D12_CPU_DESCRIPTOR_HANDLE hRTV[frameCount]; //ディスクリプタハンドル(RenderTargetView)
	static D3D12_CPU_DESCRIPTOR_HANDLE hDSV;			 //ディスクリプタハンドル(DepthStencilView)
	static ID3D12Resource* pRenderTarget[frameCount]; // レンダーターゲット
	static ID3D12Resource* pDepthBuffer;
	static ID3D12CommandAllocator* pCmdAllocator; // コマンドアロケーター
	static ID3D12GraphicsCommandList* pCmdList; // コマンドリスト
	static ID3D12RootSignature* pRootSignature; // ルートシグネチャ

	static UINT fenceValue;

	static ID3D12Resource* pWhiteTex;
	static D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// 初期化関数
	static void createFactory();
	static void createDevice();
	static void createCommandQueue();
	static void createSwapChain();
	static void createRenderTargetView();
	static void createDepthStencilBuffer();
	static void createCommandList();
	static void createRootSignature();

	static void createWhiteTexture();

public:
	// フレームワーク管理
	static int initialize(void);
	static void terminate(void);

	// 描画システムコール
	static void beginRender(void);
	static void finishRender(void);
	static void present(void);

	// インターフェース関連の取得
	static ID3D12Device* getDevice(void);
	static ID3D12CommandQueue* getCommandQueue(void);
	static ID3D12RootSignature* getRootSignature(void);

	static ID3D12GraphicsCommandList* getCommandList(void);

	static ID3D12Resource* getRenderTarget(int id);

	static UINT getIndex(void);

	static ID3D12Resource* getWhiteTexture();
	static D3D12_SHADER_RESOURCE_VIEW_DESC* getWhiteTextureViewDesc();
};
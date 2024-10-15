// Direct3D管理
#include "../../framework.h"
#include "../san_environment.h"

extern HWND hWnd; // ウィンドウハンドル

UINT sanDirect3D::rtvIndex = 0;

IDXGIFactory4* sanDirect3D::pFactory = NULL; // DXGIファクトリー
IDXGIAdapter3* sanDirect3D::pAdapter = NULL; // アダプター
ID3D12Device* sanDirect3D::pDevice = NULL; // D3D12デバイス
IDXGISwapChain4* sanDirect3D::pSwapChain = NULL; // スワップチェイン
ID3D12CommandQueue* sanDirect3D::pCmdQueue = NULL; // コマンドキュー
HANDLE              sanDirect3D::hFenceEvent = NULL; // フェンスイベント
ID3D12Fence* sanDirect3D::pQueueFence = NULL; // コマンドキュー用フェンス
ID3D12DescriptorHeap* sanDirect3D::pDH_RTV = NULL;	// ディスクリプタヒープ(RenderTargetView)
ID3D12DescriptorHeap* sanDirect3D::pDH_DSV = NULL;	// ディスクリプタヒープ(DepthStencilView)
D3D12_CPU_DESCRIPTOR_HANDLE	sanDirect3D::hRTV[frameCount];	// ディスクリプタハンドル(RenderTargetView)
D3D12_CPU_DESCRIPTOR_HANDLE	sanDirect3D::hDSV;				// ディスクリプタハンドル(DepthStencilView)
ID3D12Resource* sanDirect3D::pRenderTarget[frameCount];	// レンダーターゲット
ID3D12Resource* sanDirect3D::pDepthBuffer = NULL;
ID3D12CommandAllocator* sanDirect3D::pCmdAllocator = NULL; // コマンドアロケーター
ID3D12GraphicsCommandList* sanDirect3D::pCmdList = NULL;   // コマンドリスト
ID3D12RootSignature* sanDirect3D::pRootSignature = NULL;   // ルートシグネチャ

UINT sanDirect3D::fenceValue = 0;

ID3D12Resource* sanDirect3D::pWhiteTex = NULL;
D3D12_SHADER_RESOURCE_VIEW_DESC sanDirect3D::srvDesc = {};

int sanDirect3D::initialize()
{
	HRESULT hr = S_OK;

	createFactory();

	createDevice();

	createCommandQueue();

	createSwapChain();

	createRenderTargetView();

	createDepthStencilBuffer();

	createCommandList();

	createRootSignature();

	createWhiteTexture();

	hr = pCmdList->Close();

	return 1;
}

// DXGIファクトリ作成
// 主にディスプレイやスワップチェインを管理するために使用
void sanDirect3D::createFactory()
{
	HRESULT hr = S_OK; // 初期化
	UINT dxgiFactoryFlag = 0;

	// デバッグモードの場合はデバッグレイヤーを有効にする
#if defined(_DEBUG)
	{
		ID3D12Debug* debug = NULL; // インターフェースの初期化
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug)); // デバッグインターフェース取得
		assert(hr == S_OK);
		debug->EnableDebugLayer(); // デバッグレイヤー
		dxgiFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG; // デバッグ用に生成
		debug->Release(); // インターフェース開放
	}
#endif

	// DXGIファクトリの作成
	hr = CreateDXGIFactory2(dxgiFactoryFlag, IID_PPV_ARGS(&pFactory));
	assert(hr == S_OK);

	// フルスクリーンへの遷移をサポートしない
	// hr = pFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
}

// デバイス生成
// GPUやディスプレイアダプタを使ってグラフィックスリソースを管理・操作するためのインターフェース
void sanDirect3D::createDevice()
{
	HRESULT hr = S_OK; // 初期化

	// アダプターを列挙し、最初に見つかったアダプターを使用する
	hr = pFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter);

#ifdef USE_GPU
	// ディスクリートGPUを検索して使用する
	IDXGIAdapter3* tmpAdapter = NULL;
	for (int i = 1; pFactory->EnumAdapters(i, (IDXGIAdapter**)&tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC1 adesc = {};
		tmpAdapter->GetDesc1(&adesc);
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
		}
		else
		{
			pAdapter = tmpAdapter;
			break;
		}
	}
#endif
	assert(hr == S_OK);

#if 1
	{
		// アダプターの情報をデバッグ
		DXGI_ADAPTER_DESC1 adesc = {};
		pAdapter->GetDesc1(&adesc);
		OutputDebugStringW(L"Selected Adapter : ");
		OutputDebugStringW(adesc.Description);
		OutputDebugStringW(L"\n");
	}
#endif
	// フューチャーレベル列挙
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	// デバイスの作成
	for (int i = 0; i < sizeof(levels) / sizeof(D3D_FEATURE_LEVEL); i++)
	{
		hr = D3D12CreateDevice(
			pAdapter,
			levels[i],
			IID_PPV_ARGS(&pDevice)
		);
		if (hr == S_OK)
		{
			pDevice->SetName(L"sanDirect3D::pDevice");
			break;
		}
	}
	assert(pDevice != NULL);
}

// コマンドキュー生成
// GPUに対して描画や計算の命令(コマンド)を送るためのキュー(列)
void sanDirect3D::createCommandQueue()
{
	HRESULT hr = S_OK; // 初期化

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
	command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // コマンドリストと合わせる
	command_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // 指定なし
	command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // タイムアウトなし
	command_queue_desc.NodeMask = 0;

	// コマンドキューの作成
	hr = pDevice->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&pCmdQueue));
	assert(hr == S_OK);
	pCmdQueue->SetName(L"sanDirect3D::pCmdQueue");

	// フェンスイベントの作成
	hFenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	assert(hFenceEvent != NULL);
	// コマンドキュー用のフェンスの生成
	hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pQueueFence));
	assert(hr == S_OK);
	pQueueFence->SetName(L"sanDirect3D::pQueueFence");

	fenceValue = 1; // フェンスの初期値を1に設定
}

// スワップチェイン作成
// フロントバッファとバックバッファを入れ替える機能
// バックバッファは描画中のフレーム(ユーザーに見えていない)
// フロントバッファは画面に表示されているフレーム(ユーザーに見えている)
void sanDirect3D::createSwapChain()
{
	HRESULT hr = S_OK;

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = (UINT)sanMainFrame::screenWidth;   // 画面解像度[幅]
	swapChainDesc.Height = (UINT)sanMainFrame::screenHeight; // 画面解像度[高]
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // ピクセルフォーマット
	swapChainDesc.Stereo = false;                            // ステレオ表示フラグ
	swapChainDesc.SampleDesc.Count = 1;                      // マルチサンプルの指定
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ダブルバッファーなので2
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	// スワップチェインの作成
	hr = pFactory->CreateSwapChainForHwnd(
		pCmdQueue,
		hWnd,
		&swapChainDesc,
		NULL,
		NULL,
		(IDXGISwapChain1**)&pSwapChain
	);
	assert(hr == S_OK);

	// カレントのバックバッファのインデックスを取得する
	rtvIndex = pSwapChain->GetCurrentBackBufferIndex();

	// フルスクリーンへの遷移をサポートしない
	// スワップチェインの作成後
	hr = pFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
}

// レンダーターゲットの生成
// ディスクリプタヒープはリソースを参照するためのディスクリプタの集合体
// ディスクリプタハンドルはディスクリプタヒープ内の特定のリソースを指し示すポインタ
// レンダーターゲットはGPUが描画結果を出力するバッファのこと
void sanDirect3D::createRenderTargetView()
{
	HRESULT hr = S_OK;

	// RTV用デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
	heap_desc.NumDescriptors = frameCount;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heap_desc.NodeMask = 0;

	// RTV用のデスクリプタヒープの作成
	hr = pDevice->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&pDH_RTV));
	assert(hr == S_OK);
	pDH_RTV->SetName(L"sanDirect3D::pDH_RTV");

	// ディスクリプタのサイズ取得
	UINT size = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// レンダーターゲット
	for (UINT i = 0; i < frameCount; i++)
	{
		// スワップチェインからバッファを受け取る
		hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pRenderTarget[i]));
		assert(hr == S_OK);
		pRenderTarget[i]->SetName(L"sanDirect3D::pRenderTarget");
		// RenderTargetViewを作成してヒープデスクリプタに登録
		hRTV[i] = pDH_RTV->GetCPUDescriptorHandleForHeapStart();
		hRTV[i].ptr += size * i;
		pDevice->CreateRenderTargetView(pRenderTarget[i], NULL, hRTV[i]);
	}
}

// 深度シテンシルバッファ作成
// 3Dレンダリングの際にピクセルの奥行情報を記録し、物体の前後関係を判断
void sanDirect3D::createDepthStencilBuffer()
{
	HRESULT hr = S_OK;

	// 深度バッファ作成
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Alignment = 0;
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2Dテクスチャデータ
	depthResDesc.Width = sanMainFrame::screenWidth;   // レンダーダーゲットと同じ
	depthResDesc.Height = sanMainFrame::screenHeight;
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値書き込み用フォーマット
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.SampleDesc.Quality = 0;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // 深度ステンシルとして使用
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthResDesc.MipLevels = 0;

	// ピーププロパティ設定
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT; // デフォルトを使用
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	depthHeapProp.CreationNodeMask = 0;
	depthHeapProp.VisibleNodeMask = 0;

	// クリアバリュー
	D3D12_CLEAR_VALUE _depthClearValue = {};
	_depthClearValue.DepthStencil.Depth = 1.0f; // 深度バッファの最大値を(1.0)でクリア
	_depthClearValue.DepthStencil.Stencil = 0;
	_depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 32bit深度値としてクリア

	// 深度バッファリソース作成
	hr = pDevice->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度書き込みとして使用
		&_depthClearValue,
		IID_PPV_ARGS(&pDepthBuffer));
	assert(hr == S_OK);
	pDepthBuffer->SetName(L"sanDirect3D::pDepthBuffer");

	// 深度用ディスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビュー1つのみ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // 深度ステンシルビュータイプ使用
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	hr = pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&pDH_DSV));
	assert(hr == S_OK);
	pDH_DSV->SetName(L"sanDirect3D::pDH_DSV");

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	pDevice->CreateDepthStencilView(pDepthBuffer, &dsvDesc, pDH_DSV->GetCPUDescriptorHandleForHeapStart());

	// ハンドルの取得
	hDSV = pDH_DSV->GetCPUDescriptorHandleForHeapStart();
}


// コマンドリスト作成
// DirectX12の描画や計算の指示の一連の命令としてまとめる
void sanDirect3D::createCommandList()
{
	HRESULT hr = S_OK;
	// コマンドアロケーターの作成
	hr = pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));
	assert(hr == S_OK);
	pCmdAllocator->SetName(L"sanDirect3D::pCmdAllocator");

	// コマンドリストの作成(コマンドアロケーターとバインド)
	hr = pDevice->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, NULL, IID_PPV_ARGS(&pCmdList));
	assert(hr == S_OK);
	pCmdList->SetName(L"sanDirect3D::pCmdList");
}

// ルートシグネチャ作成
void sanDirect3D::createRootSignature()
{
	HRESULT hr = S_OK;

	// ルートシグネチャの設定
	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {}; // テクスチャと定数の2つ

	// テクスチャ
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // 種別はテクスチャ
	descTblRange[0].BaseShaderRegister = 0;                      // 0番スロットから
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 定数バッファ
	descTblRange[1].NumDescriptors = 1;	         // 定数一つ
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // 種別は定数
	descTblRange[1].BaseShaderRegister = 0;                      // 0番スロットから
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootparam = {};
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange[0];         // デスクリプタレンジのアドレス
	rootparam.DescriptorTable.NumDescriptorRanges = _countof(descTblRange); // デスクリプタレンジ数
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;               // 全てのシェーダーから見える

	// サンプラーステートの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ルートシグネチャを設定
	D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
	root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	root_signature_desc.NumParameters = 1;
	root_signature_desc.pParameters = &rootparam;
	root_signature_desc.NumStaticSamplers = 1;
	root_signature_desc.pStaticSamplers = &samplerDesc;

	// ルートシグネチャのバイナリコードを作成
	ID3D10Blob* rSigBlob = NULL;
	ID3D10Blob* rSigBlobErr = NULL;
	hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &rSigBlob, &rSigBlobErr);
	assert(hr == S_OK);

	// ルートシグネチャの作成
	hr = pDevice->CreateRootSignature(0, rSigBlob->GetBufferPointer(), rSigBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
	assert(hr == S_OK);
	pRootSignature->SetName(L"sanDirect3D::pRootSignature");

	SAFE_RELEASE(rSigBlob);
	SAFE_RELEASE(rSigBlobErr);
}

void sanDirect3D::createWhiteTexture()
{

}

void sanDirect3D::terminate()
{
	// 安全に解放するためのマクロ
	//SAFE_RELEASE()

	// 安全のため最後に宣言していく
}

void sanDirect3D::beginRender(void)
{

}

void sanDirect3D::beginRender2(void)
{

}

void sanDirect3D::finishRender(void)
{

}

void sanDirect3D::present(void)
{

}

ID3D12Device* sanDirect3D::getDevice(void)
{
	return pDevice;
}

ID3D12CommandQueue* sanDirect3D::getCommandQueue(void)
{
	return pCmdQueue;
}

ID3D12RootSignature* sanDirect3D::getRootSignature(void)
{
	return pRootSignature;
}

ID3D12GraphicsCommandList* sanDirect3D::getCommandList(void)
{
	return pCmdList;
}

ID3D12Resource* sanDirect3D::getRenderTarget(int id)
{
	return pRenderTarget[id];
}

UINT sanDirect3D::getIndex(void)
{
	return rtvIndex;
}

ID3D12Resource* sanDirect3D::getWhiteTexture()
{
	return pWhiteTex;
}

D3D12_SHADER_RESOURCE_VIEW_DESC* sanDirect3D::getWhiteTextureViewDesc()
{
	return &srvDesc;
}
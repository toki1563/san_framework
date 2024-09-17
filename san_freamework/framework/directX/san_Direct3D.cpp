#include "../../framework.h"
#include "../san_environment.h"

extern HWND hWnd; // ウィンドウハンドル

IDXGIFactory4* sanDirect3D::pFactory = NULL; // DXGIファクトリー
IDXGIAdapter3* sanDirect3D::pAdapter = NULL; // アダプター
ID3D12Device* sanDirect3D::pDevice = NULL; // D3D12デバイス
IDXGISwapChain4* sanDirect3D::pSwapChain = NULL; // スワップチェイン
ID3D12CommandQueue* sanDirect3D::pCmdQueue = NULL; // コマンドキュー
ID3D12CommandAllocator* sanDirect3D::pCmdAllocator = NULL; // コマンドアロケーター
ID3D12GraphicsCommandList* sanDirect3D::pCmdList = NULL; // コマンドリスト

int sanDirect3D::initialize()
{
	createFactory();
	createDevice();


	createCommandList();
}

// DXGIファクトリの作成
void sanDirect3D::createFactory()
{
	HRESULT hr = S_OK;
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

void sanDirect3D::createDevice()
{
	HRESULT hr = S_OK;

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
		OutputDebugString(L"Selected Adapter : ");
		OutputDebugString(adesc.Description);
		OutputDebugString(L"\n");
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

void sanDirect3D::createCommandQueue()
{
	HRESULT hr = S_OK;

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
	command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // コマンドリストと合わせる
	command_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // 指定なし
	command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // タイムアウトなし
	command_queue_desc.NodeMask = 0;

	//============================================================================================================================
	// まだ記入できてない

}


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
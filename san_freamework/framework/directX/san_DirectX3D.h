#pragma once

class sanDirect3D
{

private:
	static ID3D12Device* pDevice; // D3D12デバイス
	static IDXGIFactory6* pAdapter; // アダプター
	static IDXGISwapChain4* pSwapChain; // スワップチェイン

};
#include "../../framework.h"
#include "../san_environment.h"

ID3D12Device* sanDirect3D::pDevice = NULL; // D3D12デバイス
IDXGIFactory6* sanDirect3D::pAdapter = NULL; // アダプター
IDXGISwapChain4* sanDirect3D::pSwapChain = NULL; // スワップチェイン


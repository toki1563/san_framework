//--------------------------------------------------------------//
//	"san_DebugDraw.cpp"											//
//	     デバッグ描画												//
//													2024/11/07	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework//san_environment.h"

// 頂点要素
D3D12_INPUT_ELEMENT_DESC sanDebugDraw::inputElementDescs[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};

int				  sanDebugDraw::VertexNum = 0; 	// 頂点数
sanVertex3D_Line  *sanDebugDraw::vtx = NULL;    // 頂点データ
XMFLOAT4X4        *sanDebugDraw::VP = NULL;     // マトリクス

// 頂点バッファ
ID3D12Resource* sanDebugDraw::vertBuff = NULL;
D3D12_VERTEX_BUFFER_VIEW sanDebugDraw::vertexBufferView;

// 定数バッファ
ID3D12Resource* sanDebugDraw::constBuff = NULL;
ID3D12DescriptorHeap* sanDebugDraw::basicDescHeap = NULL;

ID3D12PipelineState* sanDebugDraw::pPipelineState; // パイプラインステート

bool sanDebugDraw::initialize()
{
	VertexNum = 0; // 頂点数初期化

	HRESULT hr = S_OK;

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) }; // 頂点レイアウトの指定
	psoDesc.pRootSignature = sanDirect3D::getRootSignature();				  // ルートシグネチャの取得

	// 頂点シェーダーとピクセルシェーダーの設定
	psoDesc.VS.pShaderBytecode = sanShader::getVShader(sanShader::eVertexShader::VS_3D_Line)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getVShader(sanShader::eVertexShader::VS_3D_Line)->getLength();
	psoDesc.PS.pShaderBytecode = sanShader::getPShader(sanShader::ePixelShader::PS_3D_Line)->getCode();
	psoDesc.PS.BytecodeLength = sanShader::getPShader(sanShader::ePixelShader::PS_3D_Line)->getLength();

	// ラスタライザーステートの設定
	psoDesc.RasterizerState.MultisampleEnable = false;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.RasterizerState.FrontCounterClockwise = false;
	psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	psoDesc.RasterizerState.AntialiasedLineEnable = false;
	psoDesc.RasterizerState.ForcedSampleCount = 0;
	psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// ブレンドステートの設定
	psoDesc.BlendState.AlphaToCoverageEnable = false;
	psoDesc.BlendState.IndependentBlendEnable = false;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_DEST_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
	psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.StencilEnable = false;
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	// パイプラインステートオブジェクト(PSO)の作成
	hr = sanDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
	pPipelineState->SetName(L"sanDebugDraw::pPipelineState");

	// 頂点バッファの作成
	const UINT vertexBufferSize = sizeof(sanVertex3D_Line) * sanDebugDraw_VertexMax;

	// ヒーププロパティの設定
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	// リソースディスクリプタの設定
	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = vertexBufferSize;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファリソースの作成
	hr = sanDirect3D::getDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(&vertBuff));
	if (hr != S_OK)
	{
		assert(hr == S_OK);
	}
	vertBuff->SetName(L"sanDebug::vertBuff");

	// 頂点バッファのマッピング
	hr = vertBuff->Map(0, NULL, reinterpret_cast<void**>(&vtx));

	vertexBufferView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(sanVertex3D_Line);
	vertexBufferView.SizeInBytes = vertexBufferSize;

	// 定数バッファの作成
	D3D12_HEAP_PROPERTIES constHeapProp = {};
	constHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	constHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	constHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	constHeapProp.CreationNodeMask = 1;
	constHeapProp.VisibleNodeMask = 1;
	D3D12_RESOURCE_DESC constDesc = {};
	constDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	constDesc.Width = ((sizeof(XMFLOAT4X4) + 0xff) & ~0xff);
	constDesc.Height = 1;
	constDesc.DepthOrArraySize = 1;
	constDesc.MipLevels = 1;
	constDesc.Format = DXGI_FORMAT_UNKNOWN;
	constDesc.SampleDesc.Count = 1;
	constDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	constDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファリソースの作成
	hr = sanDirect3D::getDevice()->CreateCommittedResource(
		&constHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&constDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	if (hr != S_OK)
	{
		assert(hr == S_OK);
	}
	constBuff->SetName(L"sanDebugDraw::constBuff");

	// 定数バッファのマッピング
	hr = constBuff->Map(0, NULL, (void**)&VP);

	// ディスクリプタヒープの設定と作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	hr = sanDirect3D::getDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
	basicDescHeap->SetName(L"sanDebugDraw::basicDescHeap");

	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
	// SRVの作成
	sanDirect3D::getDevice()->CreateShaderResourceView(sanDirect3D::getWhiteTexture(),
		sanDirect3D::getWhiteTextureViewDesc(),
		basicHeapHandle);
	basicHeapHandle.ptr += sanDirect3D::getDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;

	// 定数バッファビュー(CBV)の作成
	sanDirect3D::getDevice()->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

	return true;
}

void sanDebugDraw::terminate()
{
	SAFE_RELEASE(basicDescHeap);
	SAFE_RELEASE(constBuff);
	SAFE_RELEASE(vertBuff);
	SAFE_RELEASE(pPipelineState);
}

void sanDebugDraw::render()
{
	if (VertexNum == 0) return;

	// コンスタントバッファ
	XMStoreFloat4x4(VP, XMMatrixTranspose(*sanCamera::getScreen()));

	sanDirect3D::getCommandList()->SetPipelineState(pPipelineState);

	sanDirect3D::getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	sanDirect3D::getCommandList()->SetDescriptorHeaps(1, &basicDescHeap);
	sanDirect3D::getCommandList()->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());
	sanDirect3D::getCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	sanDirect3D::getCommandList()->DrawInstanced(VertexNum, 1, 0, 0);

	VertexNum = 0;
}

void sanDebugDraw::Line(const XMVECTOR* start, const XMVECTOR* end, DWORD color)
{
	if (VertexNum >= sanDebugDraw_VertexMax - 2) return;

	vtx[VertexNum].x = XMVectorGetX(*start);	vtx[VertexNum].y = XMVectorGetY(*start);	vtx[VertexNum].z = XMVectorGetZ(*start);	vtx[VertexNum].color = color; VertexNum++;
	vtx[VertexNum].x = XMVectorGetX(*end);		vtx[VertexNum].y = XMVectorGetY(*end);		vtx[VertexNum].z = XMVectorGetZ(*end);		vtx[VertexNum].color = color; VertexNum++;
}

void sanDebugDraw::Line(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z, DWORD color)
{
	if (VertexNum >= sanDebugDraw_VertexMax - 2) return;

	vtx[VertexNum].x = start_x;		vtx[VertexNum].y = start_y;		vtx[VertexNum].z = start_z;		vtx[VertexNum].color = color;	VertexNum++;
	vtx[VertexNum].x = end_x;		vtx[VertexNum].y = end_y;		vtx[VertexNum].z = end_z;		vtx[VertexNum].color = color;	VertexNum++;
}

void sanDebugDraw::Grid(int num, float interval, DWORD color, bool gridActive)
{
	if (gridActive)
	{
		for (interval = -num; interval <= num; interval++)
		{
			// X軸
			vtx[VertexNum].x = -5.0f;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = interval;	vtx[VertexNum].color = 0xffffffff;	VertexNum++;
			vtx[VertexNum].x = 5.0f;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = interval;	vtx[VertexNum].color = 0xffffffff;	VertexNum++;

			// Y軸
			vtx[VertexNum].x = interval;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = -5.0f;	vtx[VertexNum].color = 0xffffffff;	VertexNum++;
			vtx[VertexNum].x = interval;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = 5.0f;	vtx[VertexNum].color = 0xffffffff;	VertexNum++;
		}
	}
}

void sanDebugDraw::Axis(float length, bool axisActive)
{
	if (axisActive)
	{
		//X軸
		vtx[VertexNum].x = 0.0f;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = 0.0f;	vtx[VertexNum].color = 0xff0000ff;	VertexNum++;
		vtx[VertexNum].x = length;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = 0.0f;	vtx[VertexNum].color = 0xff0000ff;	VertexNum++;
		//Y軸
		vtx[VertexNum].x = 0.0f;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = 0.0f;	vtx[VertexNum].color = 0xff00ff00;	VertexNum++;
		vtx[VertexNum].x = 0.0f;	vtx[VertexNum].y = length;	vtx[VertexNum].z = 0.0f;	vtx[VertexNum].color = 0xff00ff00;	VertexNum++;
		//Z軸
		vtx[VertexNum].x = 0.0f;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = 0.0f;	vtx[VertexNum].color = 0xffff0000;	VertexNum++;
		vtx[VertexNum].x = 0.0f;	vtx[VertexNum].y = 0.0f;	vtx[VertexNum].z = length;	vtx[VertexNum].color = 0xffff0000;	VertexNum++;
	}
}
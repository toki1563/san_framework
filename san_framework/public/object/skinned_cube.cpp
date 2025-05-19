#include "../../framework.h"
#include "../../framework/san_environment.h"

#include "skinned_cube.h"

//頂点要素
D3D12_INPUT_ELEMENT_DESC cSkinnedCube::inputElementDescs[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

//静的共通データ
int					cSkinnedCube::initCommon = 0;
ID3D12PipelineState* cSkinnedCube::pPipelineState = NULL;	//パイプラインステート

//静的共通データ初期化
bool cSkinnedCube::initializeCommon()
{
	if (initCommon++ > 0)return true;

	HRESULT hr = S_OK;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = sanDirect3D::getRootSignature();

	psoDesc.VS.pShaderBytecode = sanShader::getVShader(sanShader::eVertexShader::VS_3D_Cube)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getVShader(sanShader::eVertexShader::VS_3D_Cube)->getLength();
	psoDesc.PS.pShaderBytecode = sanShader::getPShader(sanShader::ePixelShader::PS_3D_Cube)->getCode();
	psoDesc.PS.BytecodeLength = sanShader::getPShader(sanShader::ePixelShader::PS_3D_Cube)->getLength();

	psoDesc.RasterizerState.MultisampleEnable = false;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.RasterizerState.FrontCounterClockwise = false;
	psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	psoDesc.RasterizerState.AntialiasedLineEnable = false;
	psoDesc.RasterizerState.ForcedSampleCount = 0;
	psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	psoDesc.BlendState.AlphaToCoverageEnable = false;
	psoDesc.BlendState.IndependentBlendEnable = false;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = false;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_DEST_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
	psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
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
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	hr = sanDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
	pPipelineState->SetName(L"cSkinnedCube::pPipelineState");

	return true;
}

//静的共通データ削除
void cSkinnedCube::terminateCommon()
{
	if (--initCommon > 0)return;

	SAFE_RELEASE(pPipelineState);
}


cSkinnedCube::cSkinnedCube()
{
	initializeCommon();

	//辺の長さ
	float size = 1.0f;

	//面の数
	int FaceNum = 8;
	//頂点の数
	VertexNum = FaceNum * 2 * 3;

	//オブジェクト自身の位置
	Position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//オブジェクト自身の回転
	Rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//オブジェクト自身の拡大
	Scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	//姿勢変形用マトリクスの初期化
	trans = XMMatrixIdentity();
	rotate = XMMatrixIdentity();
	scale = XMMatrixIdentity();
	world = XMMatrixIdentity();

	HRESULT hr;
	//テクスチャの読み込み
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};
	hr = LoadFromWICFile(L"data/image/checker.png", WIC_FLAGS_NONE, &metadata, scratchImg);
	if (hr == S_OK)
	{
		const Image* img = scratchImg.GetImage(0, 0, 0);

		D3D12_HEAP_PROPERTIES texHeapProp = {};
		texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		texHeapProp.CreationNodeMask = 0;
		texHeapProp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Format = metadata.format;
		resDesc.Width = metadata.width;
		resDesc.Height = (UINT)metadata.height;
		resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.MipLevels = (UINT16)metadata.mipLevels;
		resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		hr = sanDirect3D::getDevice()->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			NULL,
			IID_PPV_ARGS(&texbuff)
		);
		texbuff->SetName(L"cSkinnedCube::texbuff");

		hr = texbuff->WriteToSubresource(0,
			NULL,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 2;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		hr = sanDirect3D::getDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

		//定数バッファ
		D3D12_HEAP_PROPERTIES constHeapProp = {};
		constHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		constHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		constHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		constHeapProp.CreationNodeMask = 1;
		constHeapProp.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC constDesc = {};
		constDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		constDesc.Width = ((sizeof(stConstantBuffer) + 0xff) & ~0xff);
		constDesc.Height = 1;
		constDesc.DepthOrArraySize = 1;
		constDesc.MipLevels = 1;
		constDesc.Format = DXGI_FORMAT_UNKNOWN;
		constDesc.SampleDesc.Count = 1;
		constDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		constDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
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
		constBuff->SetName(L"cSkinnedCube::constBuff");

		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
		//シェーダリソースビューの作成
		if (texbuff != NULL)
		{
			//テクスチャビュー作成
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = metadata.format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;

			sanDirect3D::getDevice()->CreateShaderResourceView(texbuff,
				&srvDesc,
				basicHeapHandle
			);
		}
		else
		{
			sanDirect3D::getDevice()->CreateShaderResourceView(sanDirect3D::getWhiteTexture(),
				sanDirect3D::getWhiteTextureViewDesc(),
				basicHeapHandle
			);
		}
		basicHeapHandle.ptr += sanDirect3D::getDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
		//定数バッファビューの作成
		sanDirect3D::getDevice()->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

		hr = constBuff->Map(0, NULL, (void**)&pConstBuffer);
	}

	//頂点バッファ
	const UINT vertexBufferSize = sizeof(stCubeVertex3D) * VertexNum;

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapprop.CreationNodeMask = 1;
	heapprop.VisibleNodeMask = 1;

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

	hr = sanDirect3D::getDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(&vertBuff)
	);
	if (hr != S_OK)
	{
		assert(hr == S_OK);
	}
	vertBuff->SetName(L"cSkinnedCube::vertBuff");

	hr = vertBuff->Map(0, NULL, reinterpret_cast<void**>(&vtx));

	vertexBufferView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(stCubeVertex3D);
	vertexBufferView.SizeInBytes = vertexBufferSize;

	//頂点メモリの初期化
	memset(vtx, 0, vertexBufferSize);

	//頂点メモリの確保(スキン用)
	vtxS = new stSkinVertex3D[VertexNum];

	//頂点要素の初期化
	int v = 0;
	//-Z面(root)
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = 0.0f;			vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	vtxS[v].x = -size;	vtxS[v].y = 0.0f;			vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = 0.0f;			vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	//+X面(root)
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = 0.0f;			vtxS[v].z = -size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	vtxS[v].x = size;	vtxS[v].y = 0.0f;			vtxS[v].z = -size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = 0.0f;			vtxS[v].z = size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	//+Z面(root)
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = 0.0f;			vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	vtxS[v].x = size;	vtxS[v].y = 0.0f;			vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = 0.0f;			vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	//-X面(root)
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = 0.0f;			vtxS[v].z = size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;

	vtxS[v].x = -size;	vtxS[v].y = 0.0f;			vtxS[v].z = size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = 0.0f;			vtxS[v].z = -size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 1.0f;	 vtxS[v].boneWeight[1] = 0.0f;	 v++;
	
	//-Z面(top)
	vtxS[v].x = -size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = -1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	//+X面(top)
	vtxS[v].x = size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = -size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	//+Z面(top)
	vtxS[v].x = size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	vtxS[v].x = size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = 0.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 1.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	//-X面(top)
	vtxS[v].x = -size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = -size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 0.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 4.0f;	vtxS[v].z = -size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 0.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.0f;	 vtxS[v].boneWeight[1] = 1.0f;	 v++;
	vtxS[v].x = -size;	vtxS[v].y = size * 2.0f;	vtxS[v].z = -size;	vtxS[v].nx = -1.0f;	vtxS[v].ny = 0.0f;	vtxS[v].nz = 0.0f;	vtxS[v].u = 1.0f;	vtxS[v].v = 1.0f;	vtxS[v].boneID[0] = 0;	vtxS[v].boneID[1] = 1;	vtxS[v].boneWeight[0] = 0.5f;	 vtxS[v].boneWeight[1] = 0.5f;	 v++;

	//ボーン情報の初期化
	BoneNum = _countof(bones);
	// size(bone) / sizeof(bone[0])

	bones[0].pParent = NULL;
	bones[0].BindPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	bones[0].BindRot = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	bones[0].BindScl = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	bones[1].pParent = &bones[0];
	bones[1].BindPos = XMVectorSet(0.0f, size * 2.0f, 0.0f, 0.0f);
	bones[1].BindRot = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	bones[1].BindScl = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	//ボーンマトリクス設定
	for (int i = 0; i < BoneNum; i++)
	{
		// バインドポーズに設定
		bones[i].Pos = bones[i].BindPos;
		bones[i].Rot = bones[i].BindRot;
		bones[i].Scl = bones[i].BindScl;

		// ローカルマトリクス
		//XMMATRIX trans = XMMatrixTranslation(XMVectorGetX(bones[i].BindPos), XMVectorGetY(bones[i].BindPos), XMVectorGetZ(bones[i].BindPos));
		//XMMATRIX rotate = XMMatrixRotationRollPitchYaw(XMVectorGetX(bones[i].BindRot), XMVectorGetY(bones[i].BindRot, XMVectorGetZ(bones[i].BindRot);
		//XMMATRIX scale = XMMatrixScaling(XMVectorGetX(bones[i].BindScl), XMVectorGetY(bones[i].BindScl), XMVectorGetZ(bones[i].BindScl);
		XMMATRIX trans = XMMatrixTranslationFromVector(bones[i].BindPos);
		XMMATRIX rotate = XMMatrixRotationRollPitchYawFromVector(bones[i].BindRot);
		XMMATRIX scale = XMMatrixScalingFromVector(bones[i].BindScl);

		// SIMD

		bones[i].Local = scale * rotate * trans; // ここの順番大事

		if (bones[i].pParent == NULL)
		{
			bones[i].World = bones[i].Local;
		}
		else
		{
			bones[i].World = bones[i].Local * (bones[i].pParent->World);
		}

		// バインド逆行列
		bones[i].iBind = XMMatrixInverse(NULL, bones[i].World);

		// スキニング変換マトリクス
		bones[i].SkinMtx = bones[i].World * bones[i].iBind; // 今の姿勢と初期姿勢の逆行列を掛ける
	}
}

cSkinnedCube::~cSkinnedCube()
{
	SAFE_RELEASE(basicDescHeap);
	SAFE_RELEASE(constBuff);
	SAFE_RELEASE(texbuff);
	SAFE_RELEASE(vertBuff);
	texbuff = NULL;

	delete[] vtxS;

	terminateCommon();
}

void cSkinnedCube::execute()
{
}

void cSkinnedCube::render()
{
	// ボーンの情報の更新
	// ボーンマトリクス設定
	for (int i = 0; i < BoneNum; i++)
	{
		// ローカルマトリクス
		XMMATRIX trans = XMMatrixTranslationFromVector(bones[i].Pos);
		XMMATRIX rotate = XMMatrixRotationRollPitchYawFromVector(bones[i].Rot);
		XMMATRIX scale = XMMatrixScalingFromVector(bones[i].Scl);

		bones[i].Local = scale * rotate * trans; // ここの順番大事

		if (bones[i].pParent == NULL)
		{
			bones[i].World = bones[i].Local;
		}
		else
		{
			bones[i].World = bones[i].Local * (bones[i].pParent->World);
		}

		// スキニング変換マトリクス
		bones[i].SkinMtx = bones[i].World * bones[i].iBind;
	}

	//スキニング頂点計算
	for (int i = 0; i < VertexNum; i++)
	{
		// SkinMtxに変形後の情報を入れる
		// 変形前の座標
		XMVECTOR pos = XMVectorSet(vtxS[i].x, vtxS[i].y, vtxS[i].z, 1.0f);

		XMVECTOR result = XMVectorZero();

		for (int j = 0; j < 2; j++)
		{
			int boneIndex = vtxS[i].boneID[j];
			float weight = vtxS[i].boneWeight[j];

			if (weight > 0.0f)
			{
				XMVECTOR transformed = XMVector3TransformCoord(pos, bones[boneIndex].SkinMtx);
				result += transformed * weight;
			}
		}

		vtx[i].x = XMVectorGetX(result);
		vtx[i].y = XMVectorGetY(result);
		vtx[i].z = XMVectorGetZ(result);

		vtx[i].nx = vtxS[i].nx;
		vtx[i].ny = vtxS[i].ny;
		vtx[i].nz = vtxS[i].nz;

		vtx[i].u = vtxS[i].u;
		vtx[i].v = vtxS[i].v;
	}

	XMMATRIX World = XMMatrixIdentity();
	XMMATRIX WVP = *sanCamera::getScreen();

	//コンスタントバッファに情報を設定
	XMStoreFloat4x4(&pConstBuffer->WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&pConstBuffer->World, XMMatrixTranspose(World));
	XMStoreFloat4(&pConstBuffer->LightDir, *sanLight::getILightDir());
	XMStoreFloat4(&pConstBuffer->LightCol, *sanLight::getLightColor());
	XMStoreFloat4(&pConstBuffer->LightAmb, *sanLight::getAmbient());

	sanDirect3D::getCommandList()->SetPipelineState(pPipelineState);

	sanDirect3D::getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	sanDirect3D::getCommandList()->SetDescriptorHeaps(1, &basicDescHeap);

	D3D12_GPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	sanDirect3D::getCommandList()->SetGraphicsRootDescriptorTable(0, basicHeapHandle);

	sanDirect3D::getCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	sanDirect3D::getCommandList()->DrawInstanced(VertexNum, 1, 0, 0);
}

//移動値の設定
void cSkinnedCube::setPositionX(float value)
{
	Position = XMVectorSetX(Position, value);
}

void cSkinnedCube::setPositionY(float value)
{
	Position = XMVectorSetY(Position, value);
}

void cSkinnedCube::setPositionZ(float value)
{
	Position = XMVectorSetZ(Position, value);
}

void cSkinnedCube::addPositionX(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void cSkinnedCube::addPositionY(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void cSkinnedCube::addPositionZ(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

//回転値の設定
void cSkinnedCube::setRotationX(float radian)
{
	Rotation = XMVectorSetX(Rotation, radian);
}

void cSkinnedCube::setRotationY(float radian)
{
	Rotation = XMVectorSetY(Rotation, radian);
}

void cSkinnedCube::setRotationZ(float radian)
{
	Rotation = XMVectorSetZ(Rotation, radian);
}

void cSkinnedCube::addRotationX(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(radian, 0.0f, 0.0f, 0.0f));
}

void cSkinnedCube::addRotationY(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(0.0f, radian, 0.0f, 0.0f));
}

void cSkinnedCube::addRotationZ(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(0.0f, 0.0f, radian, 0.0f));
}

//拡大値の設定
void cSkinnedCube::setScaleX(float value)
{
	Scale = XMVectorSetX(Scale, value);
}

void cSkinnedCube::setScaleY(float value)
{
	Scale = XMVectorSetY(Scale, value);
}

void cSkinnedCube::setScaleZ(float value)
{
	Scale = XMVectorSetZ(Scale, value);
}

void cSkinnedCube::addScaleX(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void cSkinnedCube::addScaleY(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void cSkinnedCube::addScaleZ(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

//移動値の取得
float cSkinnedCube::getPositionX(void)
{
	return XMVectorGetX(Position);
}

float cSkinnedCube::getPositionY(void)
{
	return XMVectorGetY(Position);
}

float cSkinnedCube::getPositionZ(void)
{
	return XMVectorGetZ(Position);
}

//回転値の取得
float cSkinnedCube::getRotationX(void)
{
	return XMVectorGetX(Rotation);
}

float cSkinnedCube::getRotationY(void)
{
	return XMVectorGetY(Rotation);
}

float cSkinnedCube::getRotationZ(void)
{
	return XMVectorGetZ(Rotation);
}

//拡大値の取得
float cSkinnedCube::getScaleX(void)
{
	return XMVectorGetX(Scale);
}

float cSkinnedCube::getScaleY(void)
{
	return XMVectorGetY(Scale);
}

float cSkinnedCube::getScaleZ(void)
{
	return XMVectorGetZ(Scale);
}

//ワールド行列の取得
XMMATRIX * cSkinnedCube::getWorld()
{
	return &world;
}

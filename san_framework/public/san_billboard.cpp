﻿//--------------------------------------------------------------//
//	"san_billboard.cpp"											//
//	     ビルボードポリゴン										//
//													2024/11/18	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/san_environment.h"

sanBillboard::sanBillboard(float width, float height, const WCHAR* texture_file, float left_u, float right_u, float top_v, float bottom_v) : sanObject()
{
	HRESULT hr = S_OK;

	float w = width * 0.5f;
	float h = height * 0.5f;
	BasePos[0] = XMVectorSet(-w, h, 0.0f, 0.0f);
	BasePos[1] = XMVectorSet(w, h, 0.0f, 0.0f);
	BasePos[2] = XMVectorSet(-w, -h, 0.0f, 0.0f);
	BasePos[3] = XMVectorSet(w, -h, 0.0f, 0.0f);

	TexMetadata metadata = {};
	ScratchImage scratchImg = {};
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;

	texBuff = NULL;
	basicDescHeap = NULL;

	// テクスチャの拡張子
	const WCHAR* ext = wcsrchr(texture_file, L'.');

	// テクスチャの読み込み
	if (ext != NULL)
	{
		if (wcscmp(ext, L".tag") == 0 || wcscmp(ext, L".TGA") == 0)
		{
			hr = LoadFromTGAFile(texture_file, TGA_FLAGS_NONE, &metadata, scratchImg);
		}
		else
		{
			hr = LoadFromWICFile(texture_file, WIC_FLAGS_NONE, &metadata, scratchImg);
		}
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
				nullptr,
				IID_PPV_ARGS(&texBuff)
			);
			texBuff->SetName(L"sanBillboard::texbuff");

			hr = texBuff->WriteToSubresource(0,
				NULL,
				img->pixels,
				(UINT)img->rowPitch,
				(UINT)img->slicePitch
			);
		}
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 2;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		hr = sanDirect3D::getDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

		// 定数バッファ
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
			assert(hr = S_OK);
		}
		constBuff->SetName(L"sanBillboard::constBuff");

		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
		// シェーダーリソースビューの作成
		if (texBuff != NULL)
		{
			// テクスチャビュー作成
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = format; // metadata.format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;

			sanDirect3D::getDevice()->CreateShaderResourceView(texBuff,
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
		// 定数バッファビューの作成
		sanDirect3D::getDevice()->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

		hr = constBuff->Map(0, NULL, (void**)&pConstBuffer);
	}

	// 頂点バッファ
	const int vnum = 4;

	const UINT vertexBufferSize = sizeof(sanVertex3D) * vnum;

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

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
	vertBuff->SetName(L"sanBillboard::vertBuff");

	hr = vertBuff->Map(0, NULL, reinterpret_cast<void**>(&vtx));

	vertexBufferView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(sanVertex3D);
	vertexBufferView.SizeInBytes = vertexBufferSize;

	// 頂点メモリの初期化
	memset(vtx, 0, sizeof(sanVertex3D) * vnum);

	// 頂点座標を設定
	setVertexPosition();

	// 頂点カラーを設定
	vtx[0].r = 1.0f;	vtx[0].g = 1.0f;	vtx[0].b = 1.0f;	vtx[0].a = 1.0f;
	vtx[1].r = 1.0f;	vtx[1].g = 1.0f;	vtx[1].b = 1.0f;	vtx[1].a = 1.0f;
	vtx[2].r = 1.0f;	vtx[2].g = 1.0f;	vtx[2].b = 1.0f;	vtx[2].a = 1.0f;
	vtx[3].r = 1.0f;	vtx[3].g = 1.0f;	vtx[3].b = 1.0f;	vtx[3].a = 1.0f;

	// UVを設定
	vtx[0].u = left_u;	vtx[0].v = top_v;
	vtx[1].u = right_u;	vtx[1].v = top_v;
	vtx[2].u = left_u;	vtx[2].v = bottom_v;
	vtx[3].u = right_u;	vtx[3].v = bottom_v;

	// マテリアル関連
	Diffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	Ambient = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Specular = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	setTransparent(true);
	setLighting(false);
}

sanBillboard::~sanBillboard()
{
	SAFE_RELEASE(basicDescHeap);
	SAFE_RELEASE(texBuff);
	SAFE_RELEASE(constBuff);
	SAFE_RELEASE(vertBuff);
}

void sanBillboard::execute()
{
}

void sanBillboard::render()
{
	// ワールドマトリクスの計算
	calculateLocalMatrix();
	calculateWorldMatrix();

	XMMATRIX W = XMMatrixIdentity();
	XMMATRIX WVP = *sanCamera::getScreen();

	// 頂点座標の設定
	setVertexPosition();

#if 1
	// コンスタントバッファ情報を設定
	XMStoreFloat4x4(&pConstBuffer->WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&pConstBuffer->World, XMMatrixTranspose(W));
	XMStoreFloat4(&pConstBuffer->LightDir, *sanLight::getILightDir());
	XMStoreFloat4(&pConstBuffer->LightCol, *sanLight::getLightColor());
	XMStoreFloat4(&pConstBuffer->LightAmb, *sanLight::getAmbient());
	XMStoreFloat4(&pConstBuffer->CameraPos, *sanCamera::getPosition());
	XMStoreFloat4(&pConstBuffer->Diffuse, Diffuse);
	XMStoreFloat4(&pConstBuffer->Ambient, Ambient);
	XMStoreFloat4(&pConstBuffer->Specular, Specular);

	if (lighting)
	{
		// ライティング有効
		if (transparent)
		{
			// 半透明有効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha);
		}
		else
		{
			// 半透明無効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState);
		}
	}
	else
	{
		// ライティング無効
		if (transparent)
		{
			// 半透明有効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha_NL);
		}
		else
		{
			// 半透明無効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_NL);
		}
	}

	sanDirect3D::getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	sanDirect3D::getCommandList()->SetDescriptorHeaps(1, &basicDescHeap);
	sanDirect3D::getCommandList()->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());
	sanDirect3D::getCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	
	sanDirect3D::getCommandList()->DrawInstanced(4, 1, 0, 0);
#endif
}

// 頂点座標の設定
void sanBillboard::setVertexPosition()
{
	XMMATRIX mBillboard = *sanCamera::getView();
	// 移動成分をなくす(※w成分は1)
	mBillboard.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	// 逆行列を計算(※アフィン変換のみのため、転換行列と同じ)
	mBillboard = XMMatrixTranspose(mBillboard);

	XMVECTOR w;
	getWorldPosition(&w);

	XMVECTOR v[4];
	for (int i = 0; i < 4; i++)
	{
		// 基本座標(BasePos)にビルボードマトリクスを乗算
		v[i] = XMVector3TransformNormal(BasePos[i], mBillboard);
		// 回転ベクトルに座標を加算する
		v[i] = XMVectorAdd(v[i], w);
		// レンダリング用頂点に代入
		vtx[i].x = XMVectorGetX(v[i]);
		vtx[i].y = XMVectorGetY(v[i]);
		vtx[i].z = XMVectorGetZ(v[i]);
	}
	//vnDebugDraw::Line(&v[0], &v[1]);
	//vnDebugDraw::Line(&v[1], &v[3]);
	//vnDebugDraw::Line(&v[3], &v[2]);
	//vnDebugDraw::Line(&v[2], &v[0]);
}

void sanBillboard::setDiffuse(float r, float g, float b, float a)
{
	Diffuse = XMVectorSet(r, g, b, a);
}
//--------------------------------------------------------------//
//	"san_effect.cpp"				     					    //
//	     エフェクト(パーティクル)クラス							//
//													2024/12/12	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/san_environment.h"

#define sanPARTICLE_MAX (1024)

sanEffect::sanEffect()
{
	// パーティクル配列のメモリ確保
	pParticle = new sanParticle[sanPARTICLE_MAX];

	for (int i = 0; i < sanPARTICLE_MAX; i++)
	{
		pParticle[i].Life = 0.0f;
		pParticle[i].Pos = XMVectorZero();
		pParticle[i].Vel = XMVectorZero();
	}

	// 描画情報の初期化
	HRESULT hr = S_OK;

	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	texbuff = NULL;
	basicDescHeap = NULL;

	// テクスチャの拡張子
	WCHAR texture_file[] = L"";//L"data/image/particle/particle001.png";
	const WCHAR* ext = wcsrchr(texture_file, L'.');

	// テスクチャの読み込み
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
				IID_PPV_ARGS(&texbuff)
			);
			texbuff->SetName(L"sanEffect::texbuff");

			hr = texbuff->WriteToSubresource(0,
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
		constBuff->SetName(L"sanEffect::constBuff");

		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

		// シェーダーリソースビュー作成
		if (texbuff != NULL)
		{
			// テクスチャビュー作成
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

	// 頂点バッファ
	const int vunm = sanPARTICLE_MAX * 4;

	const UINT vertexBufferSize = sizeof(sanVertex3D) * vunm;

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
	vertBuff->SetName(L"sanEffect::vertBuff");

	hr = vertBuff->Map(0, NULL, reinterpret_cast<void**>(&vtx));

	vertexBufferView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(sanVertex3D);
	vertexBufferView.SizeInBytes = vertexBufferSize;

	// インデックスバッファ
	const int inum = sanPARTICLE_MAX * 6;
	{
		const UINT indexBufferSize = sizeof(WORD) * inum;

		D3D12_HEAP_PROPERTIES heapprop = {};
		heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resdesc = {};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = indexBufferSize;
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
			nullptr,
			IID_PPV_ARGS(&pIndexBuffer)
		);
		pIndexBuffer->SetName(L"sanEffect::pIndexBuffer");
		pIndexBuffer->Map(0, nullptr, (void**)&idx);

		// インデックスバッファビューを作成
		indexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		indexBufferView.SizeInBytes = indexBufferSize;

		// インデックスデータの初期化
		memset(idx, 0, sizeof(WORD) * inum);
	}

	// 頂点データの初期化
	for (int i = 0; i < sanPARTICLE_MAX; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			vtx[i * 4 + j].x = 0.0f;
			vtx[i * 4 + j].y = 0.0f;
			vtx[i * 4 + j].z = 0.0f;
			vtx[i * 4 + j].nx = 0.0f;
			vtx[i * 4 + j].ny = 0.0f;
			vtx[i * 4 + j].nz = 0.0f;
			vtx[i * 4 + j].r = 1.0f;
			vtx[i * 4 + j].g = 1.0f;
			vtx[i * 4 + j].b = 1.0f;
			vtx[i * 4 + j].a = 1.0f;
		}
		vtx[i * 4 + 0].u = 0.0f;	vtx[i * 4 + 0].v = 0.0f;
		vtx[i * 4 + 1].u = 1.0f;	vtx[i * 4 + 1].v = 0.0f;
		vtx[i * 4 + 2].u = 0.0f;	vtx[i * 4 + 2].v = 1.0f;
		vtx[i * 4 + 3].u = 1.0f;	vtx[i * 4 + 3].v = 1.0f;
	}

	IndexNum = 0;

	renderParticleNum = 0;

	//マテリアル関連
	Diffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	Ambient = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Specular = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	setTransparent(true);

	setLighting(false);
}

sanEffect::~sanEffect()
{
	delete[] pParticle;

	SAFE_RELEASE(basicDescHeap);
	SAFE_RELEASE(texbuff);
	SAFE_RELEASE(constBuff);
	SAFE_RELEASE(vertBuff);
	SAFE_RELEASE(pIndexBuffer);
}

void sanEffect::execute()
{
	// パーティカルの放出
	for (int i = 0; i < sanPARTICLE_MAX; i++)
	{
		if (pParticle[i].Life > 0.0f)
		{
			continue;
		}

		pParticle[i].Life = 60.0f;
		pParticle[i].Pos = XMVectorZero();
		pParticle[i].Vel = XMVectorSet(
			(float)(rand() % 2000) / 1000.0f - 1.0f,  // -1~1
			(float)(rand() % 1000) / 1000.0f,		  //  0~1
			(float)(rand() % 2000) / 1000.0f - 1.0f,  // -1~1
			0.0f
		);
		pParticle[i].Vel *= 0.08f;
		break;
	}

	// パーティカルの動き
	for (int i = 0; i < sanPARTICLE_MAX; i++)
	{
		if (pParticle[i].Life <= 0.0f)
		{
			continue;
		}

		pParticle[i].Life -= 1.0f;

		pParticle[i].Pos += pParticle[i].Vel;
	}
}

void sanEffect::setVertexPosition()
{
	IndexNum = 0;
	renderParticleNum = 0;

	XMMATRIX mBillboard = *sanCamera::getView();
	// 移動成分をなくす(※w成分は1)
	mBillboard.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	// 逆行列を計算(※アフィン変換のため、転置行列と同じ)
	mBillboard = XMMatrixTranspose(mBillboard);

	XMVECTOR w;
	getWorldPosition(&w);

	// パーティクルのデバッグ描画
	for (int i = 0; i < sanPARTICLE_MAX; i++)
	{
		if (pParticle[i].Life <= 0.0f)
		{
			continue;
		}

		XMVECTOR v[4];
		float size = 0.2f;
		v[0] = XMVectorSet(-size, size, 0.0f, 0.0f);
		v[1] = XMVectorSet(size, size, 0.0f, 0.0f);
		v[2] = XMVectorSet(-size, -size, 0.0f, 0.0f);
		v[3] = XMVectorSet(size, -size, 0.0f, 0.0f);

		for (int j = 0; j < 4; j++)
		{
			// 基本座標(BasePos)にビルボードマトリクスくを乗算
			v[j] = XMVector3TransformNormal(v[j], mBillboard);
			v[j] += pParticle[i].Pos;
			vtx[renderParticleNum * 4 + j].x = XMVectorGetX(v[j]);
			vtx[renderParticleNum * 4 + j].y = XMVectorGetY(v[j]);
			vtx[renderParticleNum * 4 + j].z = XMVectorGetZ(v[j]);
		}

		// インデックスデータ
		idx[IndexNum++] = renderParticleNum * 4;
		idx[IndexNum++] = renderParticleNum * 4 + 1;
		idx[IndexNum++] = renderParticleNum * 4 + 2;

		idx[IndexNum++] = renderParticleNum * 4 + 1;
		idx[IndexNum++] = renderParticleNum * 4 + 3;
		idx[IndexNum++] = renderParticleNum * 4 + 2;

		renderParticleNum++;

		sanDebugDraw::Line(&v[0], &v[1]);
		sanDebugDraw::Line(&v[1], &v[3]);
		sanDebugDraw::Line(&v[3], &v[2]);
		sanDebugDraw::Line(&v[2], &v[0]);

		// 原点からデバッグ用でライン表示
		sanDebugDraw::Line(&Position, &pParticle[i].Pos, 0xFF00FFFF);
	}
}

void sanEffect::render()
{
	setVertexPosition();

	calculateLocalMatrix();
	calculateWorldMatrix();

	//ビューポート変換マトリクスの計算
	XMMATRIX WVP = XMMatrixMultiply(XMMatrixIdentity(), *sanCamera::getScreen());

	//頂点座標の設定
	setVertexPosition();

	//コンスタントバッファに情報を設定
	XMStoreFloat4x4(&pConstBuffer->WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&pConstBuffer->World, XMMatrixTranspose(World));
	XMStoreFloat4(&pConstBuffer->LightDir, *sanLight::getILightDir());
	XMStoreFloat4(&pConstBuffer->LightCol, *sanLight::getLightColor());
	XMStoreFloat4(&pConstBuffer->LightAmb, *sanLight::getAmbient());
	XMStoreFloat4(&pConstBuffer->CameraPos, *sanCamera::getPosition());
	XMStoreFloat4(&pConstBuffer->Diffuse, Diffuse);
	XMStoreFloat4(&pConstBuffer->Ambient, Ambient);
	XMStoreFloat4(&pConstBuffer->Specular, Specular);

	if (lighting)
	{ // ライティング有効
		if (transparent)
		{ // 半透明有効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha);
		}
		else
		{ // 半透明無効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState);
		}
	}
	else
	{ // ライティング無効
		if (transparent)
		{ // 半透明有効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha_NL);
		}
		else
		{ // 半透明無効
			sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_NL);
		}
	}

	sanDirect3D::getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	sanDirect3D::getCommandList()->SetDescriptorHeaps(1, &basicDescHeap);
	sanDirect3D::getCommandList()->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());
	sanDirect3D::getCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	sanDirect3D::getCommandList()->IASetIndexBuffer(&indexBufferView);

	sanDirect3D::getCommandList()->DrawIndexedInstanced(IndexNum, 1, 0, 0, 0);
}
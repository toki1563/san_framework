//--------------------------------------------------------------//
//	"san_model.cpp"												//
//	     モデルデータ描画クラス									//
//													2024/11/16	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/san_environment.h"

extern HWND hWnd;

sanModel::sanModel(const WCHAR *folder, const WCHAR *file) : sanObject()
{
	HRESULT hr;

	pModelData = NULL;
	pMaterials = NULL;
	pVertexBuffer = NULL;
	pIndexBuffer = NULL;

	// vnmファイルのロード
	WCHAR path[256];
	swprintf_s(path, L"%s%s", folder, file);

	FILE *fp = NULL;
	long size = 0;

	if ((_wfopen_s(&fp, path, L"rb")) != 0)
	{
		WCHAR text[256];
		swprintf_s(text, L"cannot open file \"%s\"", path);
		sanFont::output(text);
		MessageBox(hWnd, text, L"sanModel Construct Error", MB_OK);
		assert(false);
		return;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	pModelData = (sanModelData*)new BYTE[size];
	fread(pModelData, size, 1, fp);
	fclose(fp);

	// バージョンチェック
	if (pModelData->Version < sanMODEL_DATA_LATEST_VER)
	{
		WCHAR text[256];
		swprintf_s(text, L"this file version id obsolete : \"%s\"", path);
		sanFont::output(text);
		MessageBox(hWnd, text, L"sanModel Construct Error", MB_OK);
		assert(false);
		return;
	}

	// マテリアルの作成
	sanModel_MaterialData *pMaterialData = 
		reinterpret_cast<sanModel_MaterialData*>(reinterpret_cast<__int64>(pModelData) + pModelData->MaterialAccess);
	pMaterials = new stMaterial[pModelData->MaterialNum];

	for (int i = 0; i < (int)pModelData->MaterialNum; i++)
	{
		TexMetadata metadata = {};
		ScratchImage scratchImg = {};

		pMaterials[i].texbuff = NULL;
		pMaterials[i].basicDescHeap = NULL;
		if (pMaterialData[i].Texture[0])
		{
			WCHAR texname[256];
			size_t ret;
			mbstowcs_s(&ret, texname, 256, pMaterialData[i].Texture, strlen(pMaterialData[i].Texture));
			swprintf_s(path, L"%s%s", folder, texname);

			// テクスチャの拡張子
			const char *ext = strchr(pMaterialData[i].Texture, '.');

			// テクスチャの読み込み
			if (ext != NULL)
			{
				WCHAR texname[256];
				size_t ret;
				mbstowcs_s(&ret, texname, 256, pMaterialData[i].Texture, strlen(pMaterialData[i].Texture));
				swprintf_s(path, L"%s%s", folder, texname);
				if (strcmp(ext, ".tga") == 0 || strcmp(ext, ".TGA") == 0)
				{
					hr = LoadFromTGAFile(path, TGA_FLAGS_NONE, &metadata, scratchImg);
				}
				else
				{
					hr = LoadFromWICFile(path, WIC_FLAGS_NONE, &metadata, scratchImg);
				}
				if (hr == S_OK)
				{
					const Image* img = scratchImg.GetImage(0, 0, 0);

					// リソースのメモリ領域の配置指定
					D3D12_HEAP_PROPERTIES texHeapProp = {};
					texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
					texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
					texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
					texHeapProp.CreationNodeMask = 0;
					texHeapProp.VisibleNodeMask = 0;

					// リソースの設定
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
						& resDesc,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
						nullptr,
						IID_PPV_ARGS(&pMaterials[i].texbuff)
					);
					pMaterials[i].texbuff->SetName(L"sanModel::texbuff");

					hr = pMaterials[i].texbuff->WriteToSubresource(0,
						NULL,
						img->pixels,
						(UINT)img->rowPitch,
						(UINT)img->slicePitch
					);
				}
			}
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
			descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			descHeapDesc.NodeMask = 0;
			descHeapDesc.NumDescriptors = 2;
			descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			hr = sanDirect3D::getDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&pMaterials[i].basicDescHeap));
			if (hr != S_OK)
			{
				assert(hr == S_OK);
			}
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
				IID_PPV_ARGS(&pMaterials[i].constbuff)
			);
			if (hr != S_OK)
			{
				assert(hr == S_OK);
			}
			pMaterials[i].constbuff->SetName(L"sanModel::constBuff");

			D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = pMaterials[i].basicDescHeap->GetCPUDescriptorHandleForHeapStart();
			// シェーダリソースの作成
			if (pMaterials[i].texbuff != NULL)
			{
				// テクスチャビューの作成
				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.Format = metadata.format;
				srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;

				sanDirect3D::getDevice()->CreateShaderResourceView(pMaterials[i].texbuff,
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
			cbvDesc.BufferLocation = pMaterials[i].constbuff->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = (UINT)pMaterials[i].constbuff->GetDesc().Width;
			// 定数バッファビューの作成
			sanDirect3D::getDevice()->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

			hr = pMaterials[i].constbuff->Map(0, NULL, (void**)&pMaterials[i].pConstBuffer);
		}
		// マテリアルカラー
		pMaterials[i].Diffuse = XMVectorSet(pMaterialData[i].Diffuse[0], pMaterialData[i].Diffuse[1], pMaterialData[i].Diffuse[2], pMaterialData[i].Diffuse[3]);
		pMaterials[i].Ambient = XMVectorSet(pMaterialData[i].Ambient[0], pMaterialData[i].Ambient[1], pMaterialData[i].Ambient[2], pMaterialData[i].Ambient[3]);
		pMaterials[i].Specular = XMVectorSet(pMaterialData[i].Specular[0], pMaterialData[i].Specular[1], pMaterialData[i].Specular[2], pMaterialData[i].Specular[3]);
	}

	{
		// 頂点バッファ
		const UINT vertexBufferSize = sizeof(sanVertex3D) * pModelData->VertexNum;

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
			IID_PPV_ARGS(&pVertexBuffer)
		);
		if (hr != S_OK)
		{
			assert(hr == S_OK);
		}
		pVertexBuffer->SetName(L"sanModel::pVertexBuff");

		// 頂点データのコピー
		sanVertex3D* pVertexDataBegin = reinterpret_cast<sanVertex3D*>(reinterpret_cast<__int64>(pModelData) + pModelData->VertexAccess);
		sanVertex3D* pMappedMem = NULL;
		hr = pVertexBuffer->Map(0, NULL, (void**)&pMappedMem);
		memcpy(pMappedMem, pVertexDataBegin, vertexBufferSize);
		pVertexBuffer->Unmap(0, nullptr);

		// 頂点バッファビューを作成
		vertexBufferView.BufferLocation = pVertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(sanVertex3D);
		vertexBufferView.SizeInBytes = vertexBufferSize;
	}

	{
		// インデックスバッファ
		const UINT indexBufferSize = ((pModelData->VertexNum <= 0xffff) ? sizeof(WORD) : sizeof(DWORD)) * pModelData->IndexNum;

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
		if (hr != S_OK)
		{
			assert(hr == S_OK);
		}
		pIndexBuffer->SetName(L"sanModel::pIndexBuffer");

		// インデックスデータのコピー
		void* pIndexDataBegin = reinterpret_cast<void*>(reinterpret_cast<__int64>(pModelData) + pModelData->IndexAccess);
		void* pMappedMem = NULL;
		pIndexBuffer->Map(0, nullptr, (void**)&pMappedMem);
		memcpy(pMappedMem, pIndexDataBegin, indexBufferSize);
		pIndexBuffer->Unmap(0, nullptr);

		// インデックスバッファビューの作成
		indexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = (pModelData->VertexNum <= 0xffff) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = indexBufferSize;
	}
}

sanModel::~sanModel()
{
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pVertexBuffer);
	if (pModelData)
	{
		for (int i = 0; i < (int)pModelData->MaterialNum; i++)
		{
			SAFE_RELEASE(pMaterials[i].constbuff);
			SAFE_RELEASE(pMaterials[i].texbuff);
			SAFE_RELEASE(pMaterials[i].basicDescHeap);
		}
		delete[] pMaterials;
		delete[] (BYTE*)pModelData;
	}
}

void sanModel::execute()
{
}

void sanModel::render()
{
	// ローカル/ワールドマトリクスの計算
	calculateLocalMatrix();
	calculateWorldMatrix();

	// ビューポート変換マトリクス計算
	XMMATRIX WVP = XMMatrixMultiply(World, *sanCamera::getScreen());

	// メッシュ単位の描画
	sanModel_MeshData *pMesh = reinterpret_cast<sanModel_MeshData*>(reinterpret_cast<__int64>(pModelData) + pModelData->MeshAccess);
	for (int i = 0; i < (int)pModelData->MeshNum; i++)
	{
		int mid = pMesh[i].MaterialID;

		// コンスタントバッファに情報を設定
		XMStoreFloat4x4(&pMaterials[mid].pConstBuffer->WVP, XMMatrixTranspose(WVP));
		XMStoreFloat4x4(&pMaterials[mid].pConstBuffer->World, XMMatrixTranspose(World));
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->LightDir, *sanLight::getILightDir());
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->LightCol, *sanLight::getLightColor());
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->LightAmb, *sanLight::getAmbient());
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->CameraPos, *sanCamera::getPosition());
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->Diffuse, *getDiffuse(mid));
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->Ambient, *getAmbient(mid));
		XMStoreFloat4(&pMaterials[mid].pConstBuffer->Specular, *getSpecular(mid));

		if (zWrite)
		{
			if (lighting)
			{
				// ライティングの有効
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
				// ライティング有効
				if (transparent)
				{
					sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha_NL);
				}
				else
				{
					sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_NL);
				}
			}
		}
		else
		{
			if (lighting)
			{
				// ライティングの有効
				if (transparent)
				{
					// 半透明有効
					sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha_ZOff);
				}
				else
				{
					// 半透明無効
					sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_ZOff);
				}
			}
			else
			{
				// ライティング有効
				if (transparent)
				{
					sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_Alpha_NL_ZOff);
				}
				else
				{
					sanDirect3D::getCommandList()->SetPipelineState(pPipelineState_NL_ZOff);
				}
			}
		}

		sanDirect3D::getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		sanDirect3D::getCommandList()->SetDescriptorHeaps(1, &pMaterials[mid].basicDescHeap);

		D3D12_GPU_DESCRIPTOR_HANDLE basicHeapHandle = pMaterials[mid].basicDescHeap->GetGPUDescriptorHandleForHeapStart();
		sanDirect3D::getCommandList()->SetGraphicsRootDescriptorTable(0, basicHeapHandle);

		sanDirect3D::getCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
		sanDirect3D::getCommandList()->IASetIndexBuffer(&indexBufferView);
		sanDirect3D::getCommandList()->DrawIndexedInstanced(pMesh[i].IndexNum, 1, pMesh[i].StartIndex, pMesh[i].StartVertex, 0);
	}
}

int sanModel::getVertexNum()
{
	return pModelData ? pModelData->VertexNum : 0;
}

int sanModel::getIndexNum()
{
	return pModelData ? pModelData->IndexNum : 0;
}

int sanModel::getFaceNum()
{
	return pModelData ? pModelData->IndexNum / 3 : 0;
}

int sanModel::getMaterialNum()
{
	return pModelData ? pModelData->MaterialNum : 0;
}

int sanModel::getMeshNum()
{
	return pModelData ? pModelData->MeshNum : 0;
}

const char *sanModel::getTextureName(int id)
{
	if(id < 0 || id >= getMaterialNum()) return NULL;
	sanModel_MaterialData *pMaterialData = reinterpret_cast<sanModel_MaterialData*>(reinterpret_cast<__int64>(pModelData) + pModelData->MaterialAccess);
	return pMaterialData[id].Texture;
}

// メッシュデータ(先頭アドレス)の取得
sanModel_MeshData *sanModel::getMesh()
{
	if(!pModelData) return NULL;
	sanModel_MeshData *ret = reinterpret_cast<sanModel_MeshData*>(reinterpret_cast<__int64>(pModelData) + pModelData->MeshAccess);
	return ret;
}

// 頂点データ(先頭アドレス)の取得
sanVertex3D* sanModel::getVertex()
{
	if(!pModelData) return NULL;
	sanVertex3D *ret = reinterpret_cast<sanVertex3D*>(reinterpret_cast<__int64>(pModelData) + pModelData->VertexAccess);
	return ret;
}

// インデックスデータ(先頭アドレス)の取得
LPWORD sanModel::getIndex()
{
	if(!pModelData) return NULL;
	LPWORD ret = reinterpret_cast<LPWORD>(reinterpret_cast<__int64>(pModelData) + pModelData->IndexAccess);
	return ret;
}

// 拡散色の設定
void sanModel::setDiffuse(float r, float g, float b, float a, int id)
{
	if(pMaterials == NULL || getMaterialNum() <= 0) return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorSet(r, g, b, a);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorSet(r, g, b, a);
	}
}

void sanModel::setDiffuse(const XMVECTOR* v, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = *v;
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = *v;
	}
}

void sanModel::setDiffuseR(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorSetX(pMaterials[i].Diffuse, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorSetX(pMaterials[id].Diffuse, value);
	}
}

void sanModel::setDiffuseG(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorSetY(pMaterials[i].Diffuse, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorSetY(pMaterials[id].Diffuse, value);
	}
}

void sanModel::setDiffuseB(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorSetZ(pMaterials[i].Diffuse, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorSetZ(pMaterials[id].Diffuse, value);
	}
}

void sanModel::setDiffuseA(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorSetW(pMaterials[i].Diffuse, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorSetW(pMaterials[id].Diffuse, value);
	}
}

void sanModel::addDiffuseR(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorAdd(pMaterials[i].Diffuse, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorAdd(pMaterials[id].Diffuse, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
	}
}

void sanModel::addDiffuseG(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorAdd(pMaterials[i].Diffuse, XMVectorSet(0.0f, value, 0.0f, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorAdd(pMaterials[id].Diffuse, XMVectorSet(0.0f, value, 0.0f, 0.0f));
	}
}

void sanModel::addDiffuseB(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorAdd(pMaterials[i].Diffuse, XMVectorSet(0.0f, 0.0f, value, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorAdd(pMaterials[id].Diffuse, XMVectorSet(0.0f, 0.0f, value, 0.0f));
	}
}

void sanModel::addDiffuseA(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Diffuse = XMVectorAdd(pMaterials[i].Diffuse, XMVectorSet(0.0f, 0.0f, 0.0f, value));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Diffuse = XMVectorAdd(pMaterials[id].Diffuse, XMVectorSet(0.0f, 0.0f, 0.0f, value));
	}
}

// 拡散色の取得
XMVECTOR* sanModel::getDiffuse(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return NULL;
	return &pMaterials[id].Diffuse;
}

float sanModel::getDiffuseR(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 1.0f;
	return XMVectorGetX(pMaterials[id].Diffuse);
}

float sanModel::getDiffuseG(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 1.0f;
	return XMVectorGetY(pMaterials[id].Diffuse);
}

float sanModel::getDiffuseB(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 1.0f;
	return XMVectorGetZ(pMaterials[id].Diffuse);
}

float sanModel::getDiffuseA(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 1.0f;
	return XMVectorGetW(pMaterials[id].Diffuse);
}

// 環境色の設定
void sanModel::setAmbient(float r, float g, float b, float a, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorSet(r, g, b, a);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorSet(r, g, b, a);
	}
}

void sanModel::setAmbient(const XMVECTOR* v, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = *v;
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = *v;
	}
}

void sanModel::setAmbientR(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorSetX(pMaterials[i].Ambient, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorSetX(pMaterials[id].Ambient, value);
	}
}

void sanModel::setAmbientG(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorSetY(pMaterials[i].Ambient, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorSetY(pMaterials[id].Ambient, value);
	}
}

void sanModel::setAmbientB(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorSetZ(pMaterials[i].Ambient, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorSetZ(pMaterials[id].Ambient, value);
	}
}

void sanModel::setAmbientA(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorSetW(pMaterials[i].Ambient, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorSetW(pMaterials[id].Ambient, value);
	}
}

void sanModel::addAmbientR(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorAdd(pMaterials[i].Ambient, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorAdd(pMaterials[id].Ambient, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
	}
}

void sanModel::addAmbientG(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorAdd(pMaterials[i].Ambient, XMVectorSet(0.0f, value, 0.0f, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorAdd(pMaterials[id].Ambient, XMVectorSet(0.0f, value, 0.0f, 0.0f));
	}
}

void sanModel::addAmbientB(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorAdd(pMaterials[i].Ambient, XMVectorSet(0.0f, 0.0f, value, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorAdd(pMaterials[id].Ambient, XMVectorSet(0.0f, 0.0f, value, 0.0f));
	}
}

void sanModel::addAmbientA(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Ambient = XMVectorAdd(pMaterials[i].Ambient, XMVectorSet(0.0f, 0.0f, 0.0f, value));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Ambient = XMVectorAdd(pMaterials[id].Ambient, XMVectorSet(0.0f, 0.0f, 0.0f, value));
	}
}

// 環境色の取得
XMVECTOR* sanModel::getAmbient(int id)
{
	if(pMaterials == NULL || getMaterialNum() <= 0) return NULL;
	return &pMaterials[id].Ambient;
}

float sanModel::getAmbientR(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetX(pMaterials[id].Ambient);
}

float sanModel::getAmbientG(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetY(pMaterials[id].Ambient);
}

float sanModel::getAmbientB(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetZ(pMaterials[id].Ambient);
}

float sanModel::getAmbientA(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetW(pMaterials[id].Ambient);
}

// 鏡面反射色の設定
void sanModel::setSpecular(float r, float g, float b, float p, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorSet(r, g, b, p);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorSet(r, g, b, p);
	}
}

void sanModel::setSpecular(const XMVECTOR* v, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = *v;
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = *v;
	}
}

void sanModel::setSpecularR(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorSetX(pMaterials[i].Specular, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorSetX(pMaterials[id].Specular, value);
	}
}

void sanModel::setSpecularG(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorSetY(pMaterials[i].Specular, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorSetY(pMaterials[id].Specular, value);
	}
}

void sanModel::setSpecularB(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorSetZ(pMaterials[i].Specular, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorSetZ(pMaterials[id].Specular, value);
	}
}

void sanModel::setSpecularPower(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorSetW(pMaterials[i].Specular, value);
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorSetW(pMaterials[id].Specular, value);
	}
}

void sanModel::addSpecularR(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorAdd(pMaterials[i].Specular, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorAdd(pMaterials[id].Specular, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
	}
}

void sanModel::addSpecularG(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorAdd(pMaterials[i].Specular, XMVectorSet(0.0f, value, 0.0f, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorAdd(pMaterials[id].Specular, XMVectorSet(0.0f, value, 0.0f, 0.0f));
	}
}

void sanModel::addSpecularB(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorAdd(pMaterials[i].Specular, XMVectorSet(0.0f, 0.0f, value, 0.0f));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorAdd(pMaterials[id].Specular, XMVectorSet(0.0f, 0.0f, value, 0.0f));
	}
}

void sanModel::addSpecularPower(float value, int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return;
	if (id == -1)
	{
		for (int i = 0; i < getMaterialNum(); i++)
		{
			pMaterials[i].Specular = XMVectorAdd(pMaterials[i].Specular, XMVectorSet(0.0f, 0.0f, 0.0f, value));
		}
	}
	else if (id >= 0 && id < getMaterialNum())
	{
		pMaterials[id].Specular = XMVectorAdd(pMaterials[id].Specular, XMVectorSet(0.0f, 0.0f, 0.0f, value));
	}
}

// 鏡面反射色の取得
const XMVECTOR *sanModel::getSpecular(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return NULL;
	return &pMaterials[id].Specular;
}

float sanModel::getSpecularR(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetX(pMaterials[id].Specular);
}

float sanModel::getSpecularG(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetY(pMaterials[id].Specular);
}

float sanModel::getSpecularB(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 0.0f;
	return XMVectorGetZ(pMaterials[id].Specular);
}

float sanModel::getSpecularPower(int id)
{
	if (pMaterials == NULL || getMaterialNum() <= 0)return 1.0f;
	return XMVectorGetW(pMaterials[id].Specular);
}

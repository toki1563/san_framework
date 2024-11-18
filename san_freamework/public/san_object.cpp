//--------------------------------------------------------------//
//	"san_object.cpp"											//
//	     オブジェクト基底クラス									//
//													2024/11/13	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/san_environment.h"

// 静的共通データ
// 頂点座標
D3D12_INPUT_ELEMENT_DESC sanObject::inputElementDescs[]
{
	{ "POSITOPN", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,  	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

// パイプラインステート
ID3D12PipelineState* sanObject::pPipelineState = NULL;
ID3D12PipelineState* sanObject::pPipelineState_NL = NULL;	    // ライティングなし
ID3D12PipelineState* sanObject::pPipelineState_Alpha = NULL;
ID3D12PipelineState* sanObject::pPipelineState_Alpha_NL = NULL;	// ライティングなし

// 静的共通データの初期化
bool sanObject::initializeCommon()
{
	HRESULT hr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = sanDirect3D::getRootSignature();

	psoDesc.VS.pShaderBytecode = sanShader::getVShader(sanShader::eVertexShader::VS_3D)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getVShader(sanShader::eVertexShader::VS_3D)->getLength();
	psoDesc.PS.pShaderBytecode = sanShader::getPShader(sanShader::ePixelShader::PS_3D)->getCode();
	psoDesc.PS.BytecodeLength = sanShader::getPShader(sanShader::ePixelShader::PS_3D)->getLength();

	// ラスタライザーステートの設定
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

	// ブレンドステートの設定
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

	// 深度ステンシルテストの設定
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

	// ライティング有効＆不透明
	hr = sanDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
	pPipelineState->SetName(L"sanObject::pPipelineState");

	// ライティング無効＆不透明
	psoDesc.VS.pShaderBytecode = sanShader::getVShader(sanShader::eVertexShader::VS_3D_NoLight)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getVShader(sanShader::eVertexShader::VS_3D_NoLight)->getLength();
	psoDesc.VS.pShaderBytecode = sanShader::getPShader(sanShader::ePixelShader::PS_3D_NoLight)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getPShader(sanShader::ePixelShader::PS_3D_NoLight)->getLength();

	hr = sanDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState_NL));
	pPipelineState_NL->SetName(L"sanObject::pPipelineState_NL");

	// 半透明描画の有効
	psoDesc.BlendState.RenderTarget[0].BlendEnable = true;

	// ライティング有効＆半透明
	psoDesc.VS.pShaderBytecode = sanShader::getVShader(sanShader::eVertexShader::VS_3D)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getVShader(sanShader::eVertexShader::VS_3D)->getLength();
	psoDesc.PS.pShaderBytecode = sanShader::getPShader(sanShader::ePixelShader::PS_3D)->getCode();
	psoDesc.PS.BytecodeLength = sanShader::getPShader(sanShader::ePixelShader::PS_3D)->getLength();

	hr = sanDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState_Alpha));
	pPipelineState_Alpha->SetName(L"sanObject::pPipelineState_Alpha");

	// ライティング無効&半透明
	psoDesc.VS.pShaderBytecode = sanShader::getVShader(sanShader::eVertexShader::VS_3D_NoLight)->getCode();
	psoDesc.VS.BytecodeLength = sanShader::getVShader(sanShader::eVertexShader::VS_3D_NoLight)->getLength();
	psoDesc.PS.pShaderBytecode = sanShader::getPShader(sanShader::ePixelShader::PS_3D_NoLight)->getCode();
	psoDesc.PS.BytecodeLength = sanShader::getPShader(sanShader::ePixelShader::PS_3D_NoLight)->getLength();

	hr = sanDirect3D::getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState_Alpha_NL));
	pPipelineState_Alpha_NL->SetName(L"sanObject::pPipelineState_Alpha_NL");

	return true;
}

// 静的共通データの削除
void sanObject::terminateCommon()
{
	SAFE_RELEASE(pPipelineState);
	SAFE_RELEASE(pPipelineState_NL);
	SAFE_RELEASE(pPipelineState_Alpha);
	SAFE_RELEASE(pPipelineState_Alpha_NL);
}

sanObject::sanObject()
{
	Position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	Rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	Scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

	Local = XMMatrixIdentity();
	World = XMMatrixIdentity();

	executeEnable = true;
	renderEnable = true;

	lighting = true;
	transparent = false;

	pParent = NULL;
}

sanObject::~sanObject()
{
}

void sanObject::execute()
{
}

void sanObject::render()
{
}

// マトリクスの計算
void sanObject::calculateLocalMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(getPositionX(), getPositionY(), getPositionZ());
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(getRotationX(), getRotationY(), getRotationZ());
	XMMATRIX scale = XMMatrixScaling(getScaleX(), getScaleY(), getScaleZ());
	Local = scale * rotate* trans; // ここの順番大事
}

void sanObject::calculateWorldMatrix()
{
	if (pParent == NULL)
	{
		World = Local;
	}
	else
	{
		World = Local * (*pParent->getWorld());
	}
}

// 移動量の設定
void sanObject::setPosition(float x, float y, float z)
{
	Position = XMVectorSet(x, y, z, 0.0f);
}

void sanObject::setPosition(const XMVECTOR *v)
{
	Position = *v;
}

void sanObject::setPositionX(float value)
{
	Position = XMVectorSetX(Position, value);
}

void sanObject::setPositionY(float value)
{
	Position = XMVectorSetY(Position, value);
}

void sanObject::setPositionZ(float value)
{
	Position = XMVectorSetZ(Position, value);
}

void sanObject::addPosition(float x, float y, float z)
{
	Position = XMVectorAdd(Position, XMVectorSet(x, y, z, 0.0f));
}

void sanObject::addPosition(const XMVECTOR *v)
{
	Position = XMVectorAdd(Position, *v);
}

void sanObject::addPositionX(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanObject::addPositionY(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void sanObject::addPositionZ(float value)
{
	Position = XMVectorAdd(Position, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

// 移動値の取得
XMVECTOR *sanObject::getPosition()
{
	return &Position;
}

float sanObject::getPositionX(void)
{
	return XMVectorGetX(Position);
}

float sanObject::getPositionY(void)
{
	return XMVectorGetY(Position);
}

float sanObject::getPositionZ(void)
{
	return XMVectorGetZ(Position);
}

// 回転値の設定
void sanObject::setRotation(float x, float y, float z)
{
	Rotation = XMVectorSet(x, y, z, 0.0f);
}

void sanObject::setRotation(const XMVECTOR *v)
{
	Rotation = *v;
}

void sanObject::setRotationX(float radian)
{
	Rotation = XMVectorSetX(Rotation, radian);
}

void sanObject::setRotationY(float radian)
{
	Rotation = XMVectorSetY(Rotation, radian);
}

void sanObject::setRotationZ(float radian)
{
	Rotation = XMVectorSetZ(Rotation, radian);
}

void sanObject::addRotation(float x, float y, float z)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(x, y, z, 0.0f));
}

void sanObject::addRotation(const XMVECTOR* v)
{
	Rotation = XMVectorAdd(Rotation, *v);
}

void sanObject::addRotationX(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(radian, 0.0f, 0.0f, 0.0f));
}

void sanObject::addRotationY(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(0.0f, radian, 0.0f, 0.0f));
}

void sanObject::addRotationZ(float radian)
{
	Rotation = XMVectorAdd(Rotation, XMVectorSet(0.0f, 0.0f, radian, 0.0f));
}

// 回転値の取得
XMVECTOR *sanObject::getRotation()
{
	return &Rotation;
}

float sanObject::getRotationX(void)
{
	return XMVectorGetX(Rotation);
}

float sanObject::getRotationY(void)
{
	return XMVectorGetY(Rotation);
}

float sanObject::getRotationZ(void)
{
	return XMVectorGetZ(Rotation);
}

// 拡大値の設定
void sanObject::setScale(float x, float y, float z)
{
	Scale = XMVectorSet(x, y, z, 0.0f);
}

void sanObject::setScale(const XMVECTOR* v)
{
	Scale = *v;
}

void sanObject::setScaleX(float value)
{
	Scale = XMVectorSetX(Scale, value);
}

void sanObject::setScaleY(float value)
{
	Scale = XMVectorSetY(Scale, value);
}

void sanObject::setScaleZ(float value)
{
	Scale = XMVectorSetZ(Scale, value);
}

void sanObject::addScale(float x, float y, float z)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(x, y, z, 0.0f));
}

void sanObject::addScale(const XMVECTOR *v)
{
	Scale = XMVectorAdd(Scale, *v);
}

void sanObject::addScaleX(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void sanObject::addScaleY(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void sanObject::addScaleZ(float value)
{
	Scale = XMVectorAdd(Scale, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

// 拡大値の取得
XMVECTOR *sanObject::getScale()
{
	return &Scale;
}

float sanObject::getScaleX(void)
{
	return XMVectorGetX(Scale);
}

float sanObject::getScaleY(void)
{
	return XMVectorGetY(Scale);
}

float sanObject::getScaleZ(void)
{
	return XMVectorGetZ(Scale);
}

// ワールド行列の取得
XMMATRIX *sanObject::getWorld()
{
	return &World;
}

// ワールド座標の取得
void sanObject::getWorldPosition(XMVECTOR *v)
{
	*v = World.r[3];
}

float sanObject::getWorldPositionX()
{
	return XMVectorGetX(World.r[3]);
}

float sanObject::getWorldPositionY()
{
	return XMVectorGetY(World.r[3]);
}

float sanObject::getWorldPositionZ()
{
	return XMVectorGetZ(World.r[3]);
}

// ワールド行列の設定
void sanObject::setWorld(XMMATRIX* m)
{
	if (m)
	{
		World = *m;
	}
}

// 実行状態の設定
void sanObject::setExecuteEnble(bool flag)
{
	executeEnable = flag;
}

// 描画状態の設定
void sanObject::setRenderEnable(bool flag)
{
	renderEnable = flag;
}

// 実行状態の取得
bool sanObject::isExcuteEnable()
{
	return executeEnable;
}

// 描画状態の取得
bool sanObject::isRenderEnable()
{
	return renderEnable;
}

// ライティングの有効/無効の設定
void sanObject::setLighting(bool flag)
{
	lighting = flag;
}

// 半透明の有効/無効の設定
void sanObject::setTransparent(bool flag)
{
	transparent = flag;
}

// ライティングの有効/無効の取得
bool sanObject::getLighting()
{
	return lighting;
}

// 半透明の有効/無効の取得
bool sanObject::getTransparent()
{
	return transparent;
}

// 階層構造の親の設定
void sanObject::setParent(sanObject* p)
{
	pParent = p;
}

// attach / dettach : 合成 / 解除

// 階層構造の親の取得
sanObject* sanObject::getParent(void)
{
	return pParent;
}
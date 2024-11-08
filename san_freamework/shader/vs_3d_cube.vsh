struct pixelIn
{
    float4 pos : SV_POSITION;
    float4 col : COLORO;
    float2 tx0 : TEXCOORD0;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float4 main(pixelIn IN) : SV_Target
{
    // テクスチャのサンプリング
    float4 color = txDiffuse.Sample(samLinear, IN.tx0) * IN.col;

    return color;
}

struct vertexIn
{
    float4 pos : POSITION0;
    float3 nor : NORMAL0;
    float2 tx0 : TEXCOORD0;
};

struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tx0 : TEXCOORD0;
};

// コンスタントバッファ
cbuffer ConstantBuffer : register(b0)
{
    float4x4 WVP; // World * View * Proj
    float4x4 World; // ワールド行列
    float4 LightDir; // 光源方向 (単位ベクトル)
    float4 LightCol; // 光源の色
    float4 LightAmb; // 環境光の色
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    // 頂点座標をWVP行列で変換
    OUT.pos = mul(IN.pos, WVP);

    // 法線ベクトルをワールド行列で変換
    float3 n = mul(float4(IN.nor.xyz, 0), World).xyz;

    // ライティング (dot: 内積)
    // saturate関数で-1から0に制限
    // LightColを掛けて明るさを決定し、環境光を追加
    OUT.col.xyz = saturate(dot(normalize(n), LightDir.xyz)) * LightCol.xyz + LightAmb.xyz;
    OUT.col.w = 1;

    // テクスチャ座標
    OUT.tx0 = IN.tx0;

    return OUT;
}

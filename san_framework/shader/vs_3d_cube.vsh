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
    //�e�N�X�`���̃T���v�����O
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
    float4 col : COLORO;
    float2 tx0 : TEXCOORD0;
};

// �R���X�^���g�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    float4x4 WVP; // World * View * Proj
    float4x4 World; // ���[���h�s��
    float4 LightDir; // ���s������(�t)����
    float4 LightCol; // ���s�����̐F
    float4 LightAmb; // �����̐F
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    //���_���W��WVP�s��ŕϊ�
    OUT.pos = mul(IN.pos, WVP);

    //�@���x�N�g�������[���h�s��ŕϊ�
    float3 n = mul(float4(IN.nor.xyz, 0), World).xyz;

    //���C�e�B���O(dot:����)
    //saturate��-1��0�ɂ��鏈��
    //LightCol�������Ė��邳���o���čŒ�̌����A���r�G���g�ő���
    OUT.col.xyz = saturate(dot(normalize(n), LightDir.xyz)) * LightCol.xyz + LightAmb.xyz;
    OUT.col.w = 1;

    //�e�N�X�`�����W
    OUT.tx0 = IN.tx0;

    return OUT;
}
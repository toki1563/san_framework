struct vertexIn
{
    float4 pos : POSITION0;
    float4 nor : NORMAL0;
    float4 col : COLOR0;
    float2 tx0 : TEXCOORD0;
};

struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tx0 : TEXCOORD0;
    float3 nor : TEXCOORD1; // �@���x�N�g��
    float3 eye : TEXCOORD2; // �����x�N�g��
};

cbuffer ConstantBuffer
{
    float4x4 WVP; // World*View*Proj
    float4x4 World; // ���[���h�s��
    float4 LightDir; // �������� (�P�ʃx�N�g��)
    float4 LightCol; // �����̐F
    float4 LightAmb; // �����̐F
    float4 CameraPos; // �J�����̃��[���h���W
    float4 Diffuse; // �}�e���A���̊g�U�F
    float4 Ambient; // �}�e���A���̊����F
    float4 Specular; // �}�e���A���̋��ʔ��˓���
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

	// ���_���W���r���[�|�[�g���W�ɕϊ�
    OUT.pos = mul(IN.pos, WVP);

	// �@���x�N�g�������[���h�s��ŕϊ�
    float3 n = mul(float4(IN.nor.xyz, 0), World).xyz;
    OUT.nor = n;

	// ���_�J���[
    OUT.col = IN.col;

	// ���C�e�B���O
    OUT.col.xyz = IN.col.xyz * saturate(dot(normalize(n), LightDir.xyz)) * LightCol.xyz + LightAmb.xyz;

	// ���_���W�����[���h�s��ŕϊ�
    float3 wPos = mul(IN.pos, World).wyz;

	// �����x�N�g��
    OUT.eye = CameraPos.xyz - wPos;

	// �}�e���A���J���[
    OUT.col *= Diffuse;
    OUT.col.xyz += Ambient.xyz;

	// �e�N�X�`�����W
    OUT.tx0 = IN.tx0;

    return OUT;
}
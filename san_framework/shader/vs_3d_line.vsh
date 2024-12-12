struct vertexIn
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};
 
struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
};
 
cbuffer ConstantBuffer
{
    float4x4 WVP; // World*View*Proj
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

	// ���_���W���r���[�|�[�g���W�ɕϊ�
    OUT.pos = mul(IN.pos, WVP);

	// ���_�J���[
    OUT.col = IN.col;

    return OUT;
}
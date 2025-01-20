#include"Particle.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
};



//�R���X�^���g�o�b�t�@�̒�`
//�g�p�� : ConstantBuffer<�\����> �ϐ��� : register(b0);
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
//ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
    
};


PixcelShaderOutput main(VertexShaderOutPut input)
{
    PixcelShaderOutput output;
  
    
    //float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float4 tranceformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, tranceformedUV.xy);
   
    output.color = gMaterial.color * textureColor*input.color;
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
};





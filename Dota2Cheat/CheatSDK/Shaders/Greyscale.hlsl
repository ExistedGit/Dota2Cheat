// Greyscale pixel shader for unlearned abilities
Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 PSMain(PS_INPUT input) : SV_Target
{
    float4 color = tex.Sample(samplerState, input.texcoord);
    float grey = dot(color.rgb, float3(0.299, 0.587, 0.114));
    return float4(grey, grey, grey, color.a);
}
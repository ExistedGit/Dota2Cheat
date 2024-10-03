// Shader for ability icons when there is no mana to cast them
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
    
    // Greyscale => blue mask
    float grey = dot(color.rgb, float3(0.299, 0.587, 0.114));
    return float4(grey * float3(0.0, 0.45, 1.0), color.a);
}
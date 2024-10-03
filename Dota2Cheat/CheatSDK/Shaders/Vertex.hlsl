struct VSInput
{
    float2 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.pos = float4(input.pos.x, input.pos.y, 0.0f, 1.0f);
    output.tex = input.tex;
    return output;
}
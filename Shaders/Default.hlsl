
cbuffer Builtins : register(b0) {
    float4x4 LIGHTNING3D_MATRIX_WVP;
};

cbuffer MaterialConstants : register(b1) {
    float4 Color;
};

struct VSOutput {
    float4 pos : SV_POSITION;
};

VSOutput VSMain(float3 pos : POSITION) {
    VSOutput output;

    output.pos = mul(float4(pos, 1.0f), LIGHTNING3D_MATRIX_WVP);
    
    return output;
}

float4 PSMain(VSOutput input) : SV_TARGET {
    return Color;
}

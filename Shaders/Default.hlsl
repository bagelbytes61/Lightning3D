
cbuffer Builtins : register(b0) {
    float4x4 LIGHTNING3D_MATRIX_WVP;
};

float4 VSMain(float3 pos : POSITION) : SV_POSITION {

    return mul(float4(pos, 1.0f), LIGHTNING3D_MATRIX_WVP);
}

cbuffer MaterialConstants : register(b1) {
    float4 Color;
};

float4 PSMain() : SV_TARGET {
    return Color;
}

//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

float4 SimpleVertexShader(float3 pos : POSITION) : SV_POSITION
{
	pos.x = pos.x * 0.5f;
    return float4(pos, 1.0f);
}
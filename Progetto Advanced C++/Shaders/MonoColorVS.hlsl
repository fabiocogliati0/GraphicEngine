cbuffer TransformsBuffer : register(cb0)
{
	float4x4 World;
};

cbuffer CameraBuffer : register(cb1)
{
	float4x4 View;
	float4x4 Projection;
	float3 CameraPosition;
}

cbuffer Material : register(cb2)
{
	float4 Color;
};


struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
	float4 col : COLOR0;
};


PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;

	float4 worldSpacePos = mul(World, float4(input.pos, 1.0f));
	float4 cameraSpacePos = mul(View, worldSpacePos);
	float3 worldSpaceNormal = mul((float3x3)World, input.norm);
	float3 viewDir = normalize(CameraPosition - worldSpacePos.xyz);

	output.pos = mul(Projection, cameraSpacePos);
	output.col = Color;
	return output;
}


cbuffer MatrixBuffer : register( b0 )
{
	matrix matWorld;
	matrix matView;
	matrix matProjection;
};

struct VertexInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

struct VertexOutType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

VertexOutType vs_main( VertexInputType input )
{
	VertexOutType output;
	
	input.position.w = 1.0f;
	
	output.position = mul(input.position, matWorld);
	output.position = mul(input.position, matView);
	output.position = mul(input.position, matProjection);
	
	output.color = input.color;
	output.uv = input.uv;

	return output;
}
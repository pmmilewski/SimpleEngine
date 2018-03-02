//////////////////////////////////////
// Filename: ColorPixelShader.hlsl //
//////////////////////////////////////

///////////////////////////////
// TYPEDEFS                  //
///////////////////////////////

struct PixelInputType
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

/*
float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
} */

float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}
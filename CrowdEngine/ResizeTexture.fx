/////////////////////////////////////////////////////////////////////////////////////////////
//
//							ResizeTexture
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "CommonFunctions.fx"

cbuffer ResizeTextureBuffer
{
	float g_sourceScaleX;
	float g_sourceScaleY;
	int g_destWidth;
	int g_destHeight;
	int g_numSamplesX;
	int g_numSamplesY;
}

Texture2D g_SourceTexture;

SamplerState CloseSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

float4 GetSample(float x, float y) // x et y sont en réalité des entiers
{
	return g_SourceTexture.Sample(CloseSampler, float2( ((float)x+0.5f)*g_sourceScaleX , ((float)y+0.5f)*g_sourceScaleY ));
}

// Vertex shader
struct OutVS
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

OutVS ResizeVS(uint VertexId : SV_VertexID)
{
	OutVS output;

	if(VertexId == 0) { output.pos = float4(-1.0f, 1.0f, 1.0f, 1.0f); output.uv = float2(0.0f, 0.0f); return output; }	// haut gauche
	if(VertexId == 1) { output.pos = float4(1.0f, 1.0f, 1.0f, 1.0f); output.uv = float2(1.0f, 0.0f); return output; }	// haut droite
	if(VertexId == 2) { output.pos = float4(-1.0f, -1.0f, 1.0f, 1.0f); output.uv = float2(0.0f, 1.0f); return output; }	// bas gauche
	output.pos = float4(1.0f, -1.0f, 1.0f, 1.0f); output.uv = float2(1.0f, 1.0f); return output;						// bas droite
}

float4 ResizePS(float4 Pos : SV_Position, float2 UV : TEXCOORD) : SV_Target
{
	float x = UV.x*g_destWidth;
	float y = UV.y*g_destHeight;

	float4 averageSample = 0;
	for(unsigned int i = 0; i < g_numSamplesX; i++)
	{
		float4 columnAverageSample = 0;
		for(unsigned int j = 0; j < g_numSamplesY; j++)
		{
			float4 sampleColor = GetSample(x*(float)g_numSamplesX + (float)i, y*(float)g_numSamplesY + (float)j);
			columnAverageSample += sampleColor*sampleColor;
		}

		averageSample += columnAverageSample/(float) (g_numSamplesY);
	}
	averageSample = averageSample/(float) (g_numSamplesX);
	return averageSample;

	/*float4 averageSample = 0;
	for(unsigned int i = 0; i < g_numSamplesX; i++)
	{
		float4 columnAverageSample = 0;
		for(unsigned int j = 0; j < g_numSamplesY; j++)
		{
			columnAverageSample += g_SourceTexture.Sample(CloseSampler, UV);
		}

		averageSample += columnAverageSample/(float) g_numSamplesX;
	}
	return averageSample/(float)g_numSamplesY;*/
}

// Techniques
technique10 ResizeTextureTech
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_4_0, ResizeVS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_4_0, ResizePS()) );
	}
}
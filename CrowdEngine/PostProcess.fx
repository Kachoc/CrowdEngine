#include "World.fx"
#include "CommonFunctions.fx"

/////////////////////////////////////////////////////////////////////////////////////////////
//
//							Post-Process
//
/////////////////////////////////////////////////////////////////////////////////////////////

Texture2D g_DepthBuf;
Texture2D g_LightPassOutput;

cbuffer PostProcessBuffer
{
	float4 g_fogColor;
	float g_fogIntensity;
	float g_fogHeightDim;
	float g_colorIntensityAdjustment;
}

SamplerState CloseSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

// Vertex shader
struct OutVS
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

OutVS PostProcessVS(uint VertexId : SV_VertexID)
{
	OutVS output;

	if(VertexId == 0) { output.pos = float4(-1.0f, 1.0f, 1.0f, 1.0f); output.uv = float2(0.0f, 0.0f); return output; }	// haut gauche
	if(VertexId == 1) { output.pos = float4(1.0f, 1.0f, 1.0f, 1.0f); output.uv = float2(1.0f, 0.0f); return output; }	// haut droite
	if(VertexId == 2) { output.pos = float4(-1.0f, -1.0f, 1.0f, 1.0f); output.uv = float2(0.0f, 1.0f); return output; }	// bas gauche
	output.pos = float4(1.0f, -1.0f, 1.0f, 1.0f); output.uv = float2(1.0f, 1.0f); return output;						// bas droite
}

float4 PostProcessPS(float4 Pos : SV_Position, float2 UV : TEXCOORD) : SV_Target
{
	float4 colorFromLightPass = g_LightPassOutput.Sample(CloseSampler, UV);
	//float z = g_DepthBuf.Sample(CloseSampler, UV.xy);

	return colorFromLightPass*g_colorIntensityAdjustment;
}

float4 PPFogPS(float4 Pos : SV_Position, float2 UV : TEXCOORD) : SV_Target
{
	float4 colorFromLightPass = g_LightPassOutput.Sample(CloseSampler, UV);
	float z = g_DepthBuf.Sample(CloseSampler, UV.xy);

	//
	//	Calcul des coordonnées globales du pixel
	//
	float4 clipPos;
	clipPos.x = 2.0f*UV.x - 1.0f;
	clipPos.y = -2.0f*UV.y + 1.0f;
	clipPos.z = z;
	clipPos.w = 1.0f;

	float4 position = mul(clipPos, g_InvViewProjMatrix);
	position.w = 1.0f / position.w;
	position.xyz *= position.w;
	
	// Specular + fog
	float3 toEyeUnn = g_CameraPosition - position;
	float distFromEye = CalculateNorm(toEyeUnn);

	float fogDensity = g_fogIntensity*exp(-g_fogHeightDim*g_CameraPosition.y)*distFromEye*(1-exp(g_fogHeightDim*toEyeUnn.y))/(-g_fogHeightDim*toEyeUnn.y);
	float a = 1.0f-exp(-fogDensity);
	float4 color = a*g_fogColor + (1.0f-a)*colorFromLightPass;

	return float4(color.rgb, 1.0f);
}

// Techniques
technique10 PostProcessTech
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_4_0, PostProcessVS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_4_0, PostProcessPS()) );
	}
}
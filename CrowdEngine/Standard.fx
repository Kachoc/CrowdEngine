#include "World.fx"

cbuffer Object
{
	float4x4 g_PositionMatrix;
	float4 g_Diffuse;
}

Texture2D g_DiffuseSpecularTexture;

SamplerState colorSampler
{
	Filter = ANISOTROPIC;
	AddressU = WRAP;
	AddressV = WRAP;
	MaxAnisotropy = 16;
};

struct OutputVS
{
	float4 position : SV_Position;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

OutputVS LambertVS(float3 position : POSITION, float3 normal : NORMAL, float2 UV : TEXCOORD0)
{
	float4x4 fViewProj = mul( g_PositionMatrix, g_ViewProjMatrix);
	float4x4 fRotation = mul( g_PositionMatrix, g_ViewMatrix);

	OutputVS outVS;
	outVS.position = mul(float4(position, 1.0f), fViewProj);
	outVS.normal = mul(float4(normal, 0.0f), fRotation).xyz;
	outVS.uv = UV;
	return outVS;
}

struct OutPS
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

OutPS LambertPS(float4 Pos : SV_Position, float3 Normal : NORMAL, float2 UV : TEXCOORD0)
{
	OutPS outPS;
	float4 colorFromTex = g_DiffuseSpecularTexture.Sample(colorSampler, UV);
	outPS.color = g_Diffuse*colorFromTex;
	//outPS.color.a /= 255.0f;
	outPS.normal = float4(normalize(Normal), 1.0f);

	return outPS;
}

technique10 LambertTech
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, LambertVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, LambertPS() ) );
    }  
}
#include "World.fx"

cbuffer Terrain
{
	float4x4	g_PositionMatrix;
	float4		g_Diffuse;
	float		g_HeightConstant;
	float		g_HeightMultiplier;
	float		g_SquareWidth;
	float		g_SquareUVWidth;
}

Texture2D g_DiffuseSpecularTexture;
Texture2D g_HeightMap;

SamplerState ColorSampler
{
	Filter = ANISOTROPIC;
	AddressU = WRAP;
	AddressV = WRAP;
	MaxAnisotropy = 16;
};

SamplerState HeightMapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

//
//	Terrain geometry
//

struct OutputGeoVS
{
	float4 position : SV_Position;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

OutputGeoVS TerrainVS(float2 pos2D : POSITION) // coordonnées de pos2D : x représente le numéro en abscisse du sommet, y le numéro en ordonnée du sommet (-> une case fait 1.0f unité de largeur)
{
	float4x4 fViewProj = mul( g_PositionMatrix, g_ViewProjMatrix);
	float4x4 fRotation = mul( g_PositionMatrix, g_ViewMatrix);

	// Position
	float h = g_HeightMap.SampleLevel(HeightMapSampler, float2(pos2D.x*g_SquareUVWidth, -pos2D.y*g_SquareUVWidth), 0)*g_HeightMultiplier;

	float3 position = float3(pos2D.x*g_SquareWidth, h+g_HeightConstant, pos2D.y*g_SquareWidth);

	float h1 = g_HeightMap.SampleLevel(HeightMapSampler, float2(pos2D.x*g_SquareUVWidth, (-pos2D.y-1.0f)*g_SquareUVWidth), 0)*g_HeightMultiplier;
	float h2 = g_HeightMap.SampleLevel(HeightMapSampler, float2((pos2D.x+1.0f)*g_SquareUVWidth, -pos2D.y*g_SquareUVWidth), 0)*g_HeightMultiplier;

	float3 v1 = float3(0, h1-h, g_SquareWidth);
	float3 v2 = float3(g_SquareWidth, h2-h, 0);

	float3 normal = normalize(cross(v1, v2));

	OutputGeoVS outVS;
	outVS.position = mul(float4(position, 1.0f), fViewProj);
	outVS.normal = mul(float4(normal, 0.0f), fRotation).xyz;
	outVS.uv = pos2D*g_SquareUVWidth;
	return outVS;
}

struct OutGeoPS
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

OutGeoPS TerrainPS(float4 Pos : SV_Position, float3 Normal : NORMAL, float2 UV : TEXCOORD0)
{
	OutGeoPS outPS;
	float4 colorFromTex = g_DiffuseSpecularTexture.Sample(ColorSampler, UV);
	outPS.color = g_Diffuse*colorFromTex;
	//outPS.color = float4(normalize(Normal), 1.0f);
	outPS.normal = float4(normalize(Normal), 1.0f);

	return outPS;
}

technique10 TerrainTech
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, TerrainVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, TerrainPS() ) );
    }  
}
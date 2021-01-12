//
//	Terrain shadow
//

#include "LightPass.fx"

cbuffer Terrain
{
	float4x4	g_PositionMatrix;
	float4		g_Diffuse;
	float		g_HeightConstant;
	float		g_HeightMultiplier;
	float		g_SquareWidth;
	float		g_SquareUVWidth;
}

Texture2D g_HeightMap;

SamplerState HeightMapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct OutputDepthVS
{
	float4 	position : SV_Position;
	float 	depth : TEXCOORD0;
};

OutputDepthVS TerrainDepthVS(float2 pos2D : POSITION) // coordonnées de pos2D : x représente le numéro en abscisse du sommet, y le numéro en ordonnée du sommet (-> une case fait 1.0f unité de largeur)
{
	// Position
	float h = g_HeightMap.SampleLevel(HeightMapSampler, float2(pos2D.x*g_SquareUVWidth, -pos2D.y*g_SquareUVWidth), 0)*g_HeightMultiplier;

	float3 position = float3(pos2D.x*g_SquareWidth, h+g_HeightConstant, pos2D.y*g_SquareWidth);

	OutputDepthVS outVS;
	outVS.position = mul(float4(position, 1.0f), mul(g_PositionMatrix, g_ShadowViewProjMatrix));
	outVS.depth = g_ShadowViewProjMatrix._44*outVS.position.z + (1.0f - g_ShadowViewProjMatrix._44)*outVS.position.w; // Permet de distinguer les cas projection perspective et projection orthographique
	
	return outVS;
}

struct OutDepthPS
{
	float4 color : SV_Target;
};

float TerrainDepthPS(float4 Pos : SV_Position, float depth : TEXCOORD0) : SV_Target
{
	return depth;
}

technique10 TerrainDepthTech
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, TerrainDepthVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, TerrainDepthPS() ) );
    }  
}
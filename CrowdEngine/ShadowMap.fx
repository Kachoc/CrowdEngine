/////////////////////////////////////////////////////////////////////////////////////////////
//
//							Build shadow map
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "LightPass.fx"

cbuffer Object
{
	float4x4 g_PositionMatrix;
}

struct OutVS
{
	float4 position : SV_Position;
	float depth : TEXCOORD0;
};


//
//	Rendu de la shadow map
//

OutVS ShadowMapVS(float3 position : POSITION)
{
	OutVS outVS;
	outVS.position = mul(float4(position, 1.0f), mul(g_PositionMatrix, g_ShadowViewProjMatrix));
	outVS.depth = g_ShadowViewProjMatrix._44*outVS.position.z + (1.0f - g_ShadowViewProjMatrix._44)*outVS.position.w; // Permet de distinguer les cas projection perspective et projection orthographique

	return outVS;
}

float4 ShadowMapPS(float4 Pos : SV_Position, float depth : TEXCOORD0) : SV_Target
{
	return depth;
}

technique10 ComputeShadowMap
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, ShadowMapVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, ShadowMapPS() ) );
    }
}
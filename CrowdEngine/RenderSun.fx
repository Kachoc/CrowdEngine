#include "LightPass.fx"
#include "CommonFunctions.fx"

/////////////////////////////////////////////////////////////////////////////////////////////
//
//							Render Sun
//
/////////////////////////////////////////////////////////////////////////////////////////////

cbuffer Sun
{
	float4 g_SunDirection;
	float4 g_SunDiffuse;
	float4 g_SunAmbient;
	float4 g_SunSpecular;

	float4 g_SkyColor;
	float4 g_HorizonColor;
	float g_SkyBlendExponent;
	float g_SunRadiusExponent;
	float g_SunRadiusMultiplier;
}

Texture2DArray SunShadowMapsArray;

struct ShadowMapResolution
{
	unsigned int Width;
	unsigned int Height;
};

cbuffer ShadowMaps
{
	float4x4				g_ShadowMapViewProjMatrices[16];
	unsigned int			g_NumShadowMapsLevels;
	float					g_Epsilon;
}

SamplerState bufSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState shadowMapSampler
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

OutVS RenderSunVS(uint VertexId : SV_VertexID)
{
	OutVS output;

	if(VertexId == 0) { output.pos = float4(-1.0f, 1.0f, 1.0f, 1.0f); output.uv = float2(0.0f, 0.0f); return output; }	// haut gauche
	if(VertexId == 1) { output.pos = float4(1.0f, 1.0f, 1.0f, 1.0f); output.uv = float2(1.0f, 0.0f); return output; }	// haut droite
	if(VertexId == 2) { output.pos = float4(-1.0f, -1.0f, 1.0f, 1.0f); output.uv = float2(0.0f, 1.0f); return output; }	// bas gauche
	output.pos = float4(1.0f, -1.0f, 1.0f, 1.0f); output.uv = float2(1.0f, 1.0f); return output;						// bas droite
}

#define NUM_SHADOW_SAMPLES 1

#define PI 3.141592654f

float4 CalculateSkyColor(float3 lookDir)
{
	float lookCos = max(dot(lookDir, float3(0, 1.0f, 0)), 0);
	float skyBlend = pow(sqrt(1.0f - lookCos*lookCos), g_SkyBlendExponent);
	//float4 skyColor = skyBlend*g_SkyColor + (1.0f-skyBlend)*g_HorizonColor;
	float4 skyColor = (1.0f-skyBlend)*g_SkyColor + skyBlend*g_HorizonColor;

	float sunIntensity = g_SunRadiusMultiplier*pow(max(-dot(lookDir, g_SunDirection), 0), g_SunRadiusExponent);

	return skyColor + sunIntensity*g_SunDiffuse;
}

// Pixel shader
float4 RenderSunPS(float4 Pos : SV_Position, float2 UV : TEXCOORD) : SV_Target
{
	float xMultiply = g_CameraRatio*tan(g_CameraFovY/2.0f);
	float yMultiply = tan(g_CameraFovY/2.0f);

	float4 diffuseFB = DiffuseBuf.Sample(bufSampler, UV.xy); // diffuse from buffer
	float3 normalFB = NormalBuf.Sample(bufSampler, UV.xy).xyz;
	float z = DepthBuf.Sample(bufSampler, UV.xy);

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

	//
	//	Ciel
	//
	if(z == 1.0f) {
		return CalculateSkyColor(normalize(position - g_CameraPosition.xyz));//g_SunDiffuse*max(dot(g_SunDirection.xyz, float3(0, -1.0f, 0)), 0);
	}


	//
	//	Pixel solide
	//
	float3 normal = mul(float4(normalFB, 0.0f), g_InvViewMatrix).xyz;


	//
	//	Calcul des couleurs
	//

	// Ambient
	float4 ambient = g_SunAmbient*diffuseFB;// * max(dot(g_SunDirection.xyz, float3(0, -1.0f, 0)), 0);

	// Specular + fog
	float3 toEyeUnn = g_CameraPosition.xyz - position.xyz;
	float distFromEye = CalculateNorm(toEyeUnn);

	// Shadow map
	// Diffuse
	float4 diffuse = 0.0f;
	float4 specular = 0.0f;

	float3 shadowMapPosition;// = mul(float4(position.xyz, 1.0f), g_ShadowMapViewProjMatrices[0]).xyz;
	unsigned int shadowMapIndice = 0;
	//float4 shadowMapColor = 1.0f;
	for(shadowMapIndice = 0; shadowMapIndice < g_NumShadowMapsLevels; shadowMapIndice++)
	{
		shadowMapPosition = mul(float4(position.xyz, 1.0f), g_ShadowMapViewProjMatrices[shadowMapIndice]).xyz;
		
		if(shadowMapPosition.x >= -1.0f && shadowMapPosition.x <= 1.0f
			&& shadowMapPosition.y >= -1.0f && shadowMapPosition.y <= 1.0f
			&& shadowMapPosition.z >= 0.0f && shadowMapPosition.z <= 1.0f)
		{
			break;
		}
	}
	if(shadowMapIndice < g_NumShadowMapsLevels)
	{
		/*if(shadowMapIndice % 3 == 0) { shadowMapColor = float4(1.0f, 0, 0, 1.0f); }
		else if(shadowMapIndice % 3 == 1) { shadowMapColor = float4(0, 1.0f, 0, 1.0f); }
		else { shadowMapColor = float4(0, 0, 1.0f, 1.0f); }*/

		shadowMapPosition.x = 0.5f*shadowMapPosition.x + 0.5f;
		shadowMapPosition.y = -0.5f*shadowMapPosition.y + 0.5f;

		float shadowMapDepth = SunShadowMapsArray.Sample(shadowMapSampler, float3(shadowMapPosition.xy, (float) shadowMapIndice));
		if(shadowMapPosition.z - g_Epsilon < shadowMapDepth)
		{
			diffuse = max(-dot(normal, g_SunDirection), 0.0f)*g_SunDiffuse*diffuseFB;

			// Specular
			if(dot(g_SunDirection.xyz, normal)<0.0f)
			{
				float3 toEye = toEyeUnn/distFromEye;
				float3 reflection = normalize(g_SunDirection - 2*dot(g_SunDirection, normal)*normal);
				float specDot = dot(toEye, reflection);
				specular = ComputeSpecCoef(specDot, dot(normal, g_SunDirection), diffuseFB.a)*g_SunSpecular;
			}
		}
	}
	else
	{
		diffuse = max(-dot(normal, g_SunDirection), 0.0f)*g_SunDiffuse*diffuseFB;

		// Specular
		if(dot(g_SunDirection.xyz, normal)<0.0f)
		{
			float3 toEye = toEyeUnn/distFromEye;
			float3 reflection = normalize(g_SunDirection - 2*dot(g_SunDirection, normal)*normal);
			float specDot = max(dot(toEye, reflection), 0.0f);
			float specInt = pow(1.0f-diffuseFB.a*diffuseFB.a, 0.3f);
			float specCoef = pow(specDot, diffuseFB.a*255.0f)*specInt;
			specular = -specCoef*g_SunSpecular*dot(normal, g_SunDirection);
		}
	}

	float4 color = diffuse + ambient + specular;
	return float4(color.rgb, 1.0f);//*shadowMapColor;
}

// Techniques
technique10 RenderSun
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_4_0, RenderSunVS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_4_0, RenderSunPS()) );
	}
}
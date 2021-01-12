#include "LightPass.fx"
#include "CommonFunctions.fx"

/////////////////////////////////////////////////////////////////////////////////////////////
//
//							Point Light
//
/////////////////////////////////////////////////////////////////////////////////////////////

#define PL_FADE_DISTANCE 22.5831795813f

cbuffer PointLightBuffer
{
	float4		g_LightPosition;
	float4		g_LightDiffuse;
}

SamplerState bufSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

// Vertex shader
struct OutVS
{
	float4 position : SV_Position;
	float4 uv : POSITION0;
};

#define PERSP_RADIUS_COEF 2.4674011003f

OutVS PointLightVS(uint VertexId : SV_VertexID)
{
	OutVS output;

	float m = max(max(g_LightDiffuse.r, g_LightDiffuse.g), g_LightDiffuse.b);
	float4 rotPos = mul(g_LightPosition, g_ViewMatrix);
	float radius = sqrt(m)*PL_FADE_DISTANCE;
	float distEyeLight = CalculateNorm(rotPos);

	float4 position;
	if(rotPos.z > 0 && radius < distEyeLight) // On effectue un calcul des tangentes à la sphère de contrôle.
	{
		float perspRadius = distEyeLight/tan(acos(radius/distEyeLight));

		float4 relX = float4(normalize(cross(rotPos.xyz, float3(0, 1.0f, 0))), 0);
		float4 relY = float4(normalize(cross(rotPos.xyz, relX.xyz)), 0);

		relX *= perspRadius;
		relY *= perspRadius;

		if(VertexId == 0) { position = rotPos - relX + relY; }			// haut gauche
		else if(VertexId == 1) { position = rotPos + relX + relY; }		// haut droite
		else if(VertexId == 2) { position = rotPos - relX - relY; }		// bas gauche
		else { position = rotPos + relX - relY; }						// bas droite

		output.position = mul(position, g_ProjMatrix);
		output.position.z = output.position.w; // évite un non éclairement si le pixel se retrouve devant ou derrière le plan proche ou le plan lointain du frustum
		output.uv = output.position;

		return output;
	}
	else if((rotPos.z <= g_NearZ && (rotPos.z - g_NearZ + radius)  > 0) || (rotPos.z >= g_NearZ && radius >= distEyeLight)) // On calcule l'intersection entre la sphère de contrôle et le plan rapproché du frustum.
	{
		float d = g_NearZ - rotPos.z;
		float interRadius = sqrt(radius*radius - d*d);

		if(VertexId == 0) { output.position = float4(rotPos.x - interRadius, rotPos.y + interRadius, 1.0f, 1.0f); }			// haut gauche
		else if(VertexId == 1) { output.position = float4(rotPos.x + interRadius, rotPos.y + interRadius, 1.0f, 1.0f); }	// haut droite
		else if(VertexId == 2) { output.position = float4(rotPos.x - interRadius, rotPos.y - interRadius, 1.0f, 1.0f); }	// bas gauche
		else { output.position = float4(rotPos.x + interRadius, rotPos.y - interRadius, 1.0f, 1.0f); } 						// bas droite


		output.uv = output.position;

		return output;
	}
	else // rien à éclairer
	{
		output.position = float4(0, 0, 0, 1.0f);
		output.uv = float4(0, 0, 0, 1.0f);//float2(0, 0);
		return output;
	}
}


// Pixel shader
float4 PointLightPS(float4 Pos : SV_Position, float4 UV : POSITION0) : SV_Target
{
	float2 uv = float2(0.5f*(UV.x/UV.w) + 0.5f, -0.5f*(UV.y/UV.w) + 0.5f); 

	float xMultiply = g_CameraRatio*tan(g_CameraFovY/2.0f);
	float yMultiply = tan(g_CameraFovY/2.0f);

	float4 diffuseFB = DiffuseBuf.Sample(bufSampler, uv.xy); // diffuse from buffer
	float3 normalFB = NormalBuf.Sample(bufSampler, uv.xy).xyz;
	float z = DepthBuf.Sample(bufSampler, uv.xy);

	if(z == 1.0f) {
		return 0;
	}

	//
	//	Calcul des coordonnées globales du pixel
	//
	float4 clipPos;
	clipPos.x = UV.x/UV.w;//2.0f*uv.x - 1.0f;
	clipPos.y = UV.y/UV.w;//-2.0f*uv.y + 1.0f;
	clipPos.z = z;
	clipPos.w = 1.0f;

	float4 position = mul(clipPos, g_InvViewProjMatrix);
	position.w = 1.0f / position.w;
	position.xyz *= position.w;

	float3 normal = mul(float4(normalFB, 0.0f), g_InvViewMatrix).xyz;


	//
	//	Calcul des couleurs
	//

	// Specular
	float3 toEyeUnn = g_CameraPosition - position;
	float distFromEye = CalculateNorm(toEyeUnn);
	float4 specular = 0.0f;


	// Diffuse
	float3 lightToPixel = position.xyz-g_LightPosition.xyz;
	float distLightToPixel = CalculateNorm(lightToPixel);
	float3 rayDir = lightToPixel/distLightToPixel;
	float powerFactor = 1.0f/(distLightToPixel*distLightToPixel);
	float4 diffuse = max(-dot(normal, rayDir), 0.0f)*g_LightDiffuse*diffuseFB*powerFactor;


	// Specular
	if(dot(rayDir, normal)<0.0f)
	{
		float3 toEye = toEyeUnn/distFromEye;
		float3 reflection = normalize(rayDir - 2*dot(rayDir, normal)*normal);
		float specDot = dot(toEye, reflection);
		specular = ComputeSpecCoef(specDot, dot(normal, rayDir), diffuseFB.a)*g_LightDiffuse*powerFactor;
	}

	float4 color = diffuse + specular;
	return float4(color.r, color.g, color.b, 1.0f);
}

// Techniques
technique10 RenderPointLight
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_4_0, PointLightVS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_4_0, PointLightPS()) );
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////
//
//							Point Light : RenderShadowPointLight
//
/////////////////////////////////////////////////////////////////////////////////////////////

#define EPSILON 0.04f
#define SHADOW_MAP_EPSILON 0.03f
#define SHADOW_MAP_DISTANCE_CORRECTOR 0.03f
#define SHADOW_MAP_ANGLE_CORRECTOR 2.0f

Texture2DArray PointLightShadowMapsArray;

cbuffer ShadowMaps
{
	float4x4	g_ShadowViewProj[6];
	float		g_ShadowNearPlane;
	float		g_ShadowFarPlane;
}

SamplerState shadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	BorderColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

// Permet de récupérer les coordonnées du point du cube de textures vers lequel pointe la normale
// Les faces du cube sont supposées indicées comme suit:
// i = 0 : x = -1
// i = 1 : x = 1
// i = 2 : y = -1
// i = 3 : y = 1
// i = 4 : z = 1
// i = 5 : z = -1
//
// Les deux premiers composants donnent les coordonnées UV du point pointé.
// La composante z donne la profondeur dans le repère ayant pour centre le centre du cube et orientée vers la face pointée.
// La composante w donne l'indice de la face du cube pointée.
float4 GetTextureCubeCoordinates(float3 position)
{
	unsigned int i;
	float4 shadowMapPosition;
	for(i = 0; i < 6; i++)
	{
		shadowMapPosition = mul(float4(position.xyz, 1.0f), g_ShadowViewProj[i]);
		shadowMapPosition.xyz /= abs(shadowMapPosition.w);
		
		if(shadowMapPosition.x >= -1.0f && shadowMapPosition.x <= 1.0f
			&& shadowMapPosition.y >= -1.0f && shadowMapPosition.y <= 1.0f
			&& shadowMapPosition.z >= 0.0f)
		{
			shadowMapPosition.x = 0.5f*shadowMapPosition.x + 0.5f;
			shadowMapPosition.y = -0.5f*shadowMapPosition.y + 0.5f;
			shadowMapPosition.z = shadowMapPosition.w;
			shadowMapPosition.w = (float) i;

			return shadowMapPosition;
		}
	}
	/*shadowMapPosition = mul(float4(position.xyz, 1.0f), g_ShadowViewProj[i]);
	shadowMapPosition.xyz /= abs(shadowMapPosition.w);
	shadowMapPosition.x = 0.5f*shadowMapPosition.x + 0.5f;
	shadowMapPosition.y = -0.5f*shadowMapPosition.y + 0.5f;
	shadowMapPosition.z = shadowMapPosition.w;
	shadowMapPosition.w = i;*/

	shadowMapPosition = mul(float4(position.xyz, 1.0f), g_ShadowViewProj[i]);
	shadowMapPosition.xyz /= abs(shadowMapPosition.w);
	shadowMapPosition.x = 0.5f*shadowMapPosition.x + 0.5f;
	shadowMapPosition.y = -0.5f*shadowMapPosition.y + 0.5f;
	shadowMapPosition.z = shadowMapPosition.w;
	shadowMapPosition.w = i;

	return shadowMapPosition;
}


//
//	Rendu de la lampe avec shadow maps
//

// Pixel shader
float4 ShadowPointLightPS3(float4 Pos : SV_Position, float4 UV : POSITION0) : SV_Target
{
	float2 uv = float2(0.5f*(UV.x/UV.w) + 0.5f, -0.5f*(UV.y/UV.w) + 0.5f); 

	uv.x *= 3.0f;
	uv.y *= 2.0f;

	unsigned int x = uv.x;
	unsigned int y = uv.y;

	uv.x -= (float) x;
	uv.y -= (float) y;

	float sample = PointLightShadowMapsArray.Sample(shadowMapSampler, float3(uv, 3*y+x));
	return sample;
}

float4 ShadowPointLightPS(float4 Pos : SV_Position, float4 UV : POSITION0) : SV_Target
{
	float2 uv = float2(0.5f*(UV.x/UV.w) + 0.5f, -0.5f*(UV.y/UV.w) + 0.5f);

	float xMultiply = g_CameraRatio*tan(g_CameraFovY/2.0f);
	float yMultiply = tan(g_CameraFovY/2.0f);

	float4 diffuseFB = DiffuseBuf.Sample(bufSampler, uv.xy); // diffuse from buffer
	float3 normalFB = NormalBuf.Sample(bufSampler, uv.xy).xyz;
	float z = DepthBuf.Sample(bufSampler, uv.xy);

	if(z == 1.0f) {
		return 0;
	}

	//
	//	Calcul des coordonnées globales du pixel
	//
	float4 clipPos;
	clipPos.x = UV.x/UV.w;//2.0f*uv.x - 1.0f;
	clipPos.y = UV.y/UV.w;//-2.0f*uv.y + 1.0f;
	clipPos.z = z;
	clipPos.w = 1.0f;

	float4 position = mul(clipPos, g_InvViewProjMatrix);
	position.w = 1.0f / position.w;
	position.xyz *= position.w;

	float3 normal = mul(float4(normalFB, 0.0f), g_InvViewMatrix).xyz;


	//
	//	Calcul des couleurs
	//
	float3 relativePosition = position.xyz - g_LightPosition.xyz;
	float4 shadowMapPosition = GetTextureCubeCoordinates(position.xyz); /////////
	float depthFromFile = PointLightShadowMapsArray.Sample(shadowMapSampler, shadowMapPosition.xyw);
	float nearZ = g_ShadowNearPlane, farZ = g_ShadowFarPlane;
	float shadowMapDepth = nearZ*farZ/(farZ - (farZ-nearZ)*depthFromFile);

	float4 specular = 0.0f;
	float4 diffuse = 0.0f;

	// Specular
	float3 toEyeUnn = g_CameraPosition - position;
	float distFromEye = CalculateNorm(toEyeUnn);

	// Diffuse
	float3 lightToPixel = position.xyz-g_LightPosition.xyz;
	float distLightToPixel = CalculateNorm(lightToPixel);
	float3 rayDir = lightToPixel/distLightToPixel;

	float dotRayDirNormal = max(-dot(rayDir, normal), 0);
	float adaptativeEpsilon = (SHADOW_MAP_EPSILON+SHADOW_MAP_DISTANCE_CORRECTOR*distLightToPixel)/(dotRayDirNormal + (1.0f-dotRayDirNormal)*SHADOW_MAP_ANGLE_CORRECTOR);
	if(shadowMapPosition.z - adaptativeEpsilon < shadowMapDepth)
	{
		float powerFactor = 1.0f/(distLightToPixel*distLightToPixel);
		diffuse = max(-dot(normal, rayDir), 0.0f)*g_LightDiffuse*diffuseFB*powerFactor;


		// Specular
		if(dot(rayDir, normal)<0.0f)
		{
			float3 toEye = toEyeUnn/distFromEye;
			float3 reflection = normalize(rayDir - 2*dot(rayDir, normal)*normal);
			float specDot = dot(toEye, reflection);
			specular = ComputeSpecCoef(specDot, dot(normal, rayDir), diffuseFB.a)*g_LightDiffuse*powerFactor;
		}
	}

	//return depthFromFile;

	float4 color = diffuse + specular;
	return float4(color.rgb, 1.0f);
}

// Techniques
technique10 RenderShadowPointLight
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_4_0, PointLightVS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_4_0, ShadowPointLightPS()) );
	}
}


// Permet de récupérer les coordonnées du point du cube de textures vers lequel pointe la normale
// Les faces du cube sont supposées indicées comme suit:
// i = 0 : x = -1
// i = 1 : x = 1
// i = 2 : y = -1
// i = 3 : y = 1
// i = 4 : z = 1
// i = 5 : z = -1
//
// Les deux premiers composants donnent les coordonnées UV du point pointé.
// La composante z donne la profondeur dans le repère ayant pour centre le centre du cube et orientée vers la face pointée.
// La composante w donne l'indice de la face du cube pointée.
/*float4 GetTextureCubeCoordinates(float3 relativePosition)
{
	float Xi = 1.0f/abs(relativePosition.x);
	float2 xProj = float2(-Xi*relativePosition.z*sign(relativePosition.x), -Xi*relativePosition.y);
	if(relativePosition.x != 0 && abs(xProj.x) <= 1.0f && abs(xProj.y) <= 1.0f) // la relativePosition pointe vers le plan d'équation x=1 ou le plan d'équation x=-1
	{
		return float4(xProj.x/2.0f + 0.5f, xProj.y/2.0f + 0.5f, ((relativePosition.x > 0) ? float2(relativePosition.x, 1.0f) : float2(-relativePosition.x, 0.0f)));
	}

	float Yi = 1.0f/abs(relativePosition.y);
	float2 yProj = float2(Yi*relativePosition.x*sign(relativePosition.y), Yi*relativePosition.z);
	if(relativePosition.y != 0 && abs(yProj.x) <= 1.0f && abs(yProj.y) <= 1.0f)// la relativePosition pointe vers le plan d'équation y=1 ou le plan d'équation y=-1
	{
		 return float4(yProj.x/2.0f + 0.5f, yProj.y/2.0f + 0.5f, ((relativePosition.y > 0) ? float2(relativePosition.y, 3.0f) : float2(-relativePosition.y, 2.0f)));
	}

	 // la relativePosition pointe vers le plan d'équation z=1 ou le plan d'équation z=-1
	float Zi = 1.0f/abs(relativePosition.z);
	float2 zProj = float2(Zi*relativePosition.x*sign(relativePosition.z), -Zi*relativePosition.y);
	return float4(zProj.x/2.0f + 0.5f, zProj.y/2.0f + 0.5f, ((relativePosition.z > 0) ? float2(relativePosition.z, 5.0f) : float2(-relativePosition.z, 4.0f)));
}*/
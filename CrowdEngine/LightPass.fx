/// Effect pool utilisé pour la passe de lampes

shared cbuffer World
{
	float4x4	g_ViewMatrix;
	float4x4	g_ProjMatrix;
	float4x4	g_ViewProjMatrix;
	float4x4	g_InvViewMatrix;
	float4x4	g_InvProjMatrix;
	float4x4	g_InvViewProjMatrix;

	float4		g_CameraPosition;
	float4		g_CameraLookDir;
	float		g_CameraFovY;
	float		g_CameraRatio;
	float		g_NearZ;
	float		g_FarZ;
}

shared Texture2D DiffuseBuf;
shared Texture2D NormalBuf;
shared Texture2D DepthBuf;

shared cbuffer Shadow
{
	float4x4	g_ShadowViewProjMatrix;
}
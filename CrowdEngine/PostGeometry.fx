cbuffer PostGeometryBuffer
{
	float4x4 g_ViewMatrix;
	float4x4 g_ProjMatrix;

	Texture2D DiffuseBuf;
	Texture2D NormalBuf;
	Texture2D DepthBuf;
}
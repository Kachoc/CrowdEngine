#ifndef CRWD_D3D10_LIGHTSCENE_H_INCLUDED
#define CRWD_D3D10_LIGHTSCENE_H_INCLUDED

#include "D3DEntry.h"

#include "D3D10ShaderFile.h"

#include "LightScene.h"
#include "D3D10Passes.h"

namespace Crowd
{

namespace D3D10
{

class ShaderFile;

class LightScene : public Crowd::LightScene
{
	public:
	typedef SmartPtr<SunPass, IPol> ptr;

	LightScene(const AABB &boundingBox);
	virtual ~LightScene();

	static Status Load(ShaderFile::ptr worldShader);
	static Status Unload();

	void SetWorldShaderInfo(const WorldShaderInfo &info);
	
	Status CreateSunShadowMaps(unsigned int resolution, unsigned int numShadowMaps, const std::vector<float> &epsilons, const unsigned int *shadowScenesIndices = NULL);

	void RenderSunShadowMaps(const RenderInfo &info, const std::vector<OrthographicCamera> &cameras);
	void RenderSun(const RenderInfo &info, const Camera &camera);

	protected:
	struct SunShadowMapsInfo
	{
		Matrix					ShadowMapViewProjMatrices[16];
		unsigned int			NumShadowMapsLevels;
		float					Epsilon;
		float					unused1, unused2;//, unused3;
	};

	protected:
	static ShaderFile::ptr							ms_lightPass;

	static ShaderFile::ptr							ms_renderSun;
		
	static ShaderFile::ptr							ms_shadowMap;

	static ShaderFile::ptr							ms_terrainShadow;

	//
	//	World
	//
	EffectConstantBufferPtr					ms_world;
	BufferPtr								m_worldBuffer;

	EffectShaderResourceVariablePtr			ms_normalBufferView;
	EffectShaderResourceVariablePtr			ms_diffuseBufferView;
	EffectShaderResourceVariablePtr			ms_depthBufferView;


	//
	//	Sun
	//
	EffectTechniquePtr								ms_renderSunTech;

	EffectConstantBufferPtr							ms_sun;
	BufferPtr										ms_sunBuffer;


	//
	// Shadow map
	//
	Texture2DPtr							m_shadowMapArray;
	ShaderResourceViewPtr					ms_shadowMapArrayResView;
	EffectShaderResourceVariablePtr			ms_shadowMapArrayResVar;

	Texture2DPtr							ms_shadowDepthBuffer;
	DepthStencilViewPtr						ms_shadowDepthBufferView;

	EffectConstantBufferPtr					ms_shadowMapsConstantBuffer; // lien vers le constant buffer de stockage des matrices de projection
	BufferPtr								ms_shadowMapsBuffer; // buffer de stockage des matrices de projection

	EffectConstantBufferPtr					ms_shadowConstantBuffer;
	BufferPtr								ms_shadowBuffer;

	std::vector<OrthographicCamera>			m_sunCameras;



};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_LIGHTSCENE_H_INCLUDED
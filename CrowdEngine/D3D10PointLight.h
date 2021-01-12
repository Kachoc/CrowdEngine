#ifndef CROWD_D3D10_POINTLIGHT_H_INCLUDED
#define CROWD_D3D10_POINTLIGHT_H_INCLUDED

#include "D3DEntry.h"

#include "D3D10LightScene.h"

namespace Crowd
{

namespace D3D10
{

class PointLight : public Crowd::PointLight
{
	public:
	typedef SmartPtr<PointLight, IPol> ptr;

	struct PointLightData
	{
		Vector4		Position;
		Color		DiffuseSpec; // Définit la couleur émise par la lampe. L'intensité reçue par un objet est exactement égale à color à la distance de 1 unité de distance.
	};

	struct ShadowPointLightData
	{
		Matrix	ShadowViewProjMatrices[6];
		float	ShadowNearPlane;
		float	ShadowFarPlane;
		float	unused1, unused2;
	};

	PointLight(const Vector4 &position, const Color &diffuseSpec);

	virtual void Draw(const RenderInfo &info, const Camera &camera);
	virtual Status UseShadowMaps(unsigned int resolution, float projMinDist, float projMaxDist, unsigned int shadowMapSceneIndice, bool use);
	virtual void ComputeShadowMaps(const RenderInfo &info, const Camera &camera, std::vector<GeometryScene::ptr> &scene);

	static Status Load(ShaderFile::ptr lightPass, ShaderFile::ptr shadowMapFile);
	static Status Unload();

	protected:
	static ShaderFile::ptr		m_pointLightShaderFile;
	static ShaderFile::ptr		m_shadowMapShaderFile;

	EffectTechnique				*m_renderPointLight;
	EffectTechnique				*m_renderShadowPointLight;
	BlendStatePtr				m_blendState;
	RasterizerStatePtr			m_rasterizerState;

	ConstantBuffer::ptr			m_lightCBuffer;
	ConstantBuffer::ptr			m_shadowViewProjBuffer;

	//
	//	Shadow maps
	//
	ConstantBuffer::ptr						m_shadowMapsConstantBuffer[6];

	Texture2DPtr							m_shadowMapsArray;
	ShaderResourceViewPtr					m_shadowMapsArrayResView;
	EffectShaderResourceVariablePtr			m_shadowMapsArrayResVar;

	Texture2DPtr							m_shadowDepthBuffer;
	DepthStencilViewPtr						m_shadowDepthBufferView;

	//Texture2DPtr							m_shadowMapRenderTarget;
	//RenderTargetViewPtr					m_shadowMapRenderTargetView;
};

} // namespace Crowd

} // namespace D3D10

#endif // CROWD_D3D10_POINTLIGHT_H_INCLUDED
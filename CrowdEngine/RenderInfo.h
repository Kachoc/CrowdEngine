#ifndef CRWD_RENDER_INFO_H_INCLUDED
#define CRWD_RENDER_INFO_H_INCLUDED

#include <vector>
#include "D3DEntry.h"
#include "Camera.h"

namespace Crowd
{

class PerfAnalyser
{
	public:
	PerfAnalyser();
	virtual ~PerfAnalyser();

	void NewFrame(); /// A appeler lorsque l'on commence le rendu d'une nouvelle frame.

	void AddVertices(unsigned int numVertices);
	unsigned int GetVerticesCount() const;

	protected:
	unsigned int verticesCount;
};

struct RenderInfo
{
	public:
	RenderInfo() : depthView(NULL), diffuseBufferView(NULL), normalBufferView(NULL), diffuseResView(NULL), normalResView(NULL),
					depthResView(NULL), lightPassTargetResView(NULL) {}
	virtual ~RenderInfo() {}

	unsigned int							targetWidth, targetHeight;
	Texture2D								*backBuffer;
	RenderTargetView						*backBufferView;
	RenderTargetView						*diffuseBufferView; // color buffer cible du rendu diff�r�
	RenderTargetView						*normalBufferView; // normal buffer cible du rendu diff�r�
	DepthStencilView						*depthView;
	ShaderResourceView						*diffuseResView; // copie du color buffer, � utiliser comme ressource de shader
	ShaderResourceView						*normalResView; // copie du normal buffer, � utiliser comme ressource de shader
	ShaderResourceView						*depthResView; // copie du depth/stencil buffer, � utiliser comme ressource de shader
	Texture2D								*lightPassTarget;
	ShaderResourceView						*lightPassTargetResView; // texture obtenue apr�s le rendu des lampes, � utiliser comme ressource de shader
};

} // namespace Crowd

#endif // CRWD_RENDER_INFO_H_INCLUDED
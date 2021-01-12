#include "Core.h"
#include "D3DEntry.h"

namespace Crowd
{

namespace D3D10
{

DevicePtr		g_d3dDevice;

void InitializeCrowdEngine()
{
	if(g_d3dDevice) return;

	#if defined(_DEBUG) | defined(DEBUG)
	HRESULT hr = D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, g_d3dDevice.GetPtrP());
	#else
	HRESULT hr = D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, g_d3dDevice.GetPtrP());
	#endif

	if(FAILED(hr)) {
		throw Exception(String("D3D10::Renderer::Init : ") + GetD3DXErrorString(hr));
	}
}

} // namespace D3D10

void InitializeCrowdEngine()
{
	D3D10::InitializeCrowdEngine();
}

} // namespace Crowd
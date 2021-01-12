//
//	D3D10Formats.h
//
// Définit les formats de ressources utilisés par le moteur, et les fonctions permettant de les convertir.

#ifndef CRWD_D3D10_FORMATS_H_INCLUDED
#define CRWD_D3D10_FORMATS_H_INCLUDED

#include "Formats.h"
#include "D3DEntry.h"

namespace Crowd
{

namespace D3D10
{

template<FORMAT format> struct Format{ static DXGI_FORMAT format; }; // permet de récupérer l'équivalent Direct3D ou autre...

//template<>  DXGI_FORMAT Format<>::format = ;
template<> DXGI_FORMAT Format<FORMAT_R32G32B32A32_FLOAT>::format		= DXGI_FORMAT_R32G32B32A32_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R32G32B32A32_UINT>::format			= DXGI_FORMAT_R32G32B32A32_UINT;
template<> DXGI_FORMAT Format<FORMAT_R32G32B32A32_SINT>::format			= DXGI_FORMAT_R32G32B32A32_SINT;
template<> DXGI_FORMAT Format<FORMAT_R32G32B32_FLOAT>::format			= DXGI_FORMAT_R32G32B32_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R32G32B32_UINT>::format			= DXGI_FORMAT_R32G32B32_UINT;
template<> DXGI_FORMAT Format<FORMAT_R32G32B32_SINT>::format			= DXGI_FORMAT_R32G32B32_SINT;
template<> DXGI_FORMAT Format<FORMAT_R16G16B16A16_FLOAT>::format		= DXGI_FORMAT_R16G16B16A16_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R16G16B16A16_UNORM>::format		= DXGI_FORMAT_R16G16B16A16_UNORM;
template<> DXGI_FORMAT Format<FORMAT_R16G16B16A16_SNORM>::format		= DXGI_FORMAT_R16G16B16A16_SNORM;
template<> DXGI_FORMAT Format<FORMAT_R16G16B16A16_UINT>::format			= DXGI_FORMAT_R16G16B16A16_UINT;
template<> DXGI_FORMAT Format<FORMAT_R16G16B16A16_SINT>::format			= DXGI_FORMAT_R16G16B16A16_SINT;
template<> DXGI_FORMAT Format<FORMAT_R32G32_FLOAT>::format				= DXGI_FORMAT_R32G32_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R32G32_UINT>::format				= DXGI_FORMAT_R32G32_UINT;
template<> DXGI_FORMAT Format<FORMAT_R32G32_SINT>::format				= DXGI_FORMAT_R32G32_SINT;

template<> DXGI_FORMAT Format<FORMAT_R16G16_FLOAT>::format				= DXGI_FORMAT_R16G16_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R16G16_UNORM>::format				= DXGI_FORMAT_R16G16_UNORM;
template<> DXGI_FORMAT Format<FORMAT_R16G16_SNORM>::format				= DXGI_FORMAT_R16G16_SNORM;

template<> DXGI_FORMAT Format<FORMAT_R8G8B8A8_UNORM>::format			= DXGI_FORMAT_R8G8B8A8_UNORM;
template<> DXGI_FORMAT Format<FORMAT_R8G8B8A8_SNORM>::format			= DXGI_FORMAT_R8G8B8A8_SNORM;
template<> DXGI_FORMAT Format<FORMAT_R8G8B8A8_UINT>::format				= DXGI_FORMAT_R8G8B8A8_UINT;
template<> DXGI_FORMAT Format<FORMAT_R8G8B8A8_SINT>::format				= DXGI_FORMAT_R8G8B8A8_SINT;
template<> DXGI_FORMAT Format<FORMAT_D32_FLOAT>::format					= DXGI_FORMAT_D32_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R32_FLOAT>::format					= DXGI_FORMAT_R32_FLOAT;
template<> DXGI_FORMAT Format<FORMAT_R32_UINT>::format					= DXGI_FORMAT_R32_UINT;
template<> DXGI_FORMAT Format<FORMAT_R32_SINT>::format					= DXGI_FORMAT_R32_SINT;
template<> DXGI_FORMAT Format<FORMAT_R16_UNORM>::format					= DXGI_FORMAT_R16_UNORM;
template<> DXGI_FORMAT Format<FORMAT_R16_SNORM>::format					= DXGI_FORMAT_R16_SNORM;
template<> DXGI_FORMAT Format<FORMAT_R16_UINT>::format					= DXGI_FORMAT_R16_UINT;
template<> DXGI_FORMAT Format<FORMAT_R16_SINT>::format					= DXGI_FORMAT_R16_SINT;
template<> DXGI_FORMAT Format<FORMAT_R8_UNORM>::format					= DXGI_FORMAT_R8_UNORM;
template<> DXGI_FORMAT Format<FORMAT_R8_SNORM>::format					= DXGI_FORMAT_R8_SNORM;
template<> DXGI_FORMAT Format<FORMAT_R8_UINT>::format					= DXGI_FORMAT_R8_UINT;
template<> DXGI_FORMAT Format<FORMAT_R8_SINT>::format					= DXGI_FORMAT_R8_SINT;
template<> DXGI_FORMAT Format<FORMAT_D24S8>::format						= DXGI_FORMAT_D24_UNORM_S8_UINT;
template<> DXGI_FORMAT Format<FORMAT_D32_FLOAT_S8X24>::format			= DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
template<> DXGI_FORMAT Format<FORMAT_BC8>::format						= DXGI_FORMAT_BC1_UNORM;
template<> DXGI_FORMAT Format<FORMAT_BC16_A4>::format					= DXGI_FORMAT_BC2_UNORM;
template<> DXGI_FORMAT Format<FORMAT_BC16_A8>::format					= DXGI_FORMAT_BC3_UNORM;
template<FORMAT format> DXGI_FORMAT Format<format>::format(0); // par défaut

// permet de récupérer l'équivalent Direct3D ou inversement de façon dynamique
DXGI_FORMAT ConvertFormat(FORMAT format);
FORMAT ConvertFormat(DXGI_FORMAT format);

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_FORMATS_H_INCLUDED
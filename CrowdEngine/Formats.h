//
//	Formats.h
//
// Déclare les formats de ressources utilisés par le moteur.

#ifndef CRWD_FORMATS_H_INCLUDED
#define CRWD_FORMATS_H_INCLUDED

namespace Crowd
{

//
//	FORMAT
//
enum FORMAT{
		FORMAT_UNKNOWN,
		FORMAT_R32G32B32A32_FLOAT,
		FORMAT_R32G32B32A32_UINT, //
		FORMAT_R32G32B32A32_SINT, //
		FORMAT_R32G32B32_FLOAT,
		FORMAT_R32G32B32_UINT, //
		FORMAT_R32G32B32_SINT, //
		FORMAT_R16G16B16A16_FLOAT,
		FORMAT_R16G16B16A16_UNORM, ///
		FORMAT_R16G16B16A16_SNORM, ///
		FORMAT_R16G16B16A16_UINT, ///
		FORMAT_R16G16B16A16_SINT, ///
		FORMAT_R32G32_FLOAT,
		FORMAT_R32G32_UINT, //
		FORMAT_R32G32_SINT, //

		FORMAT_R16G16_FLOAT,
		FORMAT_R16G16_UNORM,
		FORMAT_R16G16_SNORM,

		FORMAT_R8G8B8A8_UNORM,
		FORMAT_R8G8B8A8_SNORM,
		FORMAT_R8G8B8A8_UINT, ///
		FORMAT_R8G8B8A8_SINT, ///
		FORMAT_D32_FLOAT, // depth/stencil buffer
		FORMAT_R32_FLOAT,
		FORMAT_R32_UINT, //
		FORMAT_R32_SINT, //
		FORMAT_R16_UNORM,
		FORMAT_R16_SNORM,
		FORMAT_R16_UINT, ///
		FORMAT_R16_SINT, ///
		FORMAT_R8_SNORM,
		FORMAT_R8_UNORM,
		FORMAT_R8_UINT, ///
		FORMAT_R8_SINT, ///
		FORMAT_D24S8,
		FORMAT_D32_FLOAT_S8X24,
		FORMAT_BC8, // block-compressed: rgb + alpha (0 ou 1 bit) -> 8 bytes pour un bloc de 4x4 pixels
		FORMAT_BC16_A4, // block-compressed: rgb + alpha (4 bit) -> 16 bytes pour un bloc de 4x4 pixels
		FORMAT_BC16_A8, // block-compressed: rgb + alpha (8 bit) -> 16 bytes pour un bloc de 4x4 pixels
		};

} // namespace Crowd

#endif // CRWD_FORMATS_H_INCLUDED
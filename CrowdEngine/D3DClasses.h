//
//	D3DClasses.h 
//
// Déclare les classes utilisées par les fichiers du coeur du moteur et pas forcément utilisables dans le reste du moteur
// (notamment en ce qui concerne les classes de Direct3D).
// Inclut les fichiers de Direct3D nécessaires au moteur pour la génération des fichiers .dll et .lib utilisés par le moteur,
// déclare les classes "extérieures" du moteur en cas d'utilisation du moteur pour un autre projet.

#ifndef CRWD_D3D_CLASSES_H_INCLUDED
#define CRWD_D3D_CLASSES_H_INCLUDED

#include "SmartPtr.h"

#ifdef CRWD_DIRECT3D
#include "D3DEntry.h"
#endif

namespace Crowd
{

#ifdef CRWD_DIRECT3D

//
//	Déclaration des structures de Direct3D utilisées par le moteur
//
typedef ID3D10Device							Device;
typedef IDXGIAdapter							Adapter;//
typedef IDXGISwapChain							SwapChain;

typedef ID3D10Texture2D							Texture2D;
typedef ID3D10Buffer							Buffer;

typedef ID3D10RenderTargetView					RenderTargetView;
typedef ID3D10DepthStencilView					DepthStencilView;
typedef ID3D10ShaderResourceView				ShaderResourceView;

typedef D3D10_INPUT_ELEMENT_DESC				INPUT_ELEMENT;
typedef ID3D10InputLayout						InputLayout;

typedef ID3D10RasterizerState					RasterizerState;
typedef ID3D10BlendState						BlendState;

typedef ID3D10Effect							Effect;
typedef ID3D10EffectPool						EffectPool;
typedef ID3D10Blob								UndefBuffer;

typedef ID3D10EffectMatrixVariable				EffectMatrixVariable;
typedef ID3D10EffectScalarVariable				EffectScalarVariable;
typedef ID3D10EffectShaderVariable				EffectShaderVariable;
typedef ID3D10EffectVectorVariable				EffectVectorVariable;
typedef ID3D10EffectTechnique					EffectTechnique;
typedef ID3D10EffectShaderResourceVariable		EffectShaderResourceVariable;

typedef ID3D10EffectConstantBuffer				EffectConstantBuffer;

typedef ID3D10InputLayout						InputLayout;

typedef IDirectInput8							DirectInput;
typedef IDirectInputDevice8						MouseDevice;
typedef IDirectInputDevice8						KeyboardDevice;

typedef ID3DX10Font								DFont;


//
//	Déclaration des pointeurs intelligents des classes de Direct3D utilisées par le moteur
//
typedef SmartPtr<Device, IPol>						DevicePtr;
typedef SmartPtr<Adapter, IPol>						AdapterPtr;
//typedef SmartPtr<RenderDevice, IPol>				RenderDevicePtr;
typedef SmartPtr<SwapChain, IPol>					SwapChainPtr;

typedef SmartPtr<Texture2D, IPol>					Texture2DPtr;
typedef SmartPtr<Buffer, IPol>						BufferPtr;

typedef SmartPtr<RenderTargetView, IPol>			RenderTargetViewPtr;
typedef SmartPtr<DepthStencilView, IPol>			DepthStencilViewPtr;
typedef SmartPtr<ShaderResourceView, IPol>			ShaderResourceViewPtr;

typedef SmartPtr<InputLayout, IPol>					InputLayoutPtr;

typedef SmartPtr<RasterizerState, IPol>				RasterizerStatePtr;
typedef SmartPtr<BlendState, IPol>					BlendStatePtr;

typedef SmartPtr<Effect, IPol>						EffectPtr;
typedef SmartPtr<EffectPool, IPol>					EffectPoolPtr;
typedef SmartPtr<UndefBuffer, IPol>					UndefBufferPtr;

typedef EffectMatrixVariable*						EffectMatrixVariablePtr;
typedef EffectScalarVariable*						EffectScalarVariablePtr;
typedef EffectShaderVariable*						EffectShaderVariablePtr;
typedef EffectVectorVariable*						EffectVectorVariablePtr;
typedef EffectTechnique*							EffectTechniquePtr;
typedef EffectShaderResourceVariable*				EffectShaderResourceVariablePtr;

typedef EffectConstantBuffer*						EffectConstantBufferPtr;

typedef SmartPtr<InputLayout, IPol>					InputLayoutPtr;

typedef SmartPtr<IDirectInput8, IPol>				DirectInputPtr;
typedef SmartPtr<IDirectInputDevice8, IPol>			MouseDevicePtr;
typedef SmartPtr<IDirectInputDevice8, IPol>			KeyboardDevicePtr;

typedef SmartPtr<DFont, IPol>						DFontPtr;

#else // CRWD_DIRECT3D

//
//	Déclaration des structures héritées de l'API utilisée et nécessaires au reste du moteur
//
struct Device;
struct RenderDevice;
struct SwapChain;

struct Texture2D;
struct Buffer;

struct RenderTargetView;
struct DepthStencilView;
struct ShaderResourceView;

struct INPUT_ELEMENT;
struct InputLayout;

struct Effect;
struct EffectPtr;

#endif // CRWD_DIRECT3D

} // namespace Crowd

#endif // CRWD_D3D_CLASSES_H_INCLUDED
#include "D3D10Textures.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

//
//	Texture
//

Texture2D *Texture::GetTexture() const
{
	return m_texture;
}

ShaderResourceView *Texture::GetShaderResourceView() const
{
	return m_textureResView;
}

Texture::Texture(Texture2DPtr texture, ShaderResourceViewPtr textureResView, const Info &info)
{
	m_texture = texture;
	m_textureResView = textureResView;
	m_textureInfo = info;
}

Texture::~Texture()
{

}

void *Texture::Map(unsigned int &rowPitch)
{
	D3D10_MAP mapType;
	if( (m_textureInfo.AccessType & Texture::CPU_READ) && (m_textureInfo.AccessType & Texture::CPU_WRITE)) {
		mapType = D3D10_MAP_READ_WRITE;
	}
	else if(m_textureInfo.AccessType & Texture::CPU_READ) {
		mapType = D3D10_MAP_READ;
	}
	else if(m_textureInfo.AccessType & Texture::CPU_WRITE) {
		mapType = D3D10_MAP_WRITE_DISCARD;
	}
	else {
		return NULL;
	}
	D3D10_MAPPED_TEXTURE2D mappedTexture;
	HRESULT hr = m_texture->Map(D3D10CalcSubresource(0, 0, 1), mapType, 0, &mappedTexture);
	if(FAILED(hr)) {
		return NULL;
	}

	rowPitch = mappedTexture.RowPitch;
	return mappedTexture.pData;
}

void Texture::Unmap()
{
	m_texture->Unmap(D3D10CalcSubresource(0, 0, 1));
}


//
//	TextureLoader
//
Texture *Texture::Load(const String &fileName, int AccessType, unsigned int Width, unsigned int Height, unsigned int NumMipLevels, bool GreyScale, Status *status)
{

	D3DX10_IMAGE_LOAD_INFO d3dLoadInfo;
	ZeroMemory(&d3dLoadInfo, sizeof(d3dLoadInfo));

	d3dLoadInfo.Width = Width;
	d3dLoadInfo.Height = Height;
	d3dLoadInfo.Depth = 0;
	d3dLoadInfo.FirstMipLevel = 0;
	d3dLoadInfo.MipLevels = NumMipLevels;

	if( (AccessType & Crowd::Texture::CPU_READ) && (AccessType & Crowd::Texture::CPU_WRITE)) {
		d3dLoadInfo.Usage = D3D10_USAGE_STAGING;
		d3dLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		d3dLoadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
	}
	else if(AccessType & Crowd::Texture::CPU_READ) {
		d3dLoadInfo.Usage = D3D10_USAGE_STAGING;
		d3dLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		d3dLoadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_READ;
	}
	else if(AccessType & Crowd::Texture::CPU_WRITE) {
		d3dLoadInfo.Usage = D3D10_USAGE_DYNAMIC;
		d3dLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		d3dLoadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}
	else if(AccessType & Crowd::Texture::IMMUTABLE) {
		d3dLoadInfo.Usage = D3D10_USAGE_IMMUTABLE;
		d3dLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		d3dLoadInfo.CpuAccessFlags = 0;
	}
	else {
		d3dLoadInfo.Usage = D3D10_USAGE_DEFAULT;
		d3dLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		d3dLoadInfo.CpuAccessFlags = 0;
	}

	if(GreyScale) {
		d3dLoadInfo.Format = DXGI_FORMAT_R32_FLOAT;
	}
	else {
		d3dLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	d3dLoadInfo.Filter = D3DX10_FILTER_TRIANGLE; 
	d3dLoadInfo.MipFilter = D3DX10_FILTER_BOX;

	Texture2DPtr texture;
	HRESULT hr = D3DX10CreateTextureFromFile(g_d3dDevice, fileName.asChar(), &d3dLoadInfo, NULL, (ID3D10Resource**) texture.GetPtrP(), NULL);
	if(FAILED(hr)) {
		if(status) {
			*status = Status("TextureLoader::Load : " + fileName + " " + GetD3DXErrorString(hr));
		}
		return NULL;
	}

	Crowd::Texture::Info info;
	info.AccessType = AccessType;
	info.NumMipLevels = NumMipLevels;

	D3D10_TEXTURE2D_DESC texDesc;
	texture->GetDesc(&texDesc);
	info.Width = texDesc.Width;
	info.Height = texDesc.Height;
	info.GreyScale = (texDesc.Format == DXGI_FORMAT_R32_FLOAT) || (texDesc.Format == DXGI_FORMAT_R16_FLOAT);

	ShaderResourceViewPtr resView;
	if(d3dLoadInfo.BindFlags & D3D10_BIND_SHADER_RESOURCE) {
		hr = g_d3dDevice->CreateShaderResourceView(texture, NULL, resView.GetPtrP());
		if(FAILED(hr)) {
			if(status) {
				*status = Status("Texture::Load : " + fileName + " " + GetD3DXErrorString(hr));
			}
			return NULL;
		}
	}

	return new Texture(texture, resView, info);
}


//
//	TextureSaver
//
Status Texture::Save(const String &fileName, Crowd::Texture *texture, const String &fileFormat)
{
	D3DX10_IMAGE_FILE_FORMAT dxFormat;
	if(fileFormat == "jpg")
	{
		dxFormat = D3DX10_IFF_JPG;
	}
	else if(fileFormat == "png")
	{
		dxFormat = D3DX10_IFF_PNG;
	}
	else if(fileFormat == "bmp")
	{
		dxFormat = D3DX10_IFF_BMP;
	}
	else
	{
		return Status("Texture::Save : le format " + fileFormat + " est incorrect.");
	}

	Texture *ctexture = (Texture*) texture;
	HRESULT hr = D3DX10SaveTextureToFile(ctexture->GetTexture(), dxFormat, fileName.asChar());
	if(FAILED(hr)) {
		return Status("Texture::Save : " + fileName + " " + GetD3DXErrorString(hr));
	}
	return Status::Success;
}

} // namespace D3D10

} // namespace Crowd
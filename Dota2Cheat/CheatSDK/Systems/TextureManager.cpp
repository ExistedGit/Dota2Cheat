#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <array>

void TextureManager::InitDX11Texture(int image_width, int image_height, unsigned char* image_data, ID3D11ShaderResourceView** out_srv) {
	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	DrawData.Dx.pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	DrawData.Dx.pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();
}

bool TextureManager::LoadTextureFromMemory(unsigned char* data, size_t size, ID3D11ShaderResourceView** tex) {
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory(data, size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	InitDX11Texture(image_width, image_height, image_data, tex);
	stbi_image_free(image_data);
	return true;
}

bool TextureManager::LoadTextureFromFile(std::string_view filename, ID3D11ShaderResourceView** tex)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename.data(), &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	InitDX11Texture(image_width, image_height, image_data, tex);
	stbi_image_free(image_data);
	return true;
}


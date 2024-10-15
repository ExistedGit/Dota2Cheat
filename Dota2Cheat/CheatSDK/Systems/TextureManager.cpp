#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <array>
#include "../Data/DrawData.h"

void TextureManager::InitTexture(unsigned image_width, unsigned image_height, const unsigned char* image_data, ID3D11ShaderResourceView** out_srv) const {
	// Create texture
	D3D11_TEXTURE2D_DESC desc{
		.Width = image_width,
		.Height = image_height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.SampleDesc {
			.Count = 1
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_SHADER_RESOURCE,
		.CPUAccessFlags = 0,
		.MiscFlags = 0
	};

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource{
		.pSysMem = image_data,
		.SysMemPitch = desc.Width * 4,
		.SysMemSlicePitch = 0
	};

	DrawData.Dx.pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D {
			.MostDetailedMip = 0,
			.MipLevels = desc.MipLevels
		}
	};
	DrawData.Dx.pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();
}

bool TextureManager::LoadTextureFromMemory(unsigned char* data, size_t size, ID3D11ShaderResourceView** tex) {
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory(data, size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	InitTexture(image_width, image_height, image_data, tex);
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

	InitTexture(image_width, image_height, image_data, tex);
	stbi_image_free(image_data);
	return true;
}


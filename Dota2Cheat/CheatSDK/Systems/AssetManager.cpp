#include "AssetManager.h"
#include "../VTexDecoders/VTexParser.h"
#include "TextureManager.h"

#include <Base/MemAlloc.h>

ID3D11ShaderResourceView* CAssetManager::VTexDir::Load(const std::string& file, std::string postfix) {
	auto& ret = cache[file];

	if (!ret) {
		auto path = dir + "/" + file + "_" + postfix + ".vtex_c";
		auto data = VTexParser::Load(path);

		if (!data.data) return nullptr;

		texManager.InitTexture(data.w, data.h, data.data, &ret);
		CMemAlloc::Get()->Free(data.data);
	}

	return ret;
}

void CAssetManager::VTexDir::LoadDeferred(ID3D11ShaderResourceView** dest, const std::string& file, std::string postfix) {
	auto& ret = cache[file];

	if (ret) {
		*dest = ret;
		return;
	}

	auto path = dir + "/" + file + "_" + postfix + ".vtex_c";
	auto data = VTexParser::Load(path);

	if (!data.data) return;

	texManager.InitTextureDeferred(data.w, data.h, data.data, &ret, [](auto& data) { CMemAlloc::Get()->Free(data.data); });
}

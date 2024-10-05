#include "AssetManager.h"
#include "../VTexDecoders/VTexParser.h"
#include "TextureManager.h"

#include <Base/MemAlloc.h>

ID3D11ShaderResourceView* CAssetManager::VTexDir::Load(const std::string& file, std::string postfix) {
	auto& ret = cache[file];

	if (!ret) {
		auto path = prefix + dir + "/" + file + "_" + postfix + ".vtex_c";
		auto data = VTexParser::Load(path);
		
		if (!data.data) return nullptr;

		texManager.InitDX11Texture(data.w, data.h, data.data, &ret);
		CMemAlloc::Get()->Free(data.data);
	}

	return ret;
}

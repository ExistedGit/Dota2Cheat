#include "AssetManager.h"
#include "../VTexDecoders/VTexParser.h"
#include "TextureManager.h"

#include <Base/MemAlloc.h>

ID3D11ShaderResourceView* CAssetManager::VTexDir::Load(const std::string& file, std::string postfix) {
	auto& ret = cache[file];

	if (!ret) {
		auto data = VTexParser::Load(prefix + dir + "/" + file + "_" + postfix + ".vtex_c");
		texManager.InitDX11Texture(data.w, data.h, data.data, &ret);
		CMemAlloc::Get()->Free(data.data);
	}

	return ret;
}

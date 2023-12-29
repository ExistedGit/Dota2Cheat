#pragma once
#include <string>
#include "../VTexDecoders/VTexParser.h"
#include "TextureManager.h"

// Operates in VPK directory panorama/images
inline class CAssetManager {
public:
	const static inline std::string prefix = "panorama/images/";

	struct VTexDir {
		std::string dir;
		std::map<std::string, ID3D11ShaderResourceView*> items;

		VTexDir(std::string dir) :dir(dir) {}

		ID3D11ShaderResourceView* Load(std::string file, std::string postfix = "png") {
			auto& ret = items[file];

			if (!ret) {
				auto data = VTexParser::Load(prefix + dir + "/" + file + "_" + postfix + ".vtex_c");
				texManager.InitDX11Texture(data.w, data.h, data.data, &ret);
				CMemAlloc::Instance()->Free(data.data);
			}

			return ret;
		}
	};

	VTexDir
		heroIcons{ "heroes/icons" },
		spellIcons{ "spellicons" },
		items{ "items" };

	// For uncommon uses
	VTexDir Directory(const std::string& dir) {
		return VTexDir(dir);
	}

} assets;
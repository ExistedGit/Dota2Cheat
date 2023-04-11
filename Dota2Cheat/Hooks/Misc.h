#pragma once
#include "../SDK/pch.h"
#include "../Config.h"

namespace Hooks {
	inline Signatures::BIsEmoticonUnlockedFn oBIsEmoticonUnlocked{};

	inline bool hkBIsEmoticonUnlocked(void* thisptr, uint32_t unk) {
		return Config::Changer::UnlockEmoticons || oBIsEmoticonUnlocked(thisptr, unk);
	}
	inline Signatures::SaveSerializedSOCacheFn oSaveSerializedSOCache{};
	inline void* hkSaveSerializedSOCache(void* thisptr) {
		auto result = oSaveSerializedSOCache(thisptr);
		Modules::SkinChanger.DeleteSOCacheFiles();
		{
			std::ofstream fout(ctx.cheatFolderPath + "\\config\\base.json");
			Config::cfg.SaveConfig(fout);
			fout.close();
		}
		{
			std::ofstream fout(ctx.cheatFolderPath + "\\config\\inventory.json");
			Config::cfg.SaveEquippedItems(fout);
			fout.close();
		}
		return result;
	}
}
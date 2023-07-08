#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Config.h"
#include "../CheatSDK/Data/DrawData.h"

namespace Hooks {
	inline Signatures::SaveSerializedSOCacheFn oSaveSerializedSOCache{};
	inline void* hkSaveSerializedSOCache(void* thisptr) {
		auto result = oSaveSerializedSOCache(thisptr);
		Modules::SkinChanger.DeleteSOCacheFiles();
		{
			std::ofstream fout(d2c.cheatFolderPath + "\\config\\base.json");
			Config::cfg.SaveConfig(fout);
			fout.close();
		}
		{
			std::ofstream fout(d2c.cheatFolderPath + "\\config\\inventory.json");
			Config::cfg.SaveEquippedItems(fout);
			fout.close();
		}
		return result;
	}
}
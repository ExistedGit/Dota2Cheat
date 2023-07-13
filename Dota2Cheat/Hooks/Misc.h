#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"

namespace Hooks {
	inline void* oSaveSerializedSOCache{};
	inline void* hkSaveSerializedSOCache(void* thisptr) {
		auto result = ((decltype(&hkSaveSerializedSOCache))oSaveSerializedSOCache)(thisptr);
		Modules::SkinChanger.DeleteSOCacheFiles();
		d2c.SaveConfig();
		//{
		//	std::ofstream fout(d2c.cheatFolderPath + "\\config\\inventory.json");
		//	Config::cfg.SaveEquippedItems(fout);
		//	fout.close();
		//}
		return result;
	}
}